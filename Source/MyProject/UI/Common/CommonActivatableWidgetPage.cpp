// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonActivatableWidgetPage.h"

void UCommonActivatableWidgetPage::ShowPage()
{
	SetVisibility(ESlateVisibility::Visible);
	ActivateWidget();
}

void UCommonActivatableWidgetPage::HidePage()
{
	SetVisibility(ESlateVisibility::Hidden);
	DeactivateWidget();
}

void UCommonActivatableWidgetPage::NativeOnActivated()
{
	Super::NativeOnActivated();
	if (GetDesiredFocusTarget())
	{
		SetFocus();
	}
}

bool UCommonActivatableWidgetPage::NativeOnHandleBackAction()
{
	HandleBackAction.Broadcast();
	//BP_OnHandleBackActionReal();
	return true;
}

