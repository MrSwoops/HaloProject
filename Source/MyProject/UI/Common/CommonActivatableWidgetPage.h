// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonActivatableWidgetPage.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHandleBackAction);
UCLASS()
class MYPROJECT_API UCommonActivatableWidgetPage : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(Blueprintable, BlueprintCallable)
	void ShowPage();

	UPROPERTY(BlueprintAssignable)
	FHandleBackAction HandleBackAction;

	UFUNCTION(Blueprintable, BlueprintCallable)
	void HidePage();

	virtual void NativeOnActivated() override;

	UFUNCTION(BlueprintImplementableEvent, Category = ActivatableWidget, meta = (DisplayName = "On Handle Back Action Real"))
	bool BP_OnHandleBackActionReal();
	virtual bool NativeOnHandleBackAction() override;
	
};
