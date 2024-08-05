// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OWHInteractableInterface.h"
#include "OWHCookingPot.generated.h"

UCLASS()
class OTHERWORLDHUNGER_API AOWHCookingPot : public AActor, public IOWHInteractableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Cook(bool StartCooking);

protected:
	virtual AActor* Interact_Implementation(APawn* InstigatorPawn) override;
};
