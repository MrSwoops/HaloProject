// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnergyShieldShellSKM.generated.h"

class AGameplayCharacter;
/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYPROJECT_API UEnergyShieldShellSKM : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UEnergyShieldShellSKM();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInstance* ShieldMat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInstance* BrokenShieldMat;

	virtual void BeginPlay() override;

public:
	
	UFUNCTION()
	void SetShieldMaterials(bool Broken);

	UFUNCTION()
	void SetShieldVisibility(bool IsVisible);
	
};
