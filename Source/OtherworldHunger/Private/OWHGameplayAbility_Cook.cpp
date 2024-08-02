// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHGameplayAbility_Cook.h"
#include "OWHCharacter.h"
#include "OWHQuestsManager.h"
#include "Components/OWHCharacterInventory.h"
#include "OWHCookingPot.h"

void UOWHGameplayAbility_Cook::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AOWHCharacter* OwnerCharacter = Cast<AOWHCharacter>(ActorInfo->AvatarActor))
	{
		if (RecipeManagerRef == nullptr)
		{
			RecipeManagerRef = OwnerCharacter->GetQuestsManager();
		}

		if (InventoryRef == nullptr)
		{
			InventoryRef = OwnerCharacter->GetCharacterInventory();
		}
	}
}

bool UOWHGameplayAbility_Cook::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */, const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor);

	if (OwnerCharacter && OwnerCharacter->GetWorld()->GetTimerManager().IsTimerActive(Cooking_TimeHandle))
	{
		UE_LOG(LogTemp, Error, TEXT("Cook Ability: Cooking Pot is busy please wait"));
		return false;
	}

	TArray<AActor*> OverlappingActors;
	OwnerCharacter->GetOverlappingActors(OverlappingActors, AOWHCookingPot::StaticClass());

	if (OverlappingActors.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Cook Ability: Please Go To the Cooking POT to be able to cook"));
		return false;
	}

	if (RecipeManagerRef && InventoryRef)
	{
		if (FRecipeDataTable* CurrentRecipe = RecipeManagerRef->GetCurrentRecipe())
		{
			if (InventoryRef->HasIngredients(CurrentRecipe->Ingredients) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("Cook Ability: Missing Ingredients"));
				return false;
			}
		}
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UOWHGameplayAbility_Cook::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (RecipeManagerRef && InventoryRef)
	{
		if (FRecipeDataTable* CurrentRecipe = RecipeManagerRef->GetCurrentRecipe())
		{
			InventoryRef->RemoveIngredients(CurrentRecipe->Ingredients);

			if (ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor))
			{
				TArray<AActor*> OverlappingActors;
				OwnerCharacter->GetOverlappingActors(OverlappingActors, AOWHCookingPot::StaticClass());
				if (OverlappingActors.Num() > 0)
				{
					CookingPOT = Cast<AOWHCookingPot>(OverlappingActors[0]);
					if(CookingPOT)
					{
						CookingPOT->Cook(true);
					}

					OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(Cooking_TimeHandle, this, &UOWHGameplayAbility_Cook::FinishCooking, CookingTime, false);
				}
			}
		}
	}
}

void UOWHGameplayAbility_Cook::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (CookingPOT)
	{
		CookingPOT->Cook(false);
	}

	if (FinishCookingAudioTag.IsValid())
	{
		if (AOWHCharacter* OwnerCharacter = Cast<AOWHCharacter>(ActorInfo->AvatarActor))
		{
			OwnerCharacter->PlaySFX(FinishCookingAudioTag);
		}
	}

	RecipeManagerRef->LoadRecipe();
}

void UOWHGameplayAbility_Cook::FinishCooking()
{
	K2_CancelAbility();
}
