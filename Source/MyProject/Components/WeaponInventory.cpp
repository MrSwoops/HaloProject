// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventory.h"

#include <FMODBlueprintStatics.h>

#include "../GameModes/BaseGameMode.h"
#include "../Weapons/Grenade.h"
#include "Kismet/GameplayStatics.h"
#include "BulletPoolManager.h"
#include "MyProjectPickUpComponent.h"
#include "../PlayerCharacter.h"
#include "../Weapons/Weapon.h"
#include "MyProject/Equipment.h"

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
	
	InitializeInventory();
	
}
void UWeaponInventory::InitializeInventory()
{
	if (UseOverrides) InitializeInventoryEquipment(DefaultInventory);
	else
	{
		Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->InitializeCharacterInventory(this);
	}
}

void UWeaponInventory::InitializeInventoryEquipment(FInventoryDefault& Inventory)
{
	AGameplayCharacter* Owner = Cast<AGameplayCharacter>(GetOwner());
	if (!Owner) return;
	if (Inventory.StartingSecondary)
	{
		AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(Inventory.StartingSecondary);
		Weapon->AttachWeapon(Owner);
	} 
	if (Inventory.StartingPrimary)
	{
		AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(Inventory.StartingPrimary);
		Weapon->AttachWeapon(Owner);
	}
	if (Inventory.StartingEquipment)
	{
	
	}
	RegularGrenades = Inventory.StartingFragGrenades;
	PlasmaGrenades = Inventory.StartingPlasmaGrenades;
}


void UWeaponInventory::InitializeInventoryEquipment(TSubclassOf<AWeapon>& InStartingPrimary, TSubclassOf<AWeapon>& InStartingSecondary, int32& InStartingFragGrenades, int32& InStartingPlasmaGrenades, TSubclassOf<AEquipment>& InStartingEquipment)
{
	AGameplayCharacter* Owner = Cast<AGameplayCharacter>(GetOwner());
	if (!Owner) return;
	if (InStartingSecondary)
	{
		AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(InStartingSecondary);
		Weapon->AttachWeapon(Owner);
	} 
	if (InStartingPrimary)
	{
		AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(InStartingPrimary);
		Weapon->AttachWeapon(Owner);
	}
	if (InStartingEquipment)
	{
	
	}
	RegularGrenades = InStartingFragGrenades;
	PlasmaGrenades = InStartingPlasmaGrenades;
}


void UWeaponInventory::PickUpEquipment(AEquipment* Equipment)
{
	
}


void UWeaponInventory::PickUpWeapon(UEnhancedInputComponent* EnhancedInputComponent, AWeapon* Weapon)
{
	if (PrimaryWeapon)
	{
		if (SecondaryWeapon)
		{
			//Drop current weapon and pick up new weapon
			CurrentWeapon->UnbindActions(EnhancedInputComponent);
			AWeapon* OldWeapon = CurrentWeapon;
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
			OldWeapon->DropWeapon();
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
			AWeapon* OldWeapon = CurrentWeapon;
			if (CurrentWeapon == PrimaryWeapon)
			{
				PrimaryWeapon = Weapon;
				CurrentWeapon = Weapon;
			} else
			{
				SecondaryWeapon = Weapon;
				CurrentWeapon = Weapon;
			}
			OldWeapon->DropWeapon();
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
		OutGrenade = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->BulletPoolManager->SpawnGrenade(SpawnLocation, SpawnRotation, CurrentGrenade);
		if (OutGrenade)
		{
			RegularGrenades--;
			return true;
		}
		break;
	case 1: // Plasma
		if (PlasmaGrenades == 0) return false;
		OutGrenade = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->BulletPoolManager->SpawnGrenade(SpawnLocation, SpawnRotation, CurrentGrenade);
		if (OutGrenade)
		{
			PlasmaGrenades--;
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}

#pragma endregion Grenades

bool UWeaponInventory::ScavageWeapon(AWeapon* Weapon)
{
	if (Weapon->IsSameWeaponType(PrimaryWeapon))
	{
		//return FillWeapon(Weapon, PrimaryWeapon);
	}
	else
	{
		//return FillWeapon(Weapon, SecondaryWeapon);
	}
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
		if (UAnimInstance* AnimInstance = Cast<APlayerCharacter>(CurrentWeapon->Character)->GetMesh1P()->GetAnimInstance()) // Get the animation object for the arms mesh
		{
			AnimInstance->StopAllMontages(0.0f);
		}
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

void UWeaponInventory::DropInventory()
{
	if (SecondaryWeapon)
	{
		SecondaryWeapon->EnableWeapon();
		SecondaryWeapon->DropWeapon();
		SecondaryWeapon = nullptr;
	} 
	if (PrimaryWeapon)
	{
		PrimaryWeapon->EnableWeapon();
		PrimaryWeapon->DropWeapon();
		PrimaryWeapon = nullptr;
	}
	CurrentWeapon = nullptr;
	for (int32 i = 0; i < RegularGrenades; i++)
	{
		const FRotator SpawnRotation = GetOwner()->GetActorRotation();
		const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(FVector(100.0f, 0.0f, 10.0f));
		AGrenade* OutNade = nullptr;
		if (TryGetGrenade(SpawnLocation, SpawnRotation, OutNade))
		{
			OutNade->PickUpComponent->Enabled = true;
		}
	}
	for (int32 i = 0; i < PlasmaGrenades; i++)
	{
		
	}
}
