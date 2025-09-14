#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Weapon.h"
#include "BulletData.generated.h"

USTRUCT(BlueprintType)
struct FBulletData
{
	GENERATED_BODY()

	FBulletData()
		: Damage(0.f), Speed(0.f), Range(0.f), HasPenetration(false)
	{}

	FBulletData(float BulletDamage, float MaxRange, float BulletSpeed, FGameplayTag InWeaponTag, bool DoesPenetration, FGameplayTag BulletDamageType)
		: Damage(BulletDamage), Speed(BulletSpeed), Range(MaxRange), HasPenetration(DoesPenetration), WeaponTag(InWeaponTag), BulletDamageTag(BulletDamageType)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed; // m/s

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range; // meters

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasPenetration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag WeaponTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag BulletDamageTag;
};

namespace BulletDamageTag
{
	static const FGameplayTag Bullet = FGameplayTag::RequestGameplayTag(FName("Damage.Bullet"));
	static const FGameplayTag Energy = FGameplayTag::RequestGameplayTag(FName("Damage.Energy"));
}

namespace BulletData
{
	static const FBulletData Default = FBulletData(10.0f, 400, 10000.0f, WeaponTags::BR55, false, BulletDamageTag::Bullet);
	static const FBulletData BR55 = FBulletData(6.0f, 400, 10000.0f, WeaponTags::BR55, false, BulletDamageTag::Bullet);
	static const FBulletData M90 = FBulletData(6.0f, 400, 10000.0f, WeaponTags::BR55, false, BulletDamageTag::Bullet);
	static const FBulletData SRS = FBulletData(80.0f, 500, 12000.0f, WeaponTags::SRS, true, BulletDamageTag::Bullet);
	static const FBulletData SP4NKR = FBulletData(80.0f, 500, 5000.0f, WeaponTags::SRS, true, BulletDamageTag::Bullet);
	static const FBulletData PlasPistol = FBulletData(80.0f, 500, 3000.0f, WeaponTags::PlasPistol, true, BulletDamageTag::Energy);
}
