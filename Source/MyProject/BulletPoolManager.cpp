// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletPoolManager.h"

#include "ActorPool.h"
#include "Weapon.h"

// Sets default values for this component's properties
UBulletPoolManager::UBulletPoolManager()
{
	BulletPool = CreateDefaultSubobject<UActorPool>(TEXT("Default Bullet Pool"));
	M90Pool = CreateDefaultSubobject<UActorPool>(TEXT("M90 Pool"));
	M99Pool = CreateDefaultSubobject<UActorPool>(TEXT("M99 Pool"));
	GrenadePool = CreateDefaultSubobject<UActorPool>(TEXT("Grenade Pool"));
}


void UBulletPoolManager::SpawnBullet(const FVector& Location, const FRotator& Rotation, const EWeapon& BulletType)
{
	switch (BulletType)
	{
	case M90:
		M90Pool->SpawnFromLocationAndRotation(Location, Rotation);
	 	break;
	// case BR55:
	// 	break;
	// case MA5C:
	// 	break;
	// case M6CSOC:
	// 	break;
	case M99:
		M99Pool->SpawnFromLocationAndRotation(Location, Rotation);
		break;
	case M9:
		GrenadePool->SpawnFromLocationAndRotation(Location, Rotation);
	default:
		BulletPool->SpawnFromLocationAndRotation(Location, Rotation);
		break;
	}
}


