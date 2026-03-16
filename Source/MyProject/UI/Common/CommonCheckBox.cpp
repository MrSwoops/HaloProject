#include "CommonCheckBox.h"

UCommonCheckBox::UCommonCheckBox()
{
	//SetIsSelectable(true);
}

void UCommonCheckBox::NativePreConstruct()
{
	SetLabel(FText::FromString(Label));
}

void UCommonCheckBox::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateVisualState();
	SetLabel(FText::FromString(Label));
}

void UCommonCheckBox::NativeOnClicked()
{
	Super::NativeOnClicked();

	bIsOn = !bIsOn;
	
	UpdateVisualState();
	OnToggleChanged.Broadcast(bIsOn);
}

void UCommonCheckBox::SetIsOn(const bool bIsSelected)
{
	if (bIsOn == bIsSelected) return;

	bIsOn = bIsSelected;
	
	UpdateVisualState();
	OnToggleChanged.Broadcast(bIsOn);
}

bool UCommonCheckBox::IsOn() const
{
	return bIsOn;
}

void UCommonCheckBox::SetLabel(const FText& NewLabel)
{
	if (LabelText)
	{
		LabelText->SetText(NewLabel);
	}
}

void UCommonCheckBox::UpdateVisualState()
{
	if (CheckmarkImage)
	{
		CheckmarkImage->SetVisibility(IsOn() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}