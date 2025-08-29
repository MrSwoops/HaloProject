// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PooledActor.h"
#include "GameFramework/Actor.h"
#include "Interfaces/DamageDealer.h"
#include "Grenade.generated.h"

class UMyProjectPickUpComponent;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class MYPROJECT_API AGrenade : public APooledActor, public IDamageDealer
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

	bool Armed = false;

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;
	

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UMyProjectPickUpComponent* PickUpComponent;

	virtual void SetActive(bool i) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnExplode(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp);

	
	UPROPERTY(EditDefaultsOnly)
	bool Sticky = false;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 75.0f;
	UFUNCTION(BlueprintCallable)
	virtual const float& GetDamage() override { return Damage;}
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* ExplosionSound;

private:
	UPROPERTY(EditDefaultsOnly)
	float FuseTime = 2.5f;
	float Timer = 0.0f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditDefaultsOnly)
	float ExplosionRadius = 175.0f;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Arm(const float& ArmTime);

	void Explode();

};
