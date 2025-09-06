// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "HurtBox.generated.h"

UENUM(BlueprintType, meta = (Bitflags = "true"))
enum EHurtboxType : int
{
	Default,
	Head,
	Torso,
	Limb
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UHurtBox : public UBoxComponent
{
	GENERATED_BODY()

public:


	UHurtBox();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HurtBox")
	TEnumAsByte<EHurtboxType> HurtboxType;
	
};
