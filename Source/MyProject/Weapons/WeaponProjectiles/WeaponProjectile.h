#pragma once

#include "CoreMinimal.h"
#include "MyProject/PooledActor.h"
#include "MyProject/Interfaces/DamageDealer.h"
#include "WeaponProjectile.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
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

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//UNiagaraSystem* NiagaraSystem;

	// The Niagara component to hold the VFX
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	UNiagaraComponent* NiagaraComponent;
	// The Niagara system asset for the bullet trail
	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* BulletTrailSystem;
	void EnableBulletTrail();
	void DisableBulletTrail();

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
