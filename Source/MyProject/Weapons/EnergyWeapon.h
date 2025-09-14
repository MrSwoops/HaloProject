// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "EnergyWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AEnergyWeapon : public AWeapon
{
	GENERATED_BODY()

#pragma region Heating
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float MaxHeatLevel = 50.0f;
	UPROPERTY()
	float CurrentHeatLevel = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float HeatBuildupPerShot = 7.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float HeatDissipationSpeed = 5.0f;

	UPROPERTY()
	bool IsOverHeated = false;
public:
	void SetOverheated();

#pragma endregion Heating
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void Fire() override;

	virtual void ShootBullet() override;

	virtual void Reload() override;
	virtual void AttachWeapon(AGameplayCharacter* TargetCharacter) override;
	
	
};
