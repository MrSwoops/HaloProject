#include "BTD_RandomChance.h"


// bool UBTD_RandomChance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
// {
// 	return FMath::FRand() <= TriggerChance;
// }
FString UBTD_RandomChance::GetStaticDescription() const
{
	FString Desc;

	Desc += FString::Printf(TEXT("Search Radius: %.001f\n"), TriggerChance);

	return Desc;
}