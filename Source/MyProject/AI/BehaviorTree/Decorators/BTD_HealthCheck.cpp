#include "BTD_HealthCheck.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "MyProject/AI/AICharacter.h"
#include "MyProject/AI/AICharacterController.h"

UBTD_HealthCheck::UBTD_HealthCheck()
{
	bNotifyBecomeRelevant = true;
	bAllowAbortChildNodes = true;
	BlackboardKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_HealthCheck, BlackboardKey));
}

// void UBTD_HealthCheck::InitializeFromAsset(UBehaviorTree& Asset)
// {
// 	Super::InitializeFromAsset(Asset);
//
// 	UBlackboardData* BBAsset = GetBlackboardAsset();
// 	if (ensure(BBAsset))
// 	{
// 		// Tell the BT that this decorator depends on a float blackboard key
// 		InHealthPercentKey.ResolveSelectedKey(*BBAsset);
// 	}
// 	
// }

FString UBTD_HealthCheck::GetStaticDescription() const
{
	FString Desc;

	//TEnumAsByte<EArithmeticKeyOperation::Type>
	FString BehaviorString = StaticEnum<EArithmeticKeyOperation::Type>()->GetNameStringByValue(ComparisonType);
	Desc += FString::Printf(TEXT("Comparison: %s\n"), *BehaviorString);
	Desc += FString::Printf(TEXT("Compared Percent: %.001f\n"), ComparisonPercent);

	return Desc;
}


bool UBTD_HealthCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAICharacterController* AICon = Cast<AAICharacterController>(OwnerComp.GetAIOwner());
	if (!AICon) return false;
	
	AAICharacter* AICharacter = Cast<AAICharacter>(AICon->GetPawn());
	if (!AICharacter) return false;

	float HealthPercent = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BlackboardKey.SelectedKeyName);
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, "Health Percent: " + FString::SanitizeFloat(HealthPercent) + ". Compare Percent: " + FString::SanitizeFloat(ComparisonPercent));
	using namespace EArithmeticKeyOperation;
	switch (ComparisonType)
	{
	case Less:
		return HealthPercent < ComparisonPercent;
	case LessOrEqual:
		if (GEngine)
		{
			
			//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, (HealthPercent <= ComparisonPercent) ? "true" : "false");
		}
		return HealthPercent <= ComparisonPercent;
	case Greater:
		return HealthPercent > ComparisonPercent;
	case GreaterOrEqual:
		return HealthPercent >= ComparisonPercent;
	case Equal:
		return HealthPercent == ComparisonPercent;
	case NotEqual:
		return HealthPercent != ComparisonPercent;
	default:
		return false;
	}
}