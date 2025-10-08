// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "UIStackManager.generated.h"

struct FObjectReference;
class UCommonActivatableWidgetPage;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UUIStackManager : public UCommonActivatableWidgetStack
{
	GENERATED_BODY()

public:
	UFUNCTION(Blueprintable, BlueprintCallable)
	void PushPage(UCommonActivatableWidgetPage* NewPage);

	UFUNCTION(Blueprintable, BlueprintCallable)
	void PopPage();

	//UPROPERTY()
	//FObjectReference<UCommonActivatableWidgetPage> TestRootPage1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCommonActivatableWidgetPage* RootPage;
protected:
	
	TArray<UCommonActivatableWidgetPage*> UIPageStack;

	
};
