// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OWHIngredient.generated.h"

UCLASS()
class OTHERWORLDHUNGER_API AOWHIngredient : public AActor
{
	GENERATED_BODY()

public:
	AOWHIngredient();

protected:
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* IngredientMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName IngredientName;

	//GETTERS AND SETTERS
public:
	FName GetIngredientName() const;
	void SetIngredientName(const FName& Ingredient);

	virtual void Tick(float DeltaTime) override;
};
