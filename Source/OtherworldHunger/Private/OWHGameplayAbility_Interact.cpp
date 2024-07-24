// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHGameplayAbility_Interact.h"
#include "GameFramework/Character.h"
#include "OWHCharacter.h"
#include "OWHInteractableInterface.h"
#include "Components/OWHCharacterInventory.h"
#include "Actors/OWHIngredient.h"
#include "OWHAbilitySystemComponent.h"
#include "../../../../../../../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemGlobals.h"


bool UOWHGameplayAbility_Interact::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                      const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
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
	OWHCharacter->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->GetClass()->ImplementsInterface(UOWHInteractableInterface::StaticClass()))
		{
			AActor* Ing = Cast<IOWHInteractableInterface>(Actor)->Interact_Implementation(OWHCharacter);
			if (Cast<AOWHIngredient>(Ing))
			{
				OWHCharacter->GetCharacterInventory()->AddIngredient(Cast<AOWHIngredient>(Ing));
				Actor->Destroy();
				OWHCharacter->GetCharacterInventory()->DisplayIngredients();
			}
		}
	}

	if (UOWHAbilitySystemComponent* OwningAbilityComponent = Cast<UOWHAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwningActorFromActorInfo())))
	{
		OwningAbilityComponent->CancelAbilityByClass(GetClass());
	}
	else
	{
		K2_CancelAbility();;
	}
}
