#include "SettingsPageWidget.h"

#include "PlayerSettingsFile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyProject/UI/Common/CommonCheckBox.h"
#include "MyProject/UI/Common/CommonValueCarousel.h"


USettingsPageWidget::USettingsPageWidget()
{
	
}

void USettingsPageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeAudio();
	InitializeGraphics();
}

void USettingsPageWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;

	PlayerSettings->ApplySettings(false);
}

#pragma region AUDIO
void USettingsPageWidget::InitializeAudio()
{
	MasterSlider->SetMinValue(0.0f);
	MasterSlider->SetMaxValue(10.0f);
	MusicSlider->SetMinValue(0.0f);
	MusicSlider->SetMaxValue(10.0f);
	SFXSlider->SetMinValue(0.0f);
	SFXSlider->SetMaxValue(10.0f);
	VoiceSlider->SetMinValue(0.0f);
	VoiceSlider->SetMaxValue(10.0f);

	if (auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings())
	{
		MasterSlider->SetValue(PlayerSettings->GetMasterVolume());
		MusicSlider->SetValue(PlayerSettings->GetMusicVolume());
		SFXSlider->SetValue(PlayerSettings->GetSFXVolume());
		VoiceSlider->SetValue(PlayerSettings->GetVoiceVolume());
		SubtitlesCheckBox->SetIsOn(PlayerSettings->GetSubtitlesEnabled());
	}

	MasterSlider->OnValueChanged.AddDynamic(this, &USettingsPageWidget::OnMasterSliderValueChanged);
	MusicSlider->OnValueChanged.AddDynamic(this, &USettingsPageWidget::OnMusicSliderValueChanged);
	SFXSlider->OnValueChanged.AddDynamic(this, &USettingsPageWidget::OnSFXSliderValueChanged);
	VoiceSlider->OnValueChanged.AddDynamic(this, &USettingsPageWidget::OnVoiceSliderValueChanged);
	SubtitlesCheckBox->OnToggleChanged.AddDynamic(this, &USettingsPageWidget::OnSubtitlesCheckBoxChanged);
}

void USettingsPageWidget::OnMasterSliderValueChanged(float Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetMasterVolume(Value);
}
void USettingsPageWidget::OnMusicSliderValueChanged(float Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetMusicVolume(Value);
}
void USettingsPageWidget::OnSFXSliderValueChanged(float Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetSFXVolume(Value);
}
void USettingsPageWidget::OnVoiceSliderValueChanged(float Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetVoiceVolume(Value);
}
void USettingsPageWidget::OnSubtitlesCheckBoxChanged(bool bIsOn)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetSubtitlesEnabled(bIsOn);
}

#pragma endregion AUDIO

#pragma region GRAPHICS
void USettingsPageWidget::InitializeGraphics()
{
	InitializeResolution();
	UncappedFramerateCheckBox->OnToggleChanged.AddDynamic(this, &USettingsPageWidget::OnUncappedFramerateCheckBoxChanged);
	
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
	
	FullscreenValueCarousel->OnSettingChanged.AddDynamic(this, &USettingsPageWidget::OnFullscreenValueChanged);
	FramerateSlider->OnValueChanged.AddDynamic(this, &USettingsPageWidget::OnFramerateSliderValueChanged);
	VSyncCheckBox->OnToggleChanged.AddDynamic(this, &USettingsPageWidget::OnVSyncCheckBoxChanged);
	
	ShadowQualityValueCarousel->OnSettingChanged.AddDynamic(this, &USettingsPageWidget::OnShadowQualityValueChanged);
	GlobalIlluminationValueCarousel->OnSettingChanged.AddDynamic(this, &USettingsPageWidget::OnGlobalIlluminationValueChanged);
	ReflectionValueCarousel->OnSettingChanged.AddDynamic(this, &USettingsPageWidget::OnReflectionValueChanged);
	AntiAliasingValueCarousel->OnSettingChanged.AddDynamic(this, &USettingsPageWidget::OnAntiAliasingValueChanged);
	TextureValueCarousel->OnSettingChanged.AddDynamic(this, &USettingsPageWidget::OnTextureValueChanged);
	VisualEffectValueCarousel->OnSettingChanged.AddDynamic(this, &USettingsPageWidget::OnVisualEffectValueChanged);
	PostProcessingValueCarousel->OnSettingChanged.AddDynamic(this, &USettingsPageWidget::OnPostProcessingValueChanged);
	FoliageValueCarousel->OnSettingChanged.AddDynamic(this, &USettingsPageWidget::OnFoliageValueChanged);
	ShadingValueCarousel->OnSettingChanged.AddDynamic(this, &USettingsPageWidget::OnShadingValueChanged);
}

void USettingsPageWidget::OnFullscreenValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetFullscreenMode(FullscreenMode(Value));
	
}
EWindowMode::Type USettingsPageWidget::FullscreenMode(const int32 Value)
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
int32 USettingsPageWidget::FullscreenValue(EWindowMode::Type Mode)
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

void USettingsPageWidget::OnResolutionValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetScreenResolution(AvailableResolutions[Value]);
}
void USettingsPageWidget::InitializeResolution()
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
	
	ResolutionValueCarousel->OnSettingChanged.AddDynamic(this, &USettingsPageWidget::OnResolutionValueChanged);
}
int32 USettingsPageWidget::GetResolutionIndex(FIntPoint Resolution)
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

void USettingsPageWidget::OnUncappedFramerateCheckBoxChanged(bool bIsOn)
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
void USettingsPageWidget::OnFramerateSliderValueChanged(float Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings || UncappedFramerateCheckBox->IsOn()) return;
	
	PlayerSettings->SetFrameRateLimit(Value);
}

void USettingsPageWidget::OnVSyncCheckBoxChanged(bool bIsOn)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetVSyncEnabled(bIsOn);
}

void USettingsPageWidget::OnFoliageValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetFoliageQuality(Value);
}
void USettingsPageWidget::OnAntiAliasingValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetAntiAliasingQuality(Value);
}
void USettingsPageWidget::OnReflectionValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetReflectionQuality(Value);
}
void USettingsPageWidget::OnShadingValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetShadingQuality(Value);
}
void USettingsPageWidget::OnTextureValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetTextureQuality(Value);
}
void USettingsPageWidget::OnGlobalIlluminationValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetGlobalIlluminationQuality(Value);
}
void USettingsPageWidget::OnPostProcessingValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetPostProcessingQuality(Value);
}
void USettingsPageWidget::OnShadowQualityValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetShadowQuality(Value);
}
void USettingsPageWidget::OnVisualEffectValueChanged(int32 Value)
{
	auto* PlayerSettings = UPlayerSettingsFile::GetPlayerSettings();
	if (!PlayerSettings) return;
	PlayerSettings->SetVisualEffectQuality(Value);
}

#pragma endregion GRAPHICS