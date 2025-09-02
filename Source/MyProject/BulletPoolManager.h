// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BulletPoolManager.generated.h"


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

	AActor* SpawnBullet(const FVector& Location, const FRotator& Rotation, const EWeapon& BulletType);

	AGrenade* SpawnGrenade(const FVector& Location, const FRotator& Rotation, const int32 NadeType);

protected:

		
};
