// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventory.h"

#include "CustomGameMode.h"
#include "Weapons/Grenade.h"
#include "Kismet/GameplayStatics.h"
#include "BulletPoolManager.h"
#include "Weapons/Weapon.h"

// Sets default values for this component's properties
UWeaponInventory::UWeaponInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UWeaponInventory::PickUpWeapon(UEnhancedInputComponent* EnhancedInputComponent, AWeapon* Weapon)
{
	if (PrimaryWeapon)
	{
		if (SecondaryWeapon)
		{
			//Drop current weapon and pick up new weapon
			CurrentWeapon->UnbindActions(EnhancedInputComponent);
			CurrentWeapon->DropWeapon();
			if (CurrentWeapon == PrimaryWeapon)
			{
				PrimaryWeapon = Weapon;
				CurrentWeapon = Weapon;
				CurrentWeapon->BindActions(EnhancedInputComponent);
			} else
			{
				SecondaryWeapon = Weapon;
				CurrentWeapon = Weapon;
				CurrentWeapon->BindActions(EnhancedInputComponent);
			}
		}
		else // Add to secondary and swap
		{
			SecondaryWeapon = Weapon;
			SwapWeapons(EnhancedInputComponent);
		}
	}
	else
	{
		PrimaryWeapon = Weapon;
		CurrentWeapon = Weapon;
		CurrentWeapon->BindActions(EnhancedInputComponent);
	}
}

void UWeaponInventory::PickUpWeapon(AWeapon* Weapon)
{
	if (PrimaryWeapon)
	{
		if (SecondaryWeapon)
		{
			//Drop current weapon and pick up new weapon
			CurrentWeapon->DropWeapon();
			if (CurrentWeapon == PrimaryWeapon)
			{
				PrimaryWeapon = Weapon;
				CurrentWeapon = Weapon;
			} else
			{
				SecondaryWeapon = Weapon;
				CurrentWeapon = Weapon;
			}
		}
		else // Add to secondary and swap
		{
			SecondaryWeapon = Weapon;
			SwapWeapons();
		}
	}
	else
	{
		PrimaryWeapon = Weapon;
		CurrentWeapon = Weapon;
	}
}

#pragma region Grenades

bool UWeaponInventory::PickUpGrenade(AGrenade* Grenade)
{
	if (Grenade->Sticky)
	{
		if (PlasmaGrenades >= 4) return false;
		PlasmaGrenades++;
	}
	else
	{
		if (RegularGrenades >= 4) return false;
		RegularGrenades++;
	}
	Grenade->ReturnToPool();
	return true;
}

void UWeaponInventory::SwapGrenades()
{
	if (RegularGrenades > 0 && PlasmaGrenades > 0)
	{
		CurrentGrenade = (CurrentGrenade == 0) ? 1 : 0; // 0 = Regular, 1 = Plasma
	}
	else
	{
		CurrentGrenade = -1; // No Grenades
	}
}

bool UWeaponInventory::TryGetGrenade(const FVector& SpawnLocation, const FRotator& SpawnRotation, AGrenade*& OutGrenade)
{
	switch (CurrentGrenade) {
	case 0: // Regular
		if (RegularGrenades == 0) return false;
		OutGrenade = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->BulletPoolManager->SpawnGrenade(SpawnLocation, SpawnRotation, CurrentGrenade);
		if (OutGrenade)
		{
			RegularGrenades--;
			return true;
		}
		break;
	case 1: // Plasma
		if (PlasmaGrenades == 0) return false;
		OutGrenade = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->BulletPoolManager->SpawnGrenade(SpawnLocation, SpawnRotation, CurrentGrenade);
		if (OutGrenade)
		{
			PlasmaGrenades--;
			return true;
		}
		break;
	default:
		return false;
	}
	return true;
}

#pragma endregion Grenades

bool UWeaponInventory::ScavengeWeapon(AWeapon* Weapon)
{
	int32 AmmoNeeded;
	if (Weapon->WeaponModel == PrimaryWeapon->WeaponModel)
	{
		AmmoNeeded = PrimaryWeapon->MaxReserveMags * PrimaryWeapon->MaxMagSize - PrimaryWeapon->CurrentReserveAmmo;
		if (Weapon->CurrentReserveAmmo < AmmoNeeded) AmmoNeeded = Weapon->CurrentReserveAmmo;
		if (AmmoNeeded > 0)
		{
			PrimaryWeapon->CurrentReserveAmmo += AmmoNeeded;
			PrimaryWeapon->UpdateReserveAmmoUI();
		}
	}
	else
	{
		AmmoNeeded = SecondaryWeapon->MaxReserveMags * SecondaryWeapon->MaxMagSize - SecondaryWeapon->CurrentReserveAmmo;
		if (Weapon->CurrentReserveAmmo < AmmoNeeded) AmmoNeeded = Weapon->CurrentReserveAmmo;
		if (AmmoNeeded > 0)
		{
			SecondaryWeapon->CurrentReserveAmmo += AmmoNeeded;
			SecondaryWeapon->UpdateReserveAmmoUI();
		}
	}
	Weapon->CurrentReserveAmmo -= AmmoNeeded;
	if (Weapon->CurrentReserveAmmo <= 0) return true;
	return false;
}

bool UWeaponInventory::TryMeleeWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Melee();
		return true;
	}
	return false;
}

void UWeaponInventory::TryReloadWeapon()
{
	if (CurrentWeapon) CurrentWeapon->Reload();
}

void UWeaponInventory::SwapWeapons()
{
	if (PrimaryWeapon && SecondaryWeapon)
	{
		CurrentWeapon->DisableWeapon();
		if (CurrentWeapon == PrimaryWeapon)
		{
			if (SecondaryWeapon == nullptr) return; // No secondary to swap to
			CurrentWeapon = SecondaryWeapon;
		}
		else
		{
			if (PrimaryWeapon == nullptr) return; // No primary to swap to
			CurrentWeapon = PrimaryWeapon;
		}
		CurrentWeapon->EnableWeapon();
	}
}

void UWeaponInventory::SwapWeapons(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (PrimaryWeapon && SecondaryWeapon)
	{
		CurrentWeapon->DisableWeapon();
		CurrentWeapon->UnbindActions(EnhancedInputComponent);
		
		if (CurrentWeapon == PrimaryWeapon)
		{
			if (SecondaryWeapon == nullptr) return; // No secondary to swap to
			CurrentWeapon = SecondaryWeapon;
		}
		else
		{
			if (PrimaryWeapon == nullptr) return; // No primary to swap to
			CurrentWeapon = PrimaryWeapon;
		}
		CurrentWeapon->EnableWeapon();
		CurrentWeapon->BindActions(EnhancedInputComponent);
	}
}
