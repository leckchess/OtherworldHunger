// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHNotificationUIScreen.h"
#include "Components/TextBlock.h"

void UOWHNotificationUIScreen::ShowNoticication(FString Message, ENotificationType NotificationType)
{
	if (Text_Notification == nullptr) { return; }

	Text_Notification->SetText(FText::FromString(Message));

	if (NotificationColorMapping.Contains(NotificationType))
	{
		Text_Notification->SetColorAndOpacity(NotificationColorMapping[NotificationType]);
	}

	Show();
}

void UOWHNotificationUIScreen::Show()
{
	if (Animation == nullptr) { return; }
	PlayAnimationForward(Animation);

	GetWorld()->GetTimerManager().SetTimer(Notification_Handle, this, &UOWHNotificationUIScreen::Hide, NotificationTime, false);
}

void UOWHNotificationUIScreen::Hide()
{
	if (Animation == nullptr) { return; }
	PlayAnimationReverse(Animation);
}
