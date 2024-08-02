// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OWHCookingPot.generated.h"

UCLASS()
class OTHERWORLDHUNGER_API AOWHCookingPot : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Cook(bool StartCooking);
};
