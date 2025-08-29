// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "AutomaticWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AAutomaticWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	virtual void FireHeld() override;
	
};
