// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "BurstWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ABurstWeapon : public AWeapon
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	float BurstRate = 0.1f;

	UPROPERTY(EditDefaultsOnly)
	int32 BurstShots = 3.0f;

	virtual void Fire() override;

protected:
	virtual void ShootBullet() override;

private:
	int32 CurrentBurst = 0;
};
