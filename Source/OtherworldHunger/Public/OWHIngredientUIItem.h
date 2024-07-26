// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OWHIngredientUIItem.generated.h"

/**
 * 
 */
UCLASS()
class OTHERWORLDHUNGER_API UOWHIngredientUIItem : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitItem(FString IngredientName, int32 IngredientTotalCount);

	void UpdateCount(int32 NewCount);
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess))
	class UTextBlock* Text_IngredientName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess))
	class UTextBlock* Text_IngredientCount;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess))
	class UTextBlock* Text_IngredientTotalCount;

	int32 TotalCount = 0;
};
