#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "InteractionUI.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class UInteractionUI : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonTextBlock* InteractionText;

	void UpdateInteractionText(const FString& Text);
	void ClearInteractionText();

	
};