// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BodyPart.h"
#include "Components/BoxComponent.h"
#include "HurtBox.generated.h"

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
	EBodyPart HurtboxType;
};
