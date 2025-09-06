// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletPoolManager.h"

#include "ActorPool.h"
#include "Weapons/Weapon.h"
#include "Weapons/Grenade.h"

// Sets default values for this component's properties
UBulletPoolManager::UBulletPoolManager()
{
	BulletPool = CreateDefaultSubobject<UActorPool>(TEXT("Default Bullet Pool"));
	M90Pool = CreateDefaultSubobject<UActorPool>(TEXT("M90 Pool"));
	M99Pool = CreateDefaultSubobject<UActorPool>(TEXT("M99 Pool"));
	GrenadePool = CreateDefaultSubobject<UActorPool>(TEXT("Grenade Pool"));
	RocketPool = CreateDefaultSubobject<UActorPool>(TEXT("Rocket Pool"));
}

AGrenade* UBulletPoolManager::SpawnGrenade(const FVector& Location, const FRotator& Rotation, const int32 NadeType)
{
	switch (NadeType) {
	case 0: // Regular
		return Cast<AGrenade>(GrenadePool->SpawnFromLocationAndRotation(Location, Rotation));
		break;
	case 1: // Plasma
		
		break;
	default:
		break;
	}
	return nullptr;
}


AActor* UBulletPoolManager::SpawnBullet(const FVector& Location, const FRotator& Rotation, const EWeapon& BulletType)
{
	switch (BulletType)
	{
	case M90:
		return M90Pool->SpawnFromLocationAndRotation(Location, Rotation);
	 	break;
	// case BR55:
	// 	break;
	// case MA5C:
	// 	break;
	// case M6CSOC:
	// 	break;
	case M99:
		return M99Pool->SpawnFromLocationAndRotation(Location, Rotation);
	case M9:
		return GrenadePool->SpawnFromLocationAndRotation(Location, Rotation);
	case RocketLauncher:
		return RocketPool->SpawnFromLocationAndRotation(Location, Rotation);
	default:
		return BulletPool->SpawnFromLocationAndRotation(Location, Rotation);
	}
}


