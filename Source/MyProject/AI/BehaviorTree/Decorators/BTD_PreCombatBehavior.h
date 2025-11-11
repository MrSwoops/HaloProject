// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_PreCombatBehavior.generated.h"

enum class EDefaultAIBehavior : uint8;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTD_PreCombatBehavior : public UBTDecorator
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "BehaviorTree")
	EDefaultAIBehavior PreCombatBehavior;

	virtual FString GetStaticDescription() const override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
