// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHConfirmUIScreen.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UOWHConfirmUIScreen::Show(FString Destination)
{
    TravelText->SetText(FText::FromString(Destination));
    this->SetVisibility(ESlateVisibility::Visible);
}

void UOWHConfirmUIScreen::Hide()
{
    this->SetVisibility(ESlateVisibility::Collapsed);
}