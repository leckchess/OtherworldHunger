// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "OWHQuestsManager.h"
#include "OWHCharacter.generated.h"

UCLASS()
class OTHERWORLDHUNGER_API AOWHCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AOWHCharacter();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnRecipeUpdate(FRecipeDataTable* NewRecipe);

	UFUNCTION(BlueprintCallable)
	void SetPlayerHUD(UUserWidget* InPlayerHUD);

	void OnIngredientAddedToInventory(const FGameplayTag& IngredientTag, int32 NewCount);
	void UpdateRecipe(FRecipeDataTable* NewRecipe);

protected:
	virtual void PossessedBy(AController* NewController) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UPROPERTY(BlueprintReadWrite)
	class UOWHCharacterInventory* CharacterInventory;

public:
	UOWHCharacterInventory* GetCharacterInventory() const;

	UOWHQuestsManager* GetQuestsManager() const { return QuestManagerComponent; }

private:
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	class UOWHAbilitySystemComponent* GetOWHAbilitySystemComponent();

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Ability system comp */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystemComponent;
	
	/** Ability system comp */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UOWHQuestsManager* QuestManagerComponent;

	/** MappingContext */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	class UOWHPlayerHUD* PlayerHUD;
};
