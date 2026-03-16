#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "CommonCheckBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToggleChanged, bool, bIsOn);

UCLASS()
class MYPROJECT_API UCommonCheckBox : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UCommonCheckBox();

protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnClicked() override;

	void UpdateVisualState();

protected:
	UPROPERTY(EditAnywhere)
	FString Label = "Default Label";
	UPROPERTY(meta = (BindWidget))
	UCommonLazyImage* CheckboxBackground;
	UPROPERTY(meta = (BindWidget))
	UCommonLazyImage* CheckmarkImage;
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* LabelText;

	UPROPERTY(BlueprintReadOnly, Category="Toggle")
	bool bIsOn = false;

public:

	UFUNCTION(BlueprintCallable)
	void SetIsOn(const bool bIsSelected);
	UFUNCTION(BlueprintPure)
	bool IsOn() const;
	UFUNCTION(BlueprintCallable)
	void SetLabel(const FText& NewLabel);

public:

	UPROPERTY(BlueprintAssignable, Category="Toggle")
	FOnToggleChanged OnToggleChanged;
};