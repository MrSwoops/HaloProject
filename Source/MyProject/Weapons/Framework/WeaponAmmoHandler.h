#pragma once

#include "CoreMinimal.h"
#include "WeaponAmmoHandler.generated.h"

class UWeaponUIData;
class UWeaponUIWidget;
class AWeapon;
class UFMODAudioComponent;
class UFMODEvent;
class UWeaponAmmoData;

UCLASS(BlueprintType)
class UWeaponAmmoHandler : public UObject
{
	GENERATED_BODY()
public:
	ACharacter* CharacterOwner;
	bool IsPlayerOwned = false;
	AWeapon* WeaponOwner;
	
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

	bool CanLootAmmo;
	UFMODEvent* LootAmmoSound;
	
	virtual void OnShot();

	UWeaponUIWidget* WeaponUI;
	UWeaponUIData* UIData;
	virtual void AttachWeaponUI(UWeaponUIWidget* InWeaponUI);

	bool LootWeapon(UWeaponAmmoHandler* LootedWeapon);

	virtual bool CanShoot() { return HasAmmo(); }
	bool HasAmmo() const { return CurrentMagAmmo > 0; }
	bool IsEmpty() const { return CurrentMagAmmo <= 0 && CurrentReserveAmmo <= 0; }

	//UPROPERTY(EditDefaultsOnly)
	virtual void Initialize(UWeaponAmmoData* AmmoData, UWeaponUIData* InUIData);
	virtual void OnWeaponDropped() { CharacterOwner = nullptr; IsPlayerOwned = false; }
	virtual void UpdateAmmoHandler(float DeltaTime) {}

	UFUNCTION(BlueprintCallable)
	virtual float GetCurrentMagPercent() {return static_cast<float>(CurrentMagAmmo) / static_cast<float>(MaxMagSize);}
	
};
UCLASS(BlueprintType)
class UEnergyWeaponAmmoHandler : public UWeaponAmmoHandler
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Heating")
	float MaxHeatLevel = 50.0f;
	UPROPERTY()
	float CurrentHeatLevel = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Heating")
	float HeatBuildupPerShot = 7.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Heating")
	float HeatDissipationSpeed = 5.0f;

	UPROPERTY()
	bool IsOverHeated = false;

	virtual void Initialize(UWeaponAmmoData* AmmoData, UWeaponUIData* InUIData) override;
	virtual void TriggerReload() override;

	virtual void AttachWeaponUI(UWeaponUIWidget* InWeaponUI) override;

	virtual void OnShot() override;
	
	virtual float GetCurrentMagPercent() override {return 1 - (CurrentHeatLevel / MaxHeatLevel);}

	virtual void UpdateAmmoHandler(float DeltaTime) override;

	virtual bool CanShoot() override { return Super::CanShoot() && !IsOverHeated; }
	
private:
	void SetOverheated();
	
};