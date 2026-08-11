#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CommonValueCarousel.generated.h"

class UCommonButtonBase;
class UCommonTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCarouselValueChanged, int32, NewIndex);

UCLASS()
class MYPROJECT_API UCommonValueCarousel : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

protected:

	UPROPERTY(meta = (BindWidget))
	UCommonButtonBase* LeftButton;
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* ValueText;
	UPROPERTY(meta = (BindWidget))
	UCommonButtonBase* RightButton;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	bool bLoopOptions = true;

private:
	bool bIsHovered = false;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	TArray<FText> Options;

	UPROPERTY(BlueprintAssignable)
	FOnCarouselValueChanged OnSettingChanged;

	void NextOption();
	void PreviousOption();
	void UpdateDisplay();
	UFUNCTION(BlueprintCallable)
	void SetOption(const int32 Index);

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
