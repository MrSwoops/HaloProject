#pragma once

#include "CoreMinimal.h"
#include "MyProject/Weapons/Framework/WeaponFireHandler.h"
#include "WeaponFireData.generated.h"

class UFMODEvent;
class UWeaponFireHandler;

UCLASS()
class MYPROJECT_API UWeaponFireData : public UDataAsset
{
	GENERATED_BODY()
public:

	UWeaponFireData()
		: Spread(5.0f), FireRate(0.1f), FireHandlerType(UWeaponFireHandler::StaticClass())
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Spread;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFMODEvent* FireSoundEvent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFMODEvent* DryFireSoundEvent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* FireAnimation;

	TSubclassOf<UWeaponFireHandler> FireHandlerType;
	
};

UCLASS(Blueprintable, BlueprintType, Category = "My Custom Data Assets")
class MYPROJECT_API USingleFireData : public UWeaponFireData
{
	GENERATED_BODY()
public:
	USingleFireData()
	{
		FireHandlerType = USingleFireHandler::StaticClass();
	}
	
};

UCLASS(Blueprintable, BlueprintType, Category = "My Custom Data Assets")
class MYPROJECT_API UAutomaticFireData : public UWeaponFireData
{
	GENERATED_BODY()
public:
	UAutomaticFireData()
	{
		FireHandlerType = UAutomaticFireHandler::StaticClass();
	}
	
};

UCLASS(Blueprintable, BlueprintType, Category = "My Custom Data Assets")
class MYPROJECT_API UBurstWeaponFireData : public UWeaponFireData
{
	GENERATED_BODY()
public:

	UBurstWeaponFireData()
		: BurstRate(0.1f), BurstShots(3)
	{
		FireHandlerType = UBurstFireHandler::StaticClass();
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BurstRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BurstShots;
	
};
UCLASS(Blueprintable, BlueprintType, Category = "My Custom Data Assets")
class MYPROJECT_API UPelletWeaponFireData : public UWeaponFireData
{
	GENERATED_BODY()
public:

	UPelletWeaponFireData()
		: PelletsPerShots(15)
	{
		FireHandlerType = UPelletFireHandler::StaticClass();
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PelletsPerShots;
	
};

UCLASS(Blueprintable, BlueprintType, Category = "My Custom Data Assets")
class MYPROJECT_API UChargeWeaponFireData : public UWeaponFireData
{
	GENERATED_BODY()
public:
	UChargeWeaponFireData()
		: MaxCharge(1.0f), ChargeSpeed(0.2f), OnlyFireOnFullCharge(false), AutoFireOnChargeCompletion(false)
	{
		FireHandlerType = UChargeFireHandler::StaticClass();
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCharge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChargeSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool OnlyFireOnFullCharge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AutoFireOnChargeCompletion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFMODEvent* ChargeSoundEvent;
	
};