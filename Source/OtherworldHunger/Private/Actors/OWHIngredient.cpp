// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/OWHIngredient.h"

// Sets default values
AOWHIngredient::AOWHIngredient()
{
	PrimaryActorTick.bCanEverTick = true;

	IngredientMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	IngredientName = "BasicIngredient";
}

void AOWHIngredient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOWHIngredient::BeginPlay()
{
	Super::BeginPlay();
}


FName AOWHIngredient::GetIngredientName() const
{
	return IngredientName;
}

void AOWHIngredient::SetIngredientName(const FName& NameOfIngredient)
{
	this->IngredientName = NameOfIngredient;
}
