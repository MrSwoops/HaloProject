
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_NeedToReload.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTD_NeedToReload : public UBTDecorator
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "BehaviorTree", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float AmmoPercent = 0.0f;

	virtual FString GetStaticDescription() const override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};