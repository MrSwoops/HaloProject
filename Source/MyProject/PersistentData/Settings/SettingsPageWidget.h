#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonButtonBase.h"
#include "Components/Slider.h"
#include "MyProject/UI/Common/CommonActivatableWidgetPage.h"
#include "SettingsPageWidget.generated.h"

class UCommonValueCarousel;
class UCommonCheckBox;

UCLASS()
class USettingsPageWidget : public UCommonActivatableWidgetPage
{
	GENERATED_BODY()
public:
	USettingsPageWidget();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonButtonBase* BackButton;
	
#pragma region AUDIO
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USlider* MasterSlider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USlider* MusicSlider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USlider* SFXSlider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USlider* VoiceSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonCheckBox* SubtitlesCheckBox;

private:
	void InitializeAudio();
	
	UFUNCTION()
	void OnMasterSliderValueChanged(float Value);
	UFUNCTION()
	void OnMusicSliderValueChanged(float Value);
	UFUNCTION()
	void OnSFXSliderValueChanged(float Value);
	UFUNCTION()
	void OnVoiceSliderValueChanged(float Value);
	UFUNCTION()
	void OnSubtitlesCheckBoxChanged(bool bIsOn);
#pragma endregion AUDIO

#pragma region GRAPHICS
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonValueCarousel* FullscreenValueCarousel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonValueCarousel* ResolutionValueCarousel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonCheckBox* UncappedFramerateCheckBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USlider* FramerateSlider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonCheckBox* VSyncCheckBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonValueCarousel* ShadowQualityValueCarousel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonValueCarousel* GlobalIlluminationValueCarousel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonValueCarousel* ReflectionValueCarousel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonValueCarousel* AntiAliasingValueCarousel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonValueCarousel* TextureValueCarousel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonValueCarousel* VisualEffectValueCarousel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonValueCarousel* PostProcessingValueCarousel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonValueCarousel* FoliageValueCarousel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonValueCarousel* ShadingValueCarousel;
	
	
private:
	void InitializeGraphics();

	UFUNCTION()
	void OnFullscreenValueChanged(int32 Value);
	static EWindowMode::Type FullscreenMode(int32 Value);
	static int32 FullscreenValue(EWindowMode::Type Mode);
	
	UFUNCTION()
	void OnResolutionValueChanged(int32 Value);
	void InitializeResolution();
	TArray<FIntPoint> AvailableResolutions;
	int32 GetResolutionIndex(FIntPoint Resolution);

	UFUNCTION()
	void OnUncappedFramerateCheckBoxChanged(bool bIsOn);
	UFUNCTION()
	void OnFramerateSliderValueChanged(float Value);
	UFUNCTION()
	void OnVSyncCheckBoxChanged(bool bIsOn);

	UFUNCTION()
	void OnShadowQualityValueChanged(int32 Value);
	UFUNCTION()
	void OnGlobalIlluminationValueChanged(int32 Value);
	UFUNCTION()
	void OnReflectionValueChanged(int32 Value);
	UFUNCTION()
	void OnAntiAliasingValueChanged(int32 Value);
	UFUNCTION()
	void OnTextureValueChanged(int32 Value);
	UFUNCTION()
	void OnVisualEffectValueChanged(int32 Value);
	UFUNCTION()
	void OnPostProcessingValueChanged(int32 Value);
	UFUNCTION()
	void OnFoliageValueChanged(int32 Value);
	UFUNCTION()
	void OnShadingValueChanged(int32 Value);
	
#pragma endregion GRAPHICS
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnDeactivated() override;
};
