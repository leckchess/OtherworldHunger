// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/OWHIngredient.h"

// Sets default values
AOWHIngredient::AOWHIngredient()
{
	PrimaryActorTick.bCanEverTick = true;

	IngredientMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	IngredientName = FText::FromString("BasicIngredient");
}

void AOWHIngredient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOWHIngredient::BeginPlay()
{
	Super::BeginPlay();
}


FText AOWHIngredient::GetIngredientName() const
{
	return IngredientName;
}

void AOWHIngredient::SetIngredientName(const FText& NameOfIngredient)
{
	this->IngredientName = NameOfIngredient;
}


void AOWHIngredient::SetIngredientMesh(UStaticMeshComponent* IngredientMeshComp)
{
	IngredientMesh = IngredientMeshComp;
}
