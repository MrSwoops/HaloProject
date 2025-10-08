#pragma once

#include "CoreMinimal.h"
#include "MyProject/Weapons/Framework/WeaponAmmoHandler.h"
#include "WeaponAmmoData.generated.h"

class UFMODEvent;
class UWeaponAmmoHandler;

UCLASS(Blueprintable)
class MYPROJECT_API UWeaponAmmoData : public UDataAsset
{
	GENERATED_BODY()
public:

	UWeaponAmmoData()
		: MaxMagSize(0), MaxReserveMags(0), AmmoPerShot(1), AmmoHandlerType(UWeaponAmmoHandler::StaticClass())
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	int32 MaxMagSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	int32 MaxReserveMags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	int32 AmmoPerShot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reload")
	UAnimMontage* ReloadAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reload")
	float ReloadOffsetTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reload")
	UFMODEvent* ReloadSoundEvent;

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<UWeaponAmmoHandler> AmmoHandlerType;
	
};

UCLASS(Blueprintable)
class MYPROJECT_API UEnergyAmmoData : public UWeaponAmmoData
{
	GENERATED_BODY()
public:
	UEnergyAmmoData()
		: MaxHeatLevel(50.0f), HeatBuildupPerShot(7.5f), HeatDissipationSpeed(5.0f)
	{
		AmmoHandlerType = UEnergyWeaponAmmoHandler::StaticClass();		
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Energy")
	float MaxHeatLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Energy")
	float HeatBuildupPerShot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Energy")
	float HeatDissipationSpeed;
	
};
