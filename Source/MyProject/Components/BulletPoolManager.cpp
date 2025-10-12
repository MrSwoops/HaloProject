// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletPoolManager.h"

#include "ActorPool.h"
#include "MyProject/Weapons/WeaponProjectiles/Bullet.h"
#include "MyProject/Weapons/Weapon.h"
#include "MyProject/Weapons/Grenade.h"
#include "MyProject/Weapons/WeaponData/ProjectileData.h"

// Sets default values for this component's properties
UBulletPoolManager::UBulletPoolManager()
{
	BulletPool = CreateDefaultSubobject<UActorPool>(TEXT("Default Bullet Pool"));
	GrenadePool = CreateDefaultSubobject<UActorPool>(TEXT("Grenade Pool"));
	RocketPool = CreateDefaultSubobject<UActorPool>(TEXT("Rocket Pool"));
}

AGrenade* UBulletPoolManager::SpawnGrenade(const FVector& Location, const FRotator& Rotation, const int32 NadeType)
{
	switch (NadeType) {
	case 0: // Regular
		return Cast<AGrenade>(GrenadePool->SpawnFromLocationAndRotation(Location, Rotation, false));
		break;
	case 1: // Plasma
		
		break;
	default:
		break;
	}
	return nullptr;
}

AWeaponProjectile* UBulletPoolManager::SpawnBullet(const FVector& Location, const FRotator& Rotation, UProjectileData* ProjectileData)
{
	AWeaponProjectile* OutBullet = nullptr;
	if (auto* BulletData = Cast<UBulletData>(ProjectileData))
	{
		OutBullet = Cast<AWeaponProjectile>(BulletPool->SpawnFromLocationAndRotation(Location, Rotation, false));
	}
	else if (auto* RocketData = Cast<URocketData>(ProjectileData))
	{
		OutBullet = Cast<AWeaponProjectile>(RocketPool->SpawnFromLocationAndRotation(Location, Rotation, false));
	}
	else if (auto* LaserData = Cast<ULaserData>(ProjectileData))
	{
		OutBullet = Cast<AWeaponProjectile>(BulletPool->SpawnFromLocationAndRotation(Location, Rotation, false));
	}

	if (OutBullet)
	{
		OutBullet->LoadProjectileData(ProjectileData);
		OutBullet->SetActive(true);
	}
	return OutBullet;
}
