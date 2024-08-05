// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHCookingPot.h"
#include "OWHCharacter.h"

AActor* AOWHCookingPot::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn == nullptr) { return nullptr; }

	if (AOWHCharacter* OwnerCharacter = Cast<AOWHCharacter>(InstigatorPawn))
	{
		OwnerCharacter->ShowNotification("Press C to cook", ENotificationType::EWarning);
		return this;
	}

	return nullptr;
}
