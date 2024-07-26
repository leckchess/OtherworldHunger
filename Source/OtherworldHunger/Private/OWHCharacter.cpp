// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "OWHAbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "OWHGameplayAbility_Climb.h"
#include "OWHGameplayAbility_Interact.h"
#include "OWHInteractableInterface.h"
#include "Actors/OWHIngredient.h"
#include "Components/OWHCharacterInventory.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "OWHPlayerHUD.h"

AOWHCharacter::AOWHCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	AbilitySystemComponent = CreateDefaultSubobject<UOWHAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	QuestManagerComponent = CreateDefaultSubobject<UOWHQuestsManager>(TEXT("QuestManagerComp"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	CharacterInventory = CreateDefaultSubobject<UOWHCharacterInventory>(TEXT("CharacterInventoryComp"));
	PrimaryActorTick.bCanEverTick = true;
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

	if (GetLocalRole() == ENetRole::ROLE_Authority && GetOWHAbilitySystemComponent())
	{
		GetOWHAbilitySystemComponent()->InitAbilityActorInfo(this, this);
		GetOWHAbilitySystemComponent()->InitAbilities();
	}

	if (QuestManagerComponent)
	{
		QuestManagerComponent->InitializeComp();
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

UOWHCharacterInventory* AOWHCharacter::GetCharacterInventory() const
{
	return CharacterInventory;
}

UOWHAbilitySystemComponent* AOWHCharacter::GetOWHAbilitySystemComponent()
{
	return Cast<UOWHAbilitySystemComponent>(AbilitySystemComponent);
}

void AOWHCharacter::OnRecipeUpdate(FRecipeDataTable* NewRecipe)
{
	// tell player
	// update ui
}

void AOWHCharacter::SetPlayerHUD(UUserWidget* InPlayerHUD)
{
	PlayerHUD = Cast<UOWHPlayerHUD>(InPlayerHUD);

	if (PlayerHUD && QuestManagerComponent && QuestManagerComponent->LoadRecipe())
	{
		PlayerHUD->UpdateRecipe(QuestManagerComponent->GetCurrentRecipe());
	}
}

void AOWHCharacter::OnIngredientAddedToInventory(const FGameplayTag& IngredientTag, int32 NewCount)
{
	if (PlayerHUD && QuestManagerComponent && QuestManagerComponent->GetCurrentRecipeName() != "")
	{
		PlayerHUD->OnIngredientAddedToInventory(IngredientTag, NewCount);
	}
}

void AOWHCharacter::UpdateRecipe(FRecipeDataTable* NewRecipe)
{
	if (PlayerHUD && QuestManagerComponent && QuestManagerComponent->GetCurrentRecipeName() != "")
	{
		PlayerHUD->UpdateRecipe(NewRecipe);
		//TODO: check if the inventory already have items or not
	}
}
