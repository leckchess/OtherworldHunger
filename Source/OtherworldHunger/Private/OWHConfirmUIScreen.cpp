// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHConfirmUIScreen.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UOWHConfirmUIScreen::Show(FString Message, FString LevelName)
{
    LevelChange = LevelName;
    TravelText->SetText(FText::FromString(Message));
    this->SetVisibility(ESlateVisibility::Visible);
}

void UOWHConfirmUIScreen::Hide()
{
    this->SetVisibility(ESlateVisibility::Collapsed);
}