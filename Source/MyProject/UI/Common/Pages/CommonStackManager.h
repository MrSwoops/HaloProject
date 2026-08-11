#pragma once

#include "CommonActivatableWidgetSwitcher.h"
#include "CommonStackManager.generated.h"

UCLASS()
class UCommonStackManager : public UCommonActivatableWidgetSwitcher
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void PushPage(const int32& PageIndex);
	UFUNCTION(BlueprintCallable)
	void PopPage();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CommonStackManager|RootPageBehavior")
	int32  RootPage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CommonStackManager|RootPageBehavior")
	bool bPushBasePageOnStart = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CommonStackManager|RootPageBehavior")
	bool bBasePageUnPoppable = false;
private:
	TArray<int32> PageIndexStack;
};
