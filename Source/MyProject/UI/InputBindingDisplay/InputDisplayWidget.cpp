#include "InputDisplayWidget.h"

#include "CommonInputSubsystem.h"
#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "EnhancedPlayerInput.h"
#include "InputMappingContext.h"

void UInputDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeDisplay();
}

void UInputDisplayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	DisplayText->SetText(DescriptionText);
}

bool UInputDisplayWidget::RetrieveKey(FKey& OutKey)
{
	if (!Context) { UE_LOG(LogTemp, Error, TEXT("Invalid context in UInputDisplayWidget::RetrieveKey")); return false; }
	if (!Action) { UE_LOG(LogTemp, Error, TEXT("Invalid action in UInputDisplayWidget::RetrieveKey")); return false; }

	const bool bUsingGamepad = PlayerIsUsingGamepad();
	const FEnhancedActionKeyMapping* BestMatch = nullptr;

	for (const FEnhancedActionKeyMapping& Mapping : Context->GetMappings())
	{
		if (Mapping.Action != Action)
			continue;

		const FKey& Key = Mapping.Key;

		if (bUsingGamepad && Key.IsGamepadKey())
		{
			BestMatch = &Mapping;
			break;
		}
		if (!bUsingGamepad && (IsKeyboardKey(Key) || IsMouseKey(Key)))
		{
			BestMatch = &Mapping;
			break;
		}
	}

	// Fallback if nothing matched the device
	if (!BestMatch)
	{
		for (const FEnhancedActionKeyMapping& Mapping : Context->GetMappings())
		{
			if (Mapping.Action == Action)
			{
				BestMatch = &Mapping;
				break;
			}
		}
	}

	if (BestMatch)
	{
		//UE_LOG(LogTemp, Log, TEXT("Device-based mapping: %s -> %s"), *Action->GetName(), *BestMatch->Key.ToString());
		OutKey = BestMatch->Key;
		return true;
	}
	return false;
}

void UInputDisplayWidget::InitializeDisplay()
{
	if (!Context) { UE_LOG(LogTemp, Error, TEXT("Invalid context in UInputDisplayWidget::InitializeDisplay")); return; }
	if (!Action) { UE_LOG(LogTemp, Error, TEXT("Invalid action in UInputDisplayWidget::InitializeDisplay")); return; }
	if (!InputMapData) return;

	if (FKey Key; RetrieveKey(Key))
	{
		if (const FInputPromptData* PromptData = InputMapData->GetInputPromptData(Key))
		{
			if (bUseImage)
			{
				if (PromptData->Icon)
				{
					InputImage->SetVisibility(ESlateVisibility::Visible);
					InputImage->SetBrushFromTexture(PromptData->Icon);
					DisplayText->SetText(DescriptionText);
					return;
				}
			}
			InputImage->SetVisibility(ESlateVisibility::Collapsed);
			FText NewDisplayText = FText::Format(FText::FromString("[{0}] - {1}"), PromptData->DisplayText, DescriptionText);
			DisplayText->SetText(NewDisplayText);
		}
	}
	else { UE_LOG(LogTemp, Error, TEXT("Unable to retrieve key in UInputDisplayWidget::InitializeDisplay")); }
}
bool UInputDisplayWidget::PlayerIsUsingGamepad() const
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (UCommonInputSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UCommonInputSubsystem>(PC->GetLocalPlayer()))
		{
			ECommonInputType InputType = InputSubsystem->GetCurrentInputType();
			return InputType == ECommonInputType::Gamepad;
		}
	}
	return false;
}

bool UInputDisplayWidget::IsGamepadKey(const FKey& Key)
{
	return Key.IsGamepadKey();
}

bool UInputDisplayWidget::IsKeyboardKey(const FKey& Key)
{
	return Key.GetFName().ToString().StartsWith("Keyboard");
}

bool UInputDisplayWidget::IsMouseKey(const FKey& Key)
{
	// Mouse buttons start with "Mouse"
	return Key.GetFName().ToString().StartsWith("Mouse");
}