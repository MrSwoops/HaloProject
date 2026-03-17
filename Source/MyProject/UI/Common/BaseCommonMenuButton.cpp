// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCommonMenuButton.h"

void UBaseCommonMenuButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	ButtonText->SetText(Text);
}

void UBaseCommonMenuButton::SetText(const FText& NewText)
{
	Text = NewText;
	ButtonText->SetText(Text);
}
