#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_RandomChance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTD_RandomChance : public UBTDecorator
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "BehaviorTree", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float TriggerChance = 0.0f;

	virtual FString GetStaticDescription() const override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override { return FMath::FRand() <= TriggerChance; }
};