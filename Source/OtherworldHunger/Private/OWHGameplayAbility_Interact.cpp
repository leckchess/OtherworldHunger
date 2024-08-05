// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHGameplayAbility_Interact.h"
#include "GameFramework/Character.h"
#include "OWHCharacter.h"
#include "OWHInteractableInterface.h"
#include "Components/OWHCharacterInventory.h"
#include "Components/SphereComponent.h"
#include "OWHAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/CapsuleComponent.h"


bool UOWHGameplayAbility_Interact::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor);

	TArray<AActor*> OverlappingActors;
	OwnerCharacter->GetOverlappingActors(OverlappingActors, UOWHInteractableInterface::StaticClass());

	if (OverlappingActors.Num() == 0) { return false; }

	bool FrontItemFound = false;
	for (AActor* Interactable : OverlappingActors)
	{
		FVector Dir = Interactable->GetActorLocation() - (OwnerCharacter->GetActorLocation() - (OwnerCharacter->GetActorUpVector() * OwnerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
		Dir.Normalize();

		float DotProduct = FVector::DotProduct(OwnerCharacter->GetActorForwardVector(), Dir);

		if (DotProduct > 0)
		{
			FrontItemFound = true;
			break;
		}
	}

	if (FrontItemFound == false)
	{
		return false;
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UOWHGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (OwnerCharacter == nullptr) { return; }

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	DoInteract(OwnerCharacter);
}

void UOWHGameplayAbility_Interact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UOWHGameplayAbility_Interact::DoInteract(ACharacter* OwnerCharacter)
{
	AOWHCharacter* OWHCharacter = Cast<AOWHCharacter>(OwnerCharacter);

	TArray<AActor*> OverlappingActors;
	OWHCharacter->GetOverlappingActors(OverlappingActors, UOWHInteractableInterface::StaticClass());
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor)
		{
			if (IOWHInteractableInterface* Interactable = Cast<IOWHInteractableInterface>(Actor))
			{
				FVector Dir = Actor->GetActorLocation() - (OwnerCharacter->GetActorLocation() - (OwnerCharacter->GetActorUpVector() * OwnerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
				Dir.Normalize();

				float DotProduct = FVector::DotProduct(OwnerCharacter->GetActorForwardVector(), Dir);

				if (DotProduct > 0)
				{
					Interactable->Interact_Implementation(OWHCharacter);
					break;
				}
			}
		}

		else if (Actor && Actor->ActorHasTag("SkystoneLevel"))
		{
			OWHCharacter->ShowConfirmation("Go to the Dunes of Skystone?");
		}
	}

	K2_CancelAbility();
}
