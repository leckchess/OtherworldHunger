// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/OWHCharacterInventory.h"


UOWHCharacterInventory::UOWHCharacterInventory()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UOWHCharacterInventory::BeginPlay()
{
	Super::BeginPlay();
}

void UOWHCharacterInventory::AddIngredient(AOWHIngredient* Ingredient)
{
	if (IngredientMap.Find(Ingredient))
	{
		IngredientMap[Ingredient] = IngredientMap[Ingredient] + 1;
	}
	else
	{
		IngredientMap.Add(Ingredient, 1);
	}
}

void UOWHCharacterInventory::RemoveIngredient(AOWHIngredient* Ingredient)
{
	if (IngredientMap.Find(Ingredient))
	{
		if (IngredientMap[Ingredient] > 0)
		{
			IngredientMap[Ingredient] = IngredientMap[Ingredient] - 1;
			if (IngredientMap[Ingredient] == 0)
			{
				IngredientMap.Remove(Ingredient);
			}
		}
	}
}

void UOWHCharacterInventory::DisplayIngredients()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 15.0f, FColor::Yellow, TEXT("INGREDIENT LIST"));
	for (const TPair<AOWHIngredient*, int32>& Pair : IngredientMap)
	{
		FString KeyString = Pair.Key->GetIngredientName().ToString();
		FString DebugMessage = FString::Printf(TEXT("Key: %s, Value: %d"), *KeyString, Pair.Value);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMessage);
		}
	}
}


void UOWHCharacterInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
