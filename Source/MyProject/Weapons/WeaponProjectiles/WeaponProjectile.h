#pragma once

#include "CoreMinimal.h"
#include "MyProject/PooledActor.h"
#include "MyProject/Interfaces/DamageDealer.h"
#include "WeaponProjectile.generated.h"

class UProjectileData;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class MYPROJECT_API AWeaponProjectile : public APooledActor, public IDamageDealer
{
	GENERATED_BODY()

public:

	AWeaponProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* HitCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* Movement;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void SetActive(bool i) override;
protected:

	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

	UPROPERTY()
	UProjectileData* ProjectileData;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual const float GetDamage() override;

	UFUNCTION()
	virtual void LoadProjectileData(UProjectileData* InBulletData);
};
