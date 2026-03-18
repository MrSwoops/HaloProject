// Fill out your copyright notice in the Description page of Project Settings.


#include "ForgeModeHudWidget.h"
#include "ObjectEditWindow/ForgeObjectEditMenu.h"
#include "ObjectsWindow/ForgeObjectsWidget.h"

void UForgeModeHudWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ObjectsWindow) ObjectsWindow->InitializeCategories();
	SetObjectEditWindowEnabled(false);
	SetObjectsWindowEnabled(false);
	UpdateInputDisplay();
}

void UForgeModeHudWidget::SetObjectEditWindowEnabled(bool Enabled)
{
	if (Enabled && bSubWindowOpen) return;
	ObjectEditWindow->SetVisibility(Enabled ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	InputDisplayBox->SetVisibility(Enabled ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	bSubWindowOpen = Enabled;
}

void UForgeModeHudWidget::SetObjectsWindowEnabled(bool Enabled)
{
	if (Enabled && bSubWindowOpen) return;
	ObjectsWindow->SetVisibility(Enabled ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	InputDisplayBox->SetVisibility(Enabled ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	ObjectsWindow->WidgetSwitcher->SetActiveWidgetIndex(0);
	bSubWindowOpen = Enabled;
}

void UForgeModeHudWidget::UpdateInputDisplay(const bool& HoldingObject, const bool& HoveringObject)
{
	if (HoldingObject) // have object
	{
		DeleteObjectInputDisplay->SetVisibility(ESlateVisibility::Visible);
		PickupObjectInputDisplay->SetVisibility(ESlateVisibility::Collapsed);
		PlaceObjectInputDisplay->SetVisibility(ESlateVisibility::Visible);
		OpenObjectsWindowInputDisplay->SetVisibility(ESlateVisibility::Collapsed);
		OpenEditObjectWindowInputDisplay->SetVisibility(ESlateVisibility::Visible);
	}
	else if (HoveringObject) // hovering an object
	{
		DeleteObjectInputDisplay->SetVisibility(ESlateVisibility::Visible);
		PickupObjectInputDisplay->SetVisibility(ESlateVisibility::Visible);
		PlaceObjectInputDisplay->SetVisibility(ESlateVisibility::Collapsed);
		OpenObjectsWindowInputDisplay->SetVisibility(ESlateVisibility::Visible);
		OpenEditObjectWindowInputDisplay->SetVisibility(ESlateVisibility::Visible);
	}
	else 
	{
		DeleteObjectInputDisplay->SetVisibility(ESlateVisibility::Collapsed);
		PickupObjectInputDisplay->SetVisibility(ESlateVisibility::Collapsed);
		PlaceObjectInputDisplay->SetVisibility(ESlateVisibility::Collapsed);
		OpenObjectsWindowInputDisplay->SetVisibility(ESlateVisibility::Visible);
		OpenEditObjectWindowInputDisplay->SetVisibility(ESlateVisibility::Collapsed);
	}
}
