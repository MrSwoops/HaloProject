#include "InteractionUI.h"

#include "CommonTextBlock.h"

void UInteractionUI::UpdateInteractionText(const FString& Text)
{
	InteractionText->SetText(FText::FromString(Text));
}

void UInteractionUI::ClearInteractionText()
{
	InteractionText->SetText(FText::GetEmpty());
}
