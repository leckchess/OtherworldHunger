// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "OWHAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class OTHERWORLDHUNGER_API UOWHAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void InitAbilities();
	void GrantAbility(const FGameplayTag& AbilityTag,const TSubclassOf<class UGameplayAbility>& AbilityToGrant);
	bool ActivateAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass);
	bool ActivateAbilityByTag(const FGameplayTag& AbilityTag);
	bool IsAbilityActiveByClass(TSubclassOf<UGameplayAbility> AbilityClass) const;
	bool IsAbilityActiveByTag(const FGameplayTag& AbilityTag) const;
	void CancelAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass);
	void CancelAbilityByTag(const FGameplayTag& AbilityTag);

private:
	/** Initial Abilities */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, TSubclassOf<class UGameplayAbility>> InitialAbilities;
};
