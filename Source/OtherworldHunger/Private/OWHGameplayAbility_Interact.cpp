// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHGameplayAbility_Interact.h"
#include "GameFramework/Character.h"
#include "OWHCharacter.h"
#include "OWHInteractableInterface.h"
#include "Components/OWHCharacterInventory.h"
#include "Actors/OWHIngredient.h"
#include "Components/SphereComponent.h"
#include "OWHAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"


bool UOWHGameplayAbility_Interact::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                      const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	ACharacter* OWHCharacter = Cast<ACharacter>(ActorInfo->AvatarActor);

	TArray<AActor*> OverlappingActors;
	OWHCharacter->GetOverlappingActors(OverlappingActors, UOWHInteractableInterface::StaticClass());

	if (OverlappingActors.Num() == 0) { return false; }

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
		if (Actor && Actor->GetClass()->IsChildOf(AOWHIngredient::StaticClass()))
		{
			if (AActor* Ing = Cast<IOWHInteractableInterface>(Actor)->Interact_Implementation(OWHCharacter); Cast<AOWHIngredient>(Ing))
			{
				OWHCharacter->GetCharacterInventory()->AddIngredient(Cast<AOWHIngredient>(Ing));
				Cast<AOWHIngredient>(Actor)->IngredientMesh->DestroyComponent();
				Cast<AOWHIngredient>(Actor)->InteractSphere->DestroyComponent();
				OWHCharacter->GetCharacterInventory()->DisplayIngredients();
			}
		}

		else if (Actor && Actor->ActorHasTag("SkystoneLevel"))
		{
			OWHCharacter->ShowConfirmation("Go to the Dunes of Skystone?");
		}
	}

	K2_CancelAbility();
}
