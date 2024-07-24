// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHAbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "OWHGameplayAbility.h"
#include "GameFramework/Character.h"

void UOWHAbilitySystemComponent::InitAbilities()
{
	if (InitialAbilities.Num() == 0)
	{
		return;
	}
	for (const TPair<FGameplayTag, TSubclassOf<class UGameplayAbility>>& Ability : InitialAbilities)
	{
		if (Ability.Key.IsValid() == false) { continue; }

		GrantAbility(Ability.Key, Ability.Value);
	}
}

void UOWHAbilitySystemComponent::GrantAbility(const FGameplayTag& AbilityTag, const TSubclassOf<UGameplayAbility>& AbilityToGrant)
{
	if (AbilityToGrant == nullptr) { return; }

	FGameplayAbilitySpecHandle SpecHandle = GiveAbility(FGameplayAbilitySpec(AbilityToGrant, 1, -1, this));
}

bool UOWHAbilitySystemComponent::ActivateAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass)
{
	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.Ability->IsA(AbilityClass))
		{
			return TryActivateAbility(Spec.Handle);
		}
	}

	return false;
}

bool UOWHAbilitySystemComponent::ActivateAbilityByTag(const FGameplayTag& AbilityTag)
{
	const FGameplayTagContainer TagContainer = FGameplayTagContainer(AbilityTag);
	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasAll(TagContainer))
		{
			return TryActivateAbility(Spec.Handle);
		}
	}

	return false;
}

bool UOWHAbilitySystemComponent::IsAbilityActiveByClass(TSubclassOf<UGameplayAbility> AbilityClass) const
{
	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.Ability->IsA(AbilityClass))
		{
			return Spec.IsActive();
		}
	}

	return false;
}

bool UOWHAbilitySystemComponent::IsAbilityActiveByTag(const FGameplayTag& AbilityTag) const
{
	const FGameplayTagContainer TagContainer = FGameplayTagContainer(AbilityTag);
	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasAll(TagContainer))
		{
			return Spec.IsActive();
		}
	}

	return false;
}

void UOWHAbilitySystemComponent::CancelAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass)
{
	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.Ability->IsA(AbilityClass) && Spec.IsActive())
		{
			return CancelAbility(Spec.Ability);
		}
	}
}

void UOWHAbilitySystemComponent::CancelAbilityByTag(const FGameplayTag& AbilityTag)
{
	const FGameplayTagContainer TagContainer = FGameplayTagContainer(AbilityTag);
	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasAll(TagContainer) && Spec.IsActive())
		{
			return CancelAbility(Spec.Ability);
		}
	}
}