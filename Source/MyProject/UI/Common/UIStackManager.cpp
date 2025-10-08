// Fill out your copyright notice in the Description page of Project Settings.


#include "UIStackManager.h"

#include "CommonActivatableWidgetPage.h"

void UUIStackManager::PushPage(UCommonActivatableWidgetPage* NewPage)
{
	if (UIPageStack.Num() > 0)
	{
		UCommonActivatableWidgetPage* PreviousPage = UIPageStack.Last();
		PreviousPage->HidePage();
	}
	
	UIPageStack.Push(NewPage);
	NewPage->ShowPage();
}

void UUIStackManager::PopPage()
{
	if (UIPageStack.Num() <= 0 || (RootPage != nullptr && UIPageStack.Num() <= 1)) return;
	
	UCommonActivatableWidgetPage* PoppedPage = UIPageStack.Pop();
	PoppedPage->HidePage();

	if (UIPageStack.Num() <= 0)
		return;

	if (UCommonActivatableWidgetPage* NewPage = UIPageStack.Last())
		NewPage->ShowPage();
}
