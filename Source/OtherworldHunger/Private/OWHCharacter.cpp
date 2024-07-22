// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "OWHGameplayAbility_Climb.h"
#include "OWHGameplayAbility_Interact.h"
#include "OWHInteractableInterface.h"
#include "Actors/OWHIngredient.h"
#include "Components/OWHCharacterInventory.h"
#include "GameFramework/CharacterMovementComponent.h"

AOWHCharacter::AOWHCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	CharacterInventory = CreateDefaultSubobject<UOWHCharacterInventory>(TEXT("CharacterInventoryComp"));
	PrimaryActorTick.bCanEverTick = true;
}

void AOWHCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AOWHCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitAbilities();
	}
}

void AOWHCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOWHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOWHCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOWHCharacter::Look);

		//Climb
		EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Started, this, &AOWHCharacter::Climb);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AOWHCharacter::Interact);
	}
}

void AOWHCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AOWHCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AOWHCharacter::Climb(const FInputActionValue& Value)
{
	if (AbilitySystemComponent == nullptr) { return; }

	UClass* AbilityClass = UOWHGameplayAbility_Climb::StaticClass();
	if (AbilitiesMapping.Contains(AbilityClass) == false) { return; }

	if (Value.Get<bool>())
	{
		if (IsAbilityActive(AbilityClass))
		{
			CancelAbility(AbilityClass);
		}
		else
		{
			TryActivateAbility(AbilityClass);
		}
	}
}

void AOWHCharacter::Interact(const FInputActionValue& Value)
{
	// if (AbilitySystemComponent == nullptr) { return; }
	//
	// UClass* AbilityClass = UOWHGameplayAbility_Interact::StaticClass();
	// if (AbilitiesMapping.Contains(AbilityClass) == false) { return; }
	//
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("passed return"));
	// if (Value.Get<bool>())
	// {
	// 	if (IsAbilityActive(AbilityClass))
	// 	{
	// 		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("cancel ability"));
	// 		CancelAbility(AbilityClass);
	// 	}
	// 	else
	// 	{
	// 		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("trying to activate ability"));
	// 		TryActivateAbility(AbilityClass);
	// 	}
	// }
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->GetClass()->ImplementsInterface(UOWHInteractableInterface::StaticClass()))
		{
			AActor* Ing = Cast<IOWHInteractableInterface>(Actor)->Interact_Implementation(this);
			if (Cast<AOWHIngredient>(Ing))
			{
				CharacterInventory->AddIngredient(Cast<AOWHIngredient>(Ing));
				Actor->Destroy();
				CharacterInventory->DisplayIngredients();
			}
		}
	}
}

void AOWHCharacter::InitAbilities()
{
	if (AbilitySystemComponent == nullptr || InitialAbilities.Num() == 0)
	{
		return;
	}
	for (const TPair<FGameplayTag, TSubclassOf<class UGameplayAbility>>& Ability : InitialAbilities)
	{
		if (Ability.Key.IsValid() == false) { continue; }

		GrandAbility(Ability.Key, Ability.Value);
	}
}

void AOWHCharacter::GrandAbility(const FGameplayTag& AbilityTag, TSubclassOf<UGameplayAbility> AbilityToGrand)
{
	if (AbilitySystemComponent == nullptr || AbilityToGrand == nullptr) { return; }

	GrandedAbilities.Add(AbilityTag, AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToGrand)));
	AbilitiesMapping.Add(AbilityToGrand->GetSuperClass(), AbilityTag);
}

UOWHCharacterInventory* AOWHCharacter::GetCharacterInventory() const
{
	return CharacterInventory;
}

bool AOWHCharacter::TryActivateAbility(UClass* AbilityClass)
{
	if (AbilitiesMapping.Contains(UOWHGameplayAbility_Climb::StaticClass()) == false) { return false; }

	FGameplayTag AbilityTag = AbilitiesMapping[AbilityClass];

	if (GrandedAbilities.Contains(AbilityTag) == false) { return false; }

	FGameplayAbilitySpecHandle AbilityHandle = GrandedAbilities[AbilityTag];
	if (AbilitySystemComponent->TryActivateAbility(AbilityHandle))
	{
		ActiveAbilities.AddUnique(AbilityTag);
		return true;
	}

	return false;
}

bool AOWHCharacter::IsAbilityActive(UClass* AbilityClass)
{
	if (AbilitiesMapping.Contains(UOWHGameplayAbility_Climb::StaticClass()) == false) { return false; }

	FGameplayTag AbilityTag = AbilitiesMapping[AbilityClass];

	return ActiveAbilities.Contains(AbilityTag);
}

void AOWHCharacter::CancelAbility(UClass* AbilityClass)
{
	if (AbilitiesMapping.Contains(UOWHGameplayAbility_Climb::StaticClass()) == false) { return; }

	FGameplayTag AbilityTag = AbilitiesMapping[AbilityClass];

	if (GrandedAbilities.Contains(AbilityTag) == false) { return; }

	FGameplayAbilitySpecHandle AbilityHandle = GrandedAbilities[AbilityTag];
	AbilitySystemComponent->CancelAbilityHandle(AbilityHandle);

	if (ActiveAbilities.Contains(AbilityTag))
	{
		ActiveAbilities.Remove(AbilityTag);
	}
}
