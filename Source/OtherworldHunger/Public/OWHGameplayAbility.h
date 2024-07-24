// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "InputActionValue.h"
#include "InputAction.h"
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
	virtual void OnInputStatusChanged(const FInputActionValue& Value);

private:
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AbilityInputAction; 
	
	UPROPERTY(EditAnywhere, Category = "Input")
	ETriggerEvent AbilityInputTriggerEvent = ETriggerEvent::Started;

	UPROPERTY(EditAnywhere, Category="Input")
	bool ToggableAbility = false;
	
};
