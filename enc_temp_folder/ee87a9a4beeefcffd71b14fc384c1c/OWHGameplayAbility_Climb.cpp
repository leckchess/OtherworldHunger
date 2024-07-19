// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHGameplayAbility_Climb.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

bool UOWHGameplayAbility_Climb::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */, const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor))
	{
		if (OwnerCharacter->GetCharacterMovement() == nullptr)
		{
			return false;
		}
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UOWHGameplayAbility_Climb::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (OwnerCharacter == nullptr || OwnerCharacter->GetCharacterMovement() == nullptr) { return; }

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement())
	{
		FVector StartTrace = OwnerCharacter->GetActorLocation();
		FVector EndTrace = StartTrace + OwnerCharacter->GetActorForwardVector() * AttachmentDistance;

		FHitResult HitResult;

		OwnerCharacter->GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility);

		if (HitResult.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Climb Ability Start"));

			CharacterMovementComponent->SetMovementMode(MOVE_Flying);
			CharacterMovementComponent->bOrientRotationToMovement = false;

			UCapsuleComponent* PlayerCapsule = OwnerCharacter->GetCapsuleComponent();

			FVector AttachLocation = HitResult.Location + PlayerCapsule->GetUnscaledCapsuleRadius() * HitResult.Normal;

			UKismetSystemLibrary::MoveComponentTo(PlayerCapsule, AttachLocation, UKismetMathLibrary::MakeRotFromX(-HitResult.Normal), false, false, 0.2f, false, EMoveComponentAction::Type::Move, FLatentActionInfo());
		}
	}
}

void UOWHGameplayAbility_Climb::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UE_LOG(LogTemp, Error, TEXT("Climb Ability End"));
}
