#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_TargetHasLOSDestination.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTD_TargetHasLOSDestination : public UBTDecorator
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Pathing")
	FBlackboardKeySelector InTargetKey;
	UPROPERTY(EditAnywhere, Category = "Pathing")
	FBlackboardKeySelector InDestinationKey;

	virtual FString GetStaticDescription() const override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};