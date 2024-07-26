// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "OWHQuestsManager.h"
#include "OWHRecipeUIScreen.generated.h"

/**
 * 
 */
UCLASS()
class OTHERWORLDHUNGER_API UOWHRecipeUIScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateRecipe(FRecipeDataTable* NewRecipe);

	void UpdateIngredientCount(const FGameplayTag& IngredientTag, int32 NewCount);
	
	void Show();
	void Hide();

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess))
	class UTextBlock* Text_RecipeName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess))
	class UVerticalBox* VerticalBox_Ingredients;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetAnimOptional, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> Animation;

	UPROPERTY(EditAnywhere, Category = Item)
	TSubclassOf<class UOWHIngredientUIItem> IngredientItemClass;

	TMap<FGameplayTag, class UOWHIngredientUIItem*> CreatedWidgets;
};
