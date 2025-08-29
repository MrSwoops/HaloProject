// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventory.h"

WeaponInventory::WeaponInventory()
{
}

WeaponInventory::~WeaponInventory()
{
}

bool WeaponInventory::PickUpWeapon(UMyProjectWeaponComponent* Weapon)
{
	if (PrimaryWeapon != nullptr) // Primary Occupied
	{
		if (SecondaryWeapon != nullptr) // Secondary Occupied
		{
			
		}
		else
		{
			SecondaryWeapon = Weapon;
		}
	}
	else
	{
		PrimaryWeapon = Weapon;
	}
	return true;
}
