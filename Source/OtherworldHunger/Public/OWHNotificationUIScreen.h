// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OWHNotificationUIScreen.generated.h"

UENUM(BlueprintType)
enum class ENotificationType : uint8
{
	EMessage = 0,
	EWarning,
	EError
};

/**
 *
 */
UCLASS()
class OTHERWORLDHUNGER_API UOWHNotificationUIScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowNoticication(FString Message, ENotificationType NotificationType);

private:
	void Show();
	void Hide();

public:
	UPROPERTY(EditAnywhere, Category = "Notification")
	TMap<ENotificationType, FColor> NotificationColorMapping;

	UPROPERTY(EditAnywhere, Category = "Notification")
	float NotificationTime = 1.f;

private:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess))
	class UTextBlock* Text_Notification;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnimOptional, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> Animation;

	FTimerHandle Notification_Handle;
};
