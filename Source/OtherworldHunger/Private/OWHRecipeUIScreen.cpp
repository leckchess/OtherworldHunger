// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHRecipeUIScreen.h"
#include "Components/TextBlock.h"
#include "OWHIngredientUIItem.h"
#include "Components/VerticalBox.h"

void UOWHRecipeUIScreen::UpdateRecipe(FRecipeDataTable* NewRecipe)
{
	if (IngredientItemClass == nullptr || VerticalBox_Ingredients == nullptr || NewRecipe->Ingredients.Num() == 0) 
	{
		return; 
	}

	VerticalBox_Ingredients->ClearChildren();
	CreatedWidgets.Empty();

	if (Text_RecipeName->IsValidLowLevel())
	{
		Text_RecipeName->SetText(FText::FromString(NewRecipe->RecipeName));
	}

	for (TPair<FGameplayTag, int32>& Ingredient : NewRecipe->Ingredients)
	{
		UOWHIngredientUIItem* IngredientItem = CreateWidget<UOWHIngredientUIItem>(this, IngredientItemClass);

		FString IngredientName = Ingredient.Key.GetTagName().ToString();
		IngredientName.RemoveFromStart("Ingredient.");

		IngredientItem->InitItem(IngredientName, Ingredient.Value);
		VerticalBox_Ingredients->AddChild(IngredientItem);
		CreatedWidgets.Add(Ingredient.Key, IngredientItem);
	}

	Show();
}

void UOWHRecipeUIScreen::UpdateIngredientCount(const FGameplayTag& IngredientTag, int32 NewCount)
{
	if (CreatedWidgets.Contains(IngredientTag))
	{
		CreatedWidgets[IngredientTag]->UpdateCount(NewCount);
	}
}

void UOWHRecipeUIScreen::Show()
{
	if (Animation == nullptr) { return; }

	PlayAnimationForward(Animation);
}

void UOWHRecipeUIScreen::Hide()
{
	if (Animation == nullptr) { return; }

	PlayAnimationReverse(Animation);
}
