// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "CharacterBehaviorTree.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UCharacterBehaviorTree : public UBehaviorTree
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "BehaviorTree")
	void CalculateConfidence();
	
	
};
