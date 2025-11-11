// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_UpdateThreatTarget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTS_UpdateThreatTarget : public UBTService
{
	GENERATED_BODY()
public:
	UBTS_UpdateThreatTarget();

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector OutTargetKey;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
	// FBlackboardKeySelector
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
