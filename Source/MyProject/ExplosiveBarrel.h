// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Interfaces/Damageable.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrel.generated.h"

class UProjectileData;
class UExplosiveComponent;

UCLASS()
class MYPROJECT_API AExplosiveBarrel : public AActor, public IDamageable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosiveBarrel();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UExplosiveComponent* ExplosiveComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth = 0;
	float Health = 0;

	virtual void TakeDamage_Implementation(const FDamageMessage& DmgMsg) override;
	virtual void Die_Implementation() override;
	virtual bool IsDead_Implementation() const override { return bExploded;}

	void Reset();

private:
	bool bExploded = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
