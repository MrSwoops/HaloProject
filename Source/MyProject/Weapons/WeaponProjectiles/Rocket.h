// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "Rocket.generated.h"

class UFMODEvent;
class UFMODAudioComponent;
class UExplosiveComponent;
/**
 * 
 */
UCLASS()
class MYPROJECT_API ARocket : public AWeaponProjectile
{
	GENERATED_BODY()
public:

	ARocket();
	
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UExplosiveComponent* ExplosiveComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Audio")
	UFMODEvent* PropelLoopSoundEvent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Audio")
	UFMODAudioComponent* PropelLoopSoundComponent;

	virtual void SetActive(bool i) override;
};
