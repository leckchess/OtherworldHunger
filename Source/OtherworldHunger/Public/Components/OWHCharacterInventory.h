// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/OWHIngredient.h"
#include "Components/ActorComponent.h"
#include "OWHCharacterInventory.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OTHERWORLDHUNGER_API UOWHCharacterInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UOWHCharacterInventory();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	TMap<TSubclassOf<AOWHIngredient>, int32> IngredientMap;

	UFUNCTION(BlueprintCallable)
	void AddIngredient(TSubclassOf<AOWHIngredient> Ingredient);
};
