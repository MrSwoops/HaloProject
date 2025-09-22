// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletPoolManager.h"

#include "ActorPool.h"
#include "../Bullet.h"
#include "../Weapons/BulletData.h"
#include "../Weapons/Weapon.h"
#include "../Weapons/Grenade.h"

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
		return Cast<AGrenade>(GrenadePool->SpawnFromLocationAndRotation(Location, Rotation, false));
		break;
	case 1: // Plasma
		
		break;
	default:
		break;
	}
	return nullptr;
}

ABullet* UBulletPoolManager::SpawnBullet(const FVector& Location, const FRotator& Rotation, FGameplayTag WeaponType)
{
	using namespace WeaponTags;
	ABullet* OutBullet;
	if (WeaponType == M90)
	{
		OutBullet = Cast<ABullet>(M90Pool->SpawnFromLocationAndRotation(Location, Rotation, false));
		OutBullet->LoadBulletData(BulletData::M90);
	}
	else if (WeaponType == M99)
	{
		OutBullet = Cast<ABullet>(M99Pool->SpawnFromLocationAndRotation(Location, Rotation, false));
		OutBullet->LoadBulletData(BulletData::Default);
	}
	else if (WeaponType == SP4NKR)
	{
		OutBullet = Cast<ABullet>(RocketPool->SpawnFromLocationAndRotation(Location, Rotation, false));
		OutBullet->LoadBulletData(BulletData::SP4NKR);
	}
	else if (WeaponType == BR55)
	{
		OutBullet = Cast<ABullet>(BulletPool->SpawnFromLocationAndRotation(Location, Rotation, false));
		OutBullet->LoadBulletData(BulletData::BR55);
	}
	else if (WeaponType == SRS)
	{
		OutBullet = Cast<ABullet>(BulletPool->SpawnFromLocationAndRotation(Location, Rotation, false));
		OutBullet->LoadBulletData(BulletData::SRS);
	}
	else
	{
		OutBullet = Cast<ABullet>(BulletPool->SpawnFromLocationAndRotation(Location, Rotation, false));
		OutBullet->LoadBulletData(BulletData::Default);
	}
	OutBullet->SetActive(true);
	return OutBullet;
}
