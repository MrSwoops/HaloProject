// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExplosiveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UExplosiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExplosiveComponent();

	UPROPERTY(EditDefaultsOnly)
	float CoreDamage = 75.0f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Audio")
	TArray<USoundBase*> ExplosionSounds;
	
	UPROPERTY(EditDefaultsOnly)
	float ExplosionRadius = 175.0f;

	UPROPERTY(EditDefaultsOnly)
	float BaseExplosionForce = 15000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> ExplodableObjectTypes;

	UFUNCTION()
	void ExplodeActor(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FVector& ExplosionLocation);
public:
	
	void CreateExplosion();
};
