// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHGameplayAbility_Cook.h"
#include "OWHCharacter.h"
#include "OWHQuestsManager.h"
#include "Components/OWHCharacterInventory.h"

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
			UE_LOG(LogTemp, Log, TEXT("Cook Ability: Yeaaaaaay Cooked Bracooooo"));
			K2_CancelAbility();
		}
	}
}

void UOWHGameplayAbility_Cook::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	RecipeManagerRef->LoadRecipe();
}
