#pragma once

#include "CoreMinimal.h"
//#include "../Weapon.h"
#include "GameFramework/Character.h"
#include "MyProject/Components/BulletPoolManager.h"
//#include "../../GameplayCharacter.h"
#include "WeaponFireHandler.generated.h"

class UFMODEvent;
class UWeaponFireData;
class AWeapon;
class AGameplayCharacter;

UCLASS(BlueprintType)
class UWeaponFireHandler : public UObject
{
	GENERATED_BODY()
public:

	virtual void Initialize(UWeaponFireData* FireData, UBulletPoolManager* BulletPoolManager);
	
	UFUNCTION(BlueprintCallable, Category = "Weapons|FireHandler")
	virtual void FirePressed() { IsFireHeld = true; }
	UFUNCTION(BlueprintCallable, Category = "Weapons|FireHandler")
	virtual void FireReleased() { IsFireHeld = false; }
	UFUNCTION(BlueprintCallable, Category = "Weapons|FireHandler")
	virtual void FireHeld() {}

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
	

	void GetBulletSpawnInfo(FVector& SpawnLocation, FVector& Direction);
	void GetPlayerBulletSpawnInfo(FVector& SpawnLocation, FVector& Direction);

	virtual void FireBullet();
	

	FVector* AimPoint;
};

UCLASS(BlueprintType)
class USingleFireHandler : public UWeaponFireHandler
{
	GENERATED_BODY()
public:
	virtual void Initialize(UWeaponFireData* FireData, UBulletPoolManager* BulletPoolManager) override;
	
	virtual void FirePressed() override;
	
};

UCLASS(BlueprintType)
class UAutomaticFireHandler : public USingleFireHandler
{
	GENERATED_BODY()
public:
	virtual void FireHeld() override
	{
		if (!OnFireRateCD)
		{
			OnFireRateCD = true;
			Super::FirePressed();
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
	virtual void Initialize(UWeaponFireData* FireData, UBulletPoolManager* BulletPoolManager) override;

	virtual void FirePressed() override
	{
		if (!OnFireRateCD)
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
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBurstFireHandler::FireBullet, BurstRate, false);
		}
	}

	float BurstRate;
	int32 BurstShots;
	
private:
	int32 CurrentBurstCount = 0;
};

UCLASS(BlueprintType)
class UChargeFireHandler : public UWeaponFireHandler
{
	GENERATED_BODY()
public:
	
	
};

UCLASS(BlueprintType)
class UPelletFireHandler : public UWeaponFireHandler
{
	GENERATED_BODY()
public:
	virtual void Initialize(UWeaponFireData* FireData, UBulletPoolManager* BulletPoolManager) override;

	virtual void FireBullet() override
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			FVector Direction;
			FVector SpawnLocation;
			if (IsPlayerOwned)
				GetPlayerBulletSpawnInfo(SpawnLocation, Direction);
			else
				GetBulletSpawnInfo(SpawnLocation, Direction);
			
			//if (bHit) DrawDebugLine(GetWorld(), SpawnLocation, HitResult.ImpactPoint, FColor::Green, false, 5.f, 0, 1.f); else DrawDebugLine(GetWorld(), SpawnLocation, SpawnLocation + (Direction * 20000), FColor::Red, false, 5.f, 0, 1.f);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			FVector ForwardVector = Direction;//SpawnRotation.Vector(); // Converts rotation to direction vector
			float ConeHalfAngleRad = FMath::DegreesToRadians(Spread); // Spread is an angle in radians. Convert degrees if needed:
			for (int32 i = 0; i < PelletsPerShot; i++)
			{
				FVector RandomDirection = FMath::VRandCone(ForwardVector, ConeHalfAngleRad);
				FRotator SpreadRotation = RandomDirection.Rotation(); // Get new rotation from direction
			
				BulletManager->SpawnBullet(SpawnLocation, SpreadRotation, *WeaponType);
				//DrawDebugLine(World, SpawnLocation, SpawnLocation + RandomDirection * 1000.0f, FColor::Red, false, 1.0f, 0, 1.0f);
			}
		}
	}
	
	int32 PelletsPerShot;
	
};