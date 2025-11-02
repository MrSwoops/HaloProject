#pragma once
#include "CommonButtonBase.h"
#include "MyProject/UI/Common/CommonActivatableWidgetPage.h"
#include "CampaignSelectPage.generated.h"

enum class EDifficulty : uint8;

UCLASS(Blueprintable, BlueprintType)
class UCampaignSelectPage : public UCommonActivatableWidgetPage
{
	GENERATED_BODY()
public:
	UCampaignSelectPage();

	EDifficulty Difficulty;
	FString LevelName = "ODSTLevel1";

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	// UCommonTextBlock* TimeText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonButtonBase* StartGameButton;

protected:
	UFUNCTION()
	void StartGame();

	virtual void NativeConstruct() override;
	
};