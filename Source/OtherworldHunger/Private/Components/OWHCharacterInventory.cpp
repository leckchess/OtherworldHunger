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

void UOWHCharacterInventory::AddIngredient(TSubclassOf<AOWHIngredient> Ingredient)
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


void UOWHCharacterInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
