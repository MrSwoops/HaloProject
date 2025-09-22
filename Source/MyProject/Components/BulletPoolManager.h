// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "BulletPoolManager.generated.h"

class ABullet;
struct FGameplayTag;
class AWeapon;
class AGrenade;
enum EWeapon : int;
class UActorPool;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UBulletPoolManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBulletPoolManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UActorPool* BulletPool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UActorPool* M90Pool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UActorPool* M99Pool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UActorPool* GrenadePool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UActorPool* RocketPool;

	ABullet* SpawnBullet(const FVector& Location, const FRotator& Rotation, FGameplayTag WeaponType);

	AGrenade* SpawnGrenade(const FVector& Location, const FRotator& Rotation, const int32 NadeType);

protected:


	UPROPERTY()
	TMap<FGameplayTag, UActorPool*> BulletPools;

	// Optional: default pool if no tag matches
	UPROPERTY()
	UActorPool* DefaultPool;
		
};
