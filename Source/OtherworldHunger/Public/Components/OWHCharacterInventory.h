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

	UFUNCTION(BlueprintCallable)
	void AddIngredient(const FGameplayTag& Ingredient);

	bool HasIngredients(TMap<FGameplayTag, int32> Ingredients);

	UFUNCTION(BlueprintCallable)
	void RemoveIngredient(AOWHIngredient* Ingredient);

	void RemoveIngredients(TMap<FGameplayTag, int32> Ingredients);

	int32 GetIngredientCount(const FGameplayTag& IngredientTag) const;

	UFUNCTION(BlueprintCallable)
	void DisplayIngredients();

	UPROPERTY(BlueprintReadWrite)
	TMap<FGameplayTag, int32> IngredientMap;

protected:
	virtual void BeginPlay() override;

};
