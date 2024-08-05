// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OWHConfirmUIScreen.generated.h"

/**
 * 
 */
UCLASS()
class OTHERWORLDHUNGER_API UOWHConfirmUIScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	void Show(FString Message, FString LevelName);

	UFUNCTION(BlueprintCallable)
	void Hide();

	UPROPERTY(BlueprintReadOnly)
	FString LevelChange;

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess))
	class UTextBlock* TravelText;
};
