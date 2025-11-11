#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BTD_HealthCheck.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTD_HealthCheck : public UBTDecorator_Blackboard
{
	GENERATED_BODY()

public:
	UBTD_HealthCheck();

protected:
	// UPROPERTY(EditAnywhere, Category = "Blackboard")
	// FBlackboardKeySelector InHealthPercentKey;
	
	UPROPERTY(EditAnywhere, Category = "BehaviorTree")
	TEnumAsByte<EArithmeticKeyOperation::Type> ComparisonType;

	UPROPERTY(EditAnywhere, Category = "BehaviorTree", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ComparisonPercent = 0.1f;

	virtual FString GetStaticDescription() const override;
	//virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
