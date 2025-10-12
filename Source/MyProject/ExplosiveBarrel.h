// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IDamageable.h"
#include "ExplosiveBarrel.generated.h"

class UProjectileData;
class UExplosiveComponent;

UCLASS()
class MYPROJECT_API AExplosiveBarrel : public AActor, public IIDamageable
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
	virtual void TakeDamage(IDamageDealer* DD) override;
	virtual void TakeDamage(const int32& Damage) override;
	virtual void TakeProjectileDamage(const UProjectileData*, const EHurtboxType&) override;

	void Reset();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
