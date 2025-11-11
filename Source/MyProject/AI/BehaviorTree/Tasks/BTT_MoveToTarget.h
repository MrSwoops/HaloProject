// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MoveToTarget.generated.h"

class AAICharacter;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTT_MoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_MoveToTarget();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Pathing")
	FBlackboardKeySelector InTargetKey;
	UPROPERTY(EditAnywhere, Category = "Pathing")
	float AcceptableGoalRadius;
	UPROPERTY(EditAnywhere, Category = "Pathing")
	float PathFollowLenience;
	TArray<FVector> CurrentPath;
	int CurrentIndex = 0;

	UPROPERTY(EditAnywhere, Category = "StuckCheck")
	float TimeUntilStuck;
	UPROPERTY(EditAnywhere, Category = "StuckCheck")
	float StuckPrecision;
	FVector PreviousLocation;
	float StuckTime;

	UPROPERTY(EditAnywhere, Category = "LookSettings")
	bool LookAlongPath;
	UPROPERTY(EditAnywhere, Category = "LookSettings")
	bool LookAtTarget;
	UPROPERTY(EditAnywhere, Category = "LookSettings")
	bool UsePitch;

	FVector2D GetMovementInput(AAICharacter* Char, FVector Direction);
	FVector GetMovementDirection();
protected:
	//virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
