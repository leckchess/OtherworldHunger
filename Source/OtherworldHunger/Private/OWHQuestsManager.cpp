// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHQuestsManager.h"
#include "OWHCharacter.h"

void UOWHQuestsManager::InitializeComp()
{
	if (RecipeDataTable == nullptr) { return; }

	const TMap<FName, uint8*>& AllRows = RecipeDataTable->GetRowMap();

	for (TMap<FName, uint8*>::TConstIterator RowMapIter(AllRows.CreateConstIterator()); RowMapIter; ++RowMapIter)
	{
		if (FRecipeDataTable* Recipe = reinterpret_cast<FRecipeDataTable*>(RowMapIter.Value()))
		{
			Recipes.Add(Recipe->RecipeName, Recipe);
		}
	}

	Recipes.GetKeys(RecipesNames);
}

bool UOWHQuestsManager::LoadRecipe()
{
	if (RecipesNames.Num() == 0) { return false; }

	AOWHCharacter* OwnerCharacter = Cast<AOWHCharacter>(GetOwner());

	if (OwnerCharacter == nullptr) { return false; }

	CurrentRecipeIndex++;
	if (CurrentRecipeIndex < RecipesNames.Num())
	{
		CurrentRecipeName = RecipesNames[CurrentRecipeIndex];

		OwnerCharacter->UpdateRecipe(Recipes[CurrentRecipeName]);

		return true;
	}

	CurrentRecipeName = "";
	OwnerCharacter->UpdateRecipe(nullptr);
	return false;
}
