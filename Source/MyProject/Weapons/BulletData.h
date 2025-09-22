#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Weapon.h"
#include "BulletData.generated.h"

UENUM(BlueprintType, meta = (Bitflags))
enum CritHitBehavior : uint16
{
	Default = 0,
	InstaKHeadHealth = 1,
	InstaKHeadShield = 2,
};

USTRUCT(BlueprintType)
struct FBulletData
{
	GENERATED_BODY()

	FBulletData()
		: Damage(0.f), Speed(0.f), Range(0.f), HasPenetration(false)
	{}

	FBulletData(float BulletDamage, float MaxRange, float BulletSpeed, bool DoesPenetration, FGameplayTag BulletDamageType, TEnumAsByte<CritHitBehavior> InCritBehavior)
		: Damage(BulletDamage), Speed(BulletSpeed), Range(MaxRange), HasPenetration(DoesPenetration), BulletDamageTag(BulletDamageType), CritHitBehavior(InCritBehavior)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed; // m/s

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range; // meters

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasPenetration;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FGameplayTag WeaponTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag BulletDamageTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<CritHitBehavior> CritHitBehavior;
};



namespace BulletDamageTag
{
	static const FGameplayTag Bullet = FGameplayTag::RequestGameplayTag(FName("Damage.Bullet"));
	static const FGameplayTag Energy = FGameplayTag::RequestGameplayTag(FName("Damage.Energy"));
}

namespace BulletData
{
	static const FBulletData None = FBulletData(0.0f, 0, 0.0f, false, BulletDamageTag::Bullet, CritHitBehavior::Default);
	static const FBulletData Default = FBulletData(10.0f, 400, 10000.0f, false, BulletDamageTag::Bullet, CritHitBehavior::Default);
	static const FBulletData BR55 = FBulletData(6.0f, 400, 20000.0f, false, BulletDamageTag::Bullet, InstaKHeadHealth);
	static const FBulletData MA5C = FBulletData(7.5f, 400, 15000.0f, false, BulletDamageTag::Bullet, CritHitBehavior::Default);
	static const FBulletData M90 = FBulletData(6.0f, 400, 10000.0f, false, BulletDamageTag::Bullet, CritHitBehavior::Default);
	static const FBulletData SRS = FBulletData(80.0f, 500, 14000.0f, true, BulletDamageTag::Bullet, TEnumAsByte<CritHitBehavior>(InstaKHeadHealth | InstaKHeadShield));
	static const FBulletData SP4NKR = FBulletData(80.0f, 500, 5000.0f, false, BulletDamageTag::Bullet, CritHitBehavior::Default);
	static const FBulletData PlasPistol = FBulletData(7.0f, 500, 3000.0f, false, BulletDamageTag::Energy, CritHitBehavior::Default);
	static const FBulletData PlasRifle = FBulletData(5.0f, 500, 3000.0f, false, BulletDamageTag::Energy, CritHitBehavior::Default);
}
