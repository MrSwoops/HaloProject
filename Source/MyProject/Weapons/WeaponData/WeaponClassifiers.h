#pragma once


UENUM()
enum class EWeaponType : uint8
{
	None,
	Rifle,
	Pistol,
	Launcher,
	Sniper,
	SMG,
};

UENUM(BlueprintType)
enum class EWeaponStorageSlot : uint8
{
	backGunStorageSocket UMETA(DisplayName = "Back"),
	thigh_rGunStorageSocket UMETA(DisplayName = "Right Thigh")
};



