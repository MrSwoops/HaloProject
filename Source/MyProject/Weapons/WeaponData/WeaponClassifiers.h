#pragma once


UENUM()
enum class EWeaponAmmoUsageType : uint8
{
	Bullets,
	Energy,
};

UENUM()
enum class EWeaponFireType : uint8
{
	Single,
	SemiAuto,
	Automatic,
	Burst,
	Charge,
	Pellets
};


struct FWeaponClassifiers
{
public:
	bool TwoHanded;
	bool PlayerOwned;
	EWeaponFireType WeaponFireType;
	EWeaponAmmoUsageType WeaponAmmoUsageType;
	
};


