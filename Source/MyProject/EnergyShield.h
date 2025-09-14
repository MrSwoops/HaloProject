// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnergyShield.generated.h"


class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UEnergyShield : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnergyShield();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxEnergy = 70;
	float CurrentEnergy = 0;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RegenDelay = 5.0f;
	float CurrentRegenDelay = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RegenSpeed = 15.0f;

	FTimerHandle RegenDelayTimerHandle;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	APlayerCharacter* Player;

	UPROPERTY(EditAnywhere, Category = "Shield Regen")
	USoundBase* RegenSound; // Sound asset to play

	UAudioComponent* RegenAudioComponent;

	void StartEnergyRegen();
	void StopEnergyRegen();

	void ResetRegenDelay();

	int32 TakeDamage(const int32& IncomingDamage);
		
};
