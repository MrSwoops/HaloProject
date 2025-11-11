// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_GetRandomTraversablePoint.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTT_GetRandomTraversablePoint : public UBTTaskNode
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector OutPointKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta=(EditCondition="!UseSelfAsOrigin", EditConditionHides, AllowedTypes="Object,Vector", ToolTip="Target location or actor when not using self as the origin."))
	FBlackboardKeySelector OriginKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool UseSelfAsOrigin = false;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float SearchRadius = 1000;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
	
};
