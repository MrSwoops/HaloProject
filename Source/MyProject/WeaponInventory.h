// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



class UMyProjectWeaponComponent;
/**
 * 
 */
class MYPROJECT_API WeaponInventory
{
public:
	WeaponInventory();
	~WeaponInventory();

	UFUNCTION()
	bool PickUpWeapon(UMyProjectWeaponComponent* Weapon);

	UMyProjectWeaponComponent* PrimaryWeapon;
	UMyProjectWeaponComponent* SecondaryWeapon;
};
