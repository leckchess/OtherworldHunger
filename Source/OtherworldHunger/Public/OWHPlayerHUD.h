// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OWHQuestsManager.h"
#include "CommonUserWidget.h"
#include "OWHNotificationUIScreen.h"
#include "OWHPlayerHUD.generated.h"

/**
 *
 */
UCLASS()
class OTHERWORLDHUNGER_API UOWHPlayerHUD : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void UpdateRecipe(FRecipeDataTable* NewRecipe);

	void OnIngredientAddedToInventory(const FGameplayTag& IngredientTag, int32 NewCount);

	void ShowNoticication(FString Message, ENotificationType NotificationType);

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess))
	class UOWHRecipeUIScreen* WBP_OWH_Recipe; 
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess))
	UOWHNotificationUIScreen* WBP_OWH_Notification;
};
