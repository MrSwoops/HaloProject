#pragma once

#include "CommonActivatableWidget.h"
#include "VideoSettingsWidget.generated.h"

class USlider;
class UCommonCheckBox;
class UCommonValueCarousel;

UCLASS()
class UVideoSettingsWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
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

protected:
	virtual void NativeOnActivated() override;
	
private:	
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
};
