#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsInCombat.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTD_IsInCombat : public UBTDecorator
{
	GENERATED_BODY()

protected:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};