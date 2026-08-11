// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonActivatableWidgetPage.h"

#include "CommonButtonBase.h"
#include "CommonStackManager.h"

void UCommonActivatableWidgetPage::InitializePage_Implementation(UCommonStackManager* InStackManager)
{
	if (!InStackManager) { UE_LOG(LogTemp, Error, TEXT("InStackManager is NULL in UCommonActivatableWidgetPage::InitializePage_Implementation")); return; }
	 
}
