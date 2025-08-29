// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AShotgun : public AWeapon
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	int32 PelletsPerShot = 15;

protected:
	virtual void ShootBullet() override;

private:
	
};
