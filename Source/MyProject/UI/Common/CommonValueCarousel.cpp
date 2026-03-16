#include "CommonValueCarousel.h"

#include "CommonButtonBase.h"
#include "CommonTextBlock.h"

void UCommonValueCarousel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (LeftButton) { LeftButton->OnClicked().AddUObject(this, &UCommonValueCarousel::PreviousOption); LeftButton->SetVisibility(ESlateVisibility::Hidden); }
	if (RightButton) { RightButton->OnClicked().AddUObject(this, &UCommonValueCarousel::NextOption); RightButton->SetVisibility(ESlateVisibility::Hidden); }

	UpdateDisplay();
}

void UCommonValueCarousel::NextOption()
{
	if (Options.Num() == 0) return;
	
	if (bLoopOptions) CurrentIndex = (CurrentIndex + 1) % Options.Num();
	else
	{
		if (CurrentIndex < Options.Num() - 1) CurrentIndex++;
	}

	UpdateDisplay();
	OnSettingChanged.Broadcast(CurrentIndex);
}

void UCommonValueCarousel::PreviousOption()
{
	if (Options.Num() == 0) return;

	if (bLoopOptions)
	{
		CurrentIndex--;
		if (CurrentIndex < 0) CurrentIndex = Options.Num() - 1;
	}
	else
	{
		if (CurrentIndex > 0) CurrentIndex--;
	}

	UpdateDisplay();
	OnSettingChanged.Broadcast(CurrentIndex);
}
void UCommonValueCarousel::SetOption(int32 Index)
{
	if (!Options.IsValidIndex(Index)) return;
	CurrentIndex = Index;
	UpdateDisplay();
	OnSettingChanged.Broadcast(CurrentIndex);
}

void UCommonValueCarousel::UpdateDisplay()
{
	if (ValueText && Options.IsValidIndex(CurrentIndex))
	{
		ValueText->SetText(Options[CurrentIndex]);
	}

	if (!LeftButton || !RightButton) return;

	if (!bIsHovered)
	{
		LeftButton->SetVisibility(ESlateVisibility::Hidden);
		RightButton->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (bLoopOptions)
	{
		LeftButton->SetVisibility(ESlateVisibility::Visible);
		RightButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		LeftButton->SetVisibility(CurrentIndex == 0 ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		RightButton->SetVisibility(CurrentIndex == Options.Num() - 1 ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}
}

void UCommonValueCarousel::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	bIsHovered = true;
	UpdateDisplay();
}
void UCommonValueCarousel::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bIsHovered = false;
	LeftButton->SetVisibility(ESlateVisibility::Hidden);
	RightButton->SetVisibility(ESlateVisibility::Hidden);
}
