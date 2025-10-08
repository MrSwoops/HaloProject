#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ProjectileData.generated.h"

enum CritHitBehavior : uint16;

UCLASS(Blueprintable)
class MYPROJECT_API UProjectileData : public UDataAsset
{
	GENERATED_BODY()
public:
	UProjectileData()
			: Damage(0.f), Speed(0.f), Range(0.f), HasPenetration(false)
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

UCLASS(Blueprintable)
class MYPROJECT_API UBulletData2 : public UProjectileData
{
	GENERATED_BODY()
public:
	
};

UCLASS(Blueprintable)
class MYPROJECT_API URocketData : public UProjectileData
{
	GENERATED_BODY()
public:
	
};

UCLASS(Blueprintable)
class MYPROJECT_API ULaserData : public UProjectileData
{
	GENERATED_BODY()
public:
	
};