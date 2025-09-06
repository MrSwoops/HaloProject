// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Bullet.h"
#include "Rocket.generated.h"

class UExplosiveComponent;
/**
 * 
 */
UCLASS()
class MYPROJECT_API ARocket : public ABullet
{
	GENERATED_BODY()
public:

	ARocket();
	
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UExplosiveComponent* ExplosiveComp;
};
