#pragma once

#include "CoreMinimal.h"
#include "WeaponFireHandler.generated.h"

struct FGameplayTag;
class UBulletPoolManager;
class UWeaponAmmoData;
class UWeaponAmmoHandler;
class UProjectileData;
class UFMODEvent;
class UWeaponFireData;
class AWeapon;
class AGameplayCharacter;

UCLASS(BlueprintType)
class UWeaponFireHandler : public UObject
{
	GENERATED_BODY()
public:

	virtual void Initialize(UWeaponFireData* FireData, UWeaponAmmoHandler* InAmmoHandler, UProjectileData* InProjectileData, UBulletPoolManager* BulletPoolManager);
	virtual void OnWeaponDropped()
	{
		CharacterOwner = nullptr;
		IsPlayerOwned = false;
	}
	
	UFUNCTION(BlueprintCallable, Category = "Weapons|FireHandler")
	virtual void FirePressed() { IsFireHeld = true; }
	UFUNCTION(BlueprintCallable, Category = "Weapons|FireHandler")
	virtual void FireReleased() { IsFireHeld = false; }
	UFUNCTION(BlueprintCallable, Category = "Weapons|FireHandler")
	virtual void FireHeld(float DeltaTime) {}

	bool IsFireHeld = false;

	FGameplayTag* WeaponType;
	FVector* MuzzleOffset;
	ACharacter* CharacterOwner;
	AActor* WeaponOwner;
	bool IsPlayerOwned = false;
	float Spread = 0.0f;
	float FireRate = 0.1f;
	bool OnFireRateCD = false;
	UBulletPoolManager* BulletManager;

	UFMODEvent* FireSoundEvent;
	UFMODEvent* DryFireSoundEvent;
	UAnimMontage* FireAnimation;

	UWeaponAmmoHandler* AmmoHandler;
	UProjectileData* ProjectileData;

protected:
	void GetBulletSpawnInfo(FVector& SpawnLocation, FVector& Direction);
private:
	void GetPlayerBulletSpawnInfo(FVector& SpawnLocation, FRotator& LookRotation, FVector& Start, FVector& End);
	void GetAIBulletSpawnInfo(FVector& SpawnLocation, FRotator& LookRotation, FVector& Start, FVector& End);
	
public:
	virtual void FireBullet();
	FVector* AimPoint;

protected:
	bool IsReadyToFire();
	void PlayFireAnimation();
};

UCLASS(BlueprintType)
class USingleFireHandler : public UWeaponFireHandler
{
	GENERATED_BODY()
public:
	virtual void Initialize(UWeaponFireData* FireData, UWeaponAmmoHandler* InAmmoHandler, UProjectileData* InProjectileData, UBulletPoolManager* BulletPoolManager) override;
	
	virtual void FirePressed() override;
	
};

UCLASS(BlueprintType)
class UAutomaticFireHandler : public USingleFireHandler
{
	GENERATED_BODY()
public:
	virtual void FirePressed() override
	{ IsFireHeld = true; }
	virtual void FireHeld(float DeltaTime) override
	{
		if (IsReadyToFire())
		{
			OnFireRateCD = true;
			FireBullet();
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { OnFireRateCD = false; }, FireRate, false);
		}
	}
	
};

UCLASS(BlueprintType)
class UBurstFireHandler : public UWeaponFireHandler
{
	GENERATED_BODY()
public:
	virtual void Initialize(UWeaponFireData* FireData, UWeaponAmmoHandler* InAmmoHandler, UProjectileData* InProjectileData, UBulletPoolManager* BulletPoolManager) override;

	virtual void OnWeaponDropped() override
	{
		Super::OnWeaponDropped();
		CharacterOwner = nullptr;
		IsPlayerOwned = false;
		GetWorld()->GetTimerManager().ClearTimer(BurstTimerHandle);
	}
	
	virtual void FirePressed() override
	{
		if (IsReadyToFire())
		{
			OnFireRateCD = true;
			CurrentBurstCount = 0;
			Super::FirePressed();
			FireBullet();
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { OnFireRateCD = false; }, FireRate, false);
		}
	}

	virtual void FireBullet() override
	{
		Super::FireBullet();
		CurrentBurstCount++;
		if (CurrentBurstCount < BurstShots)
		{
			GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, this, &UBurstFireHandler::FireBullet, BurstRate, false);
		}
	}

	float BurstRate;
	int32 BurstShots;
	
private:
	FTimerHandle BurstTimerHandle;
	int32 CurrentBurstCount = 0;
};

UCLASS(BlueprintType)
class UChargeFireHandler : public UWeaponFireHandler
{
	GENERATED_BODY()
public:
	virtual void Initialize(UWeaponFireData* FireData, UWeaponAmmoHandler* InAmmoHandler, UProjectileData* InProjectileData, UBulletPoolManager* BulletPoolManager) override;
	virtual void FirePressed() override;
	virtual void FireHeld(float DeltaTime) override;
	virtual void FireReleased() override;

	UFMODEvent* ChargeSoundEvent;
	
	float MaxCharge;
	float CurrentCharge = 0.0f;
	float ChargeSpeed;

	bool OnlyFireOnFullCharge;
	bool AutoFireOnChargeCompletion;
	
};

UCLASS(BlueprintType)
class UPelletFireHandler : public UWeaponFireHandler
{
	GENERATED_BODY()
public:
	virtual void Initialize(UWeaponFireData* FireData, UWeaponAmmoHandler* InAmmoHandler, UProjectileData* InProjectileData, UBulletPoolManager* BulletPoolManager) override;

	virtual void FirePressed() override;

	virtual void FireBullet() override;
	
	int32 PelletsPerShot;
	
};