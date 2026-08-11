#include "VideoSettingsWidget.h"

#include "Components/Slider.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyProject/PersistentData/Settings/PlayerSettingsFile.h"
#include "MyProject/UI/Common/CommonCheckBox.h"
#include "MyProject/UI/Common/CommonValueCarousel.h"

void UVideoSettingsWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	InitializeResolution();

	UncappedFramerateCheckBox->OnToggleChanged.AddUniqueDynamic(this, &UVideoSettingsWidget::OnUncappedFramerateCheckBoxChanged);
	
	if (auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings())
	{
		FullscreenValueCarousel->SetOption(FullscreenValue(PlayerSettings->GetFullscreenMode()));
		FramerateSlider->SetValue(PlayerSettings->GetFrameRateLimit());
		UncappedFramerateCheckBox->SetIsOn(PlayerSettings->GetFrameRateLimit() == 0);
		VSyncCheckBox->SetIsOn(PlayerSettings->IsVSyncEnabled());

		ShadowQualityValueCarousel->SetOption(PlayerSettings->GetShadowQuality());
		GlobalIlluminationValueCarousel->SetOption(PlayerSettings->GetGlobalIlluminationQuality());
		ReflectionValueCarousel->SetOption(PlayerSettings->GetReflectionQuality());
		AntiAliasingValueCarousel->SetOption(PlayerSettings->GetAntiAliasingQuality());
		TextureValueCarousel->SetOption(PlayerSettings->GetTextureQuality());
		VisualEffectValueCarousel->SetOption(PlayerSettings->GetVisualEffectQuality());
		PostProcessingValueCarousel->SetOption(PlayerSettings->GetPostProcessingQuality());
		FoliageValueCarousel->SetOption(PlayerSettings->GetFoliageQuality());
		ShadingValueCarousel->SetOption(PlayerSettings->GetShadingQuality());
	}
	
	FullscreenValueCarousel->OnSettingChanged.AddUniqueDynamic(this, &UVideoSettingsWidget::OnFullscreenValueChanged);
	FramerateSlider->OnValueChanged.AddUniqueDynamic(this, &UVideoSettingsWidget::OnFramerateSliderValueChanged);
	VSyncCheckBox->OnToggleChanged.AddUniqueDynamic(this, &UVideoSettingsWidget::OnVSyncCheckBoxChanged);
	
	ShadowQualityValueCarousel->OnSettingChanged.AddUniqueDynamic(this, &UVideoSettingsWidget::OnShadowQualityValueChanged);
	GlobalIlluminationValueCarousel->OnSettingChanged.AddUniqueDynamic(this, &UVideoSettingsWidget::OnGlobalIlluminationValueChanged);
	ReflectionValueCarousel->OnSettingChanged.AddUniqueDynamic(this, &UVideoSettingsWidget::OnReflectionValueChanged);
	AntiAliasingValueCarousel->OnSettingChanged.AddUniqueDynamic(this, &UVideoSettingsWidget::OnAntiAliasingValueChanged);
	TextureValueCarousel->OnSettingChanged.AddUniqueDynamic(this, &UVideoSettingsWidget::OnTextureValueChanged);
	VisualEffectValueCarousel->OnSettingChanged.AddUniqueDynamic(this, &UVideoSettingsWidget::OnVisualEffectValueChanged);
	PostProcessingValueCarousel->OnSettingChanged.AddUniqueDynamic(this, &UVideoSettingsWidget::OnPostProcessingValueChanged);
	FoliageValueCarousel->OnSettingChanged.AddUniqueDynamic(this, &UVideoSettingsWidget::OnFoliageValueChanged);
	ShadingValueCarousel->OnSettingChanged.AddUniqueDynamic(this, &UVideoSettingsWidget::OnShadingValueChanged);
}

void UVideoSettingsWidget::OnFullscreenValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetFullscreenMode(FullscreenMode(Value));
	
}
EWindowMode::Type UVideoSettingsWidget::FullscreenMode(const int32 Value)
{
	EWindowMode::Type WindowMode;
	switch (Value)
	{
	case 0:
		WindowMode = EWindowMode::Fullscreen; break;
	case 1:
		WindowMode = EWindowMode::WindowedFullscreen; break;
	case 2:
		WindowMode = EWindowMode::Windowed; break;
	default:
		WindowMode = EWindowMode::Fullscreen; break;
	}
	return WindowMode;
}
int32 UVideoSettingsWidget::FullscreenValue(EWindowMode::Type Mode)
{
	int32 WindowValue;
	switch (Mode)
	{
	case EWindowMode::Fullscreen:
		WindowValue = 0; break;
	case EWindowMode::WindowedFullscreen:
		WindowValue = 1; break;
	case EWindowMode::Windowed:
		WindowValue = 2; break;
	default:
		WindowValue = 0; break;
	}
	return WindowValue;
}

void UVideoSettingsWidget::OnResolutionValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetScreenResolution(AvailableResolutions[Value]);
}
void UVideoSettingsWidget::InitializeResolution()
{
	if (!ResolutionValueCarousel) return;
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;

	TArray<FIntPoint> Resolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
	//Resolutions.Sort([](const FIntPoint& A, const FIntPoint& B){ return A.X * A.Y < B.X * B.Y; });

	// Populate available resolutions
	ResolutionValueCarousel->Options.Empty();
	for (const FIntPoint& Res : Resolutions)
	{
		FString Text = FString::Printf(TEXT("%dx%d"), Res.X, Res.Y);
		ResolutionValueCarousel->Options.Add(FText::FromString(Text));
	}
	AvailableResolutions = Resolutions;
	ResolutionValueCarousel->SetOption(GetResolutionIndex(PlayerSettings->GetScreenResolution()));
	
	ResolutionValueCarousel->OnSettingChanged.AddUniqueDynamic(this, &UVideoSettingsWidget::OnResolutionValueChanged);
}
int32 UVideoSettingsWidget::GetResolutionIndex(FIntPoint Resolution)
{
	for (int32 i = 0; i < AvailableResolutions.Num(); i++)
	{
		if (AvailableResolutions[i] == Resolution)
		{
			return i;
		}
	}
	return 0;
}

void UVideoSettingsWidget::OnUncappedFramerateCheckBoxChanged(bool bIsOn)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	if (bIsOn)
	{
		FramerateSlider->SetVisibility(ESlateVisibility::HitTestInvisible);
		PlayerSettings->SetFrameRateLimit(0.0f);
	}
	else
	{
		FramerateSlider->SetVisibility(ESlateVisibility::Visible);
		FramerateSlider->SetValue(120.0f);
	}
	
}
void UVideoSettingsWidget::OnFramerateSliderValueChanged(float Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings || UncappedFramerateCheckBox->IsOn()) return;
	
	PlayerSettings->SetFrameRateLimit(Value);
}

void UVideoSettingsWidget::OnVSyncCheckBoxChanged(bool bIsOn)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetVSyncEnabled(bIsOn);
}

void UVideoSettingsWidget::OnFoliageValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetFoliageQuality(Value);
}
void UVideoSettingsWidget::OnAntiAliasingValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetAntiAliasingQuality(Value);
}
void UVideoSettingsWidget::OnReflectionValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetReflectionQuality(Value);
}
void UVideoSettingsWidget::OnShadingValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetShadingQuality(Value);
}
void UVideoSettingsWidget::OnTextureValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetTextureQuality(Value);
}
void UVideoSettingsWidget::OnGlobalIlluminationValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetGlobalIlluminationQuality(Value);
}
void UVideoSettingsWidget::OnPostProcessingValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetPostProcessingQuality(Value);
}
void UVideoSettingsWidget::OnShadowQualityValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetShadowQuality(Value);
}
void UVideoSettingsWidget::OnVisualEffectValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetVisualEffectQuality(Value);
}