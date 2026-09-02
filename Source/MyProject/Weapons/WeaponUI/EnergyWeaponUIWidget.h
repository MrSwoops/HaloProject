// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponUIWidget.h"
#include "EnergyWeaponUIWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UEnergyWeaponUIWidget : public UWeaponUIWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HeatLevelSlider;

	virtual void UpdateAmmoUI(const float& CurrentMagAmmo) override;

	virtual void InitializeWeaponUI(const float& CurrentMagAmmo, const float& CurrentReserveAmmo, UWeaponUIData* UIData) override;
	
};
