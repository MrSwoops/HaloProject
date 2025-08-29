// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PooledActor.h"
#include "Interfaces/DamageDealer.h"
#include "Bullet.generated.h"


class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class MYPROJECT_API ABullet : public APooledActor, public IDamageDealer
{
	GENERATED_BODY()

public:

	ABullet();

	UPROPERTY(EditDefaultsOnly)
	float Damage = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* HitCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* Movement;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void SetActive(bool i) override;
protected:

	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void FellOutOfWorld(const UDamageType& dmgType) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual const float& GetDamage() override;
};
