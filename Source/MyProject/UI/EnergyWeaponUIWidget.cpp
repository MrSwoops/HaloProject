// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyWeaponUIWidget.h"

#include "Components/ProgressBar.h"

void UEnergyWeaponUIWidget::UpdateAmmoUI(const float& CurrentMagAmmo)
{
	HeatLevelSlider->SetPercent(CurrentMagAmmo);
}

void UEnergyWeaponUIWidget::InitializeWeaponUI(const float& CurrentMagAmmo, const float& CurrentReserveAmmo, UWeaponUIData* UIData)
{
	AddToPlayerScreen();
	UpdateAmmoUI(CurrentMagAmmo);
	SetReserveText(CurrentReserveAmmo);
}

