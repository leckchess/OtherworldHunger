// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHIngredientUIItem.h"
#include "Components/TextBlock.h"

void UOWHIngredientUIItem::InitItem(FString IngredientName, int32 IngredientTotalCount)
{
	if (IngredientName == "" || IngredientTotalCount == 0) { return; }

	TotalCount = IngredientTotalCount;

	if (Text_IngredientName)
	{
		Text_IngredientName->SetText(FText::FromString(IngredientName));
	}

	if (Text_IngredientCount)
	{
		Text_IngredientCount->SetText(FText::FromString("0"));
	}

	if (Text_IngredientTotalCount)
	{
		Text_IngredientTotalCount->SetText(FText::FromString(FString::FromInt(IngredientTotalCount)));
	}
}

void UOWHIngredientUIItem::UpdateCount(int32 NewCount)
{
	NewCount = FMath::Clamp(NewCount, 0, TotalCount);

	if (Text_IngredientCount)
	{
		Text_IngredientCount->SetText(FText::FromString(FString::FromInt(NewCount)));
	}
}
