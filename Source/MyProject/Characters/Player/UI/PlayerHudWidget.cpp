// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHudWidget.h"

#include "MyProject/Characters/Player/PlayerCharacter.h"

void UPlayerHudWidget::InitializeConnections(APlayerCharacter* InPlayerCharacter)
{
	if (!InPlayerCharacter) return;

	InPlayerCharacter->OnReceiveDamage.AddDynamic(this, &UPlayerHudWidget::UpdateHealthBar);
	InPlayerCharacter->UpdateInteractions.AddDynamic(this, &UPlayerHudWidget::UpdateInteractionText);
	InPlayerCharacter->UpdateGrenadesUI.AddDynamic(this, &UPlayerHudWidget::UpdateGrenadeUI);
}

void UPlayerHudWidget::UpdateHealthBar(const float& CurrentShields, const float& MaxShields, const int32& DamageTaken)
{
	HealthProgress->SetPercent(CurrentShields / MaxShields);
}
void UPlayerHudWidget::UpdateInteractionText(bool IsActive, const FString& InString)
{
	InteractText->SetText((IsActive) ? FText::FromString(InString) : FText::FromString(""));
}
void UPlayerHudWidget::UpdateGrenadeUI(const int32& CurrentGrenadeSelection, const int32& RegularGrenadesCount, const int32& PlasmaGrenadesCount)
{
	GrenadesText->SetText(FText::AsNumber(RegularGrenadesCount));
	PGrenadesText->SetText(FText::AsNumber(PlasmaGrenadesCount));
}
