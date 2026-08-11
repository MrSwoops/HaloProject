// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonActivatableWidgetPage.generated.h"

/**
 * 
 */

class UCommonButtonBase;
class UCommonStackManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHandleBackAction);
UCLASS()
class MYPROJECT_API UCommonActivatableWidgetPage : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Widgets")
	void InitializePage(UCommonStackManager* InStackManager);

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	// UCommonButtonBase* BackButton;
};
