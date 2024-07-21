// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OWHIngredient.generated.h"

UCLASS(BlueprintType)
class OTHERWORLDHUNGER_API AOWHIngredient : public AActor
{
	GENERATED_BODY()

public:
	AOWHIngredient();

protected:
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* IngredientMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FText IngredientName;

	//GETTERS AND SETTERS
public:
	UFUNCTION(BlueprintGetter)
	FText GetIngredientName() const;
	UFUNCTION(BlueprintSetter)
	void SetIngredientName(const FText& Ingredient);
	UFUNCTION(BlueprintSetter)
	void SetIngredientMesh(UStaticMeshComponent* IngredientMeshComp);

	virtual void Tick(float DeltaTime) override;
};
