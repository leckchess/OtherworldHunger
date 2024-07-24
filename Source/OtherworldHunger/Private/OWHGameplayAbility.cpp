// Fill out your copyright notice in the Description page of Project Settings.

#include "OWHGameplayAbility.h"
#include "OWHAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

void UOWHGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityInputAction == nullptr) { return; }

	if (ActorInfo->AvatarActor.Get())
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
		{
			const APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());

			if (PlayerController == nullptr)
			{
				return;
			}

			// Binding enhanced inputs
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				EnhancedInputComponent->BindAction(AbilityInputAction, AbilityInputTriggerEvent, this, &UOWHGameplayAbility::OnInputStatusChanged);
				if (ToggableAbility == false)
				{
					EnhancedInputComponent->BindAction(AbilityInputAction, ETriggerEvent::Completed, this, &UOWHGameplayAbility::OnInputStatusChanged);
				}
			}
		}
	}
}

void UOWHGameplayAbility::OnInputStatusChanged(const FInputActionValue& Value)
{
	if (UOWHAbilitySystemComponent* OwningAbilityComponent = Cast<UOWHAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwningActorFromActorInfo())))
	{
		if (AbilityTags.GetByIndex(0).IsValid())
		{
			if (Value.Get<bool>())
			{
				if (ToggableAbility && OwningAbilityComponent->IsAbilityActiveByClass(GetClass()))
				{
					OwningAbilityComponent->CancelAbilityByTag(AbilityTags.GetByIndex(0));
					return;
				}

				OwningAbilityComponent->ActivateAbilityByTag(AbilityTags.GetByIndex(0));
			}
			else
			{
				OwningAbilityComponent->CancelAbilityByTag(AbilityTags.GetByIndex(0));
			}
		}
	}
}
