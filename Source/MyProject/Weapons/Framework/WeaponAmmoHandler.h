#pragma once

#include "CoreMinimal.h"
#include "WeaponAmmoHandler.generated.h"

class AWeapon;
class UFMODAudioComponent;
class UFMODEvent;
class UWeaponAmmoData;

UCLASS(BlueprintType)
class UWeaponAmmoHandler : public UObject
{
	GENERATED_BODY()
public:
	ACharacter* WeaponOwner;
	AWeapon* CharacterOwner;
	
	int32 MaxMagSize;
	int32 MaxReserveMags;

	int32 CurrentMagAmmo;
	int32 CurrentReserveAmmo;

	UAnimMontage* ReloadAnimation;
	float ReloadOffsetTime = 0.0f;
	UFMODEvent* ReloadSoundEvent;
	virtual void TriggerReload();
	virtual void OnReload();

	virtual void CancelReload();
	FTimerHandle ReloadTimerHandle;
	UFMODAudioComponent* ReloadFMODInstance;

	
	virtual void OnShot()
	{
		CurrentMagAmmo--;
	}

	virtual bool CanShoot(){ return HasAmmo(); }
	bool HasAmmo() const { return CurrentMagAmmo > 0; }

	//UPROPERTY(EditDefaultsOnly)
	virtual void Initialize(UWeaponAmmoData* AmmoData);

	virtual float GetCurrentMagPercent() {return static_cast<float>(CurrentMagAmmo) / static_cast<float>(MaxMagSize);}
	
};
UCLASS(BlueprintType)
class UEnergyWeaponAmmoHandler : public UWeaponAmmoHandler
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	float MaxHeatLevel = 50.0f;
	UPROPERTY()
	float CurrentHeatLevel = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	float HeatBuildupPerShot = 7.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	float HeatDissipationSpeed = 5.0f;

	UPROPERTY()
	bool IsOverHeated = false;

	virtual void Initialize(UWeaponAmmoData* AmmoData) override;
	
	virtual float GetCurrentMagPercent() override {return 1 - (CurrentHeatLevel / MaxHeatLevel);}
	
};