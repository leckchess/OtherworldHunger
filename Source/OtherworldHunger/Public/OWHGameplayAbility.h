// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "GameplayTagContainer.h"
#include "OWHGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class OTHERWORLDHUNGER_API UOWHGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	virtual void OnInputStatusChanged(const FInputActionValue& Value);

private:
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AbilityInputAction; 
	
	UPROPERTY(EditAnywhere, Category = "Input")
	ETriggerEvent AbilityInputTriggerEvent = ETriggerEvent::Started;

	UPROPERTY(EditAnywhere, Category="Input")
	bool ToggableAbility = false;

	UPROPERTY(EditAnywhere, Category="Audio")
	FGameplayTag AudioTag;
};
