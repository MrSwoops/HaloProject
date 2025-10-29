#pragma once
#include "CoreMinimal.h"
//#include "CommonTextBlock.h"
#include "CommonButtonBase.h"
#include "MyProject/UI/Common/CommonActivatableWidgetPage.h"
#include "CustomGamesPage.generated.h"

class UCommonTextBlock;
//class UCommonButtonBase;

UCLASS()
class MYPROJECT_API UCustomGamesPage : public UCommonActivatableWidgetPage
{
	GENERATED_BODY()
public:
	UCustomGamesPage();

	FString GameModeName = "TeamSlayer";
	FString MapName = "Foundry";

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	// UCommonTextBlock* TimeText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonButtonBase* StartGameButton;

protected:
	UFUNCTION()
	void StartGame();

	virtual void NativeConstruct() override;
	
};
