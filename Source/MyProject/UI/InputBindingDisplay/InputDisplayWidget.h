#pragma once

#include "CoreMinimal.h"
#include "ActionInputDisplayMapData.h"
#include "CommonUserWidget.h"
#include "InputDisplayWidget.generated.h"

class UInputAction;
class UInputMappingContext;
class UCommonTextBlock;
class UCommonLazyImage;
enum class ECommonInputType : uint8;
class AItem;
class UInputActionIconMappingData;
class UInputDisplayWidget;
class UPanelWidget;

/**
 * 
 */
UCLASS()
class UInputDisplayWidget : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonLazyImage* InputImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonTextBlock* DisplayText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UActionInputDisplayMapData* InputMapData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* Context;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DescriptionText;
	
	void InitializeDisplay();
	bool RetrieveKey(FKey& OutKey);
	bool PlayerIsUsingGamepad() const;

	static bool IsGamepadKey(const FKey& Key);
	static bool IsKeyboardKey(const FKey& Key);
	static bool IsMouseKey(const FKey& Key);

protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
};
