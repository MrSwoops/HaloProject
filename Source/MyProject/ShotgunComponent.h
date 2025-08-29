// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProjectWeaponComponent.h"
#include "ShotgunComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UShotgunComponent : public UMyProjectWeaponComponent
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly)
	float Spread = 30.0f;

	UPROPERTY(EditDefaultsOnly)
	int32 PelletsPerShot = 15;

	virtual void Fire() override;

private:
	bool Racked = true;
	void FinishedRacking();
};
