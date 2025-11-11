#include "BTD_IsUnderFire.h"

#include "MyProject/AI/AICharacter.h"
#include "MyProject/AI/AICharacterController.h"

bool UBTD_IsUnderFire::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAICharacterController* AICon = Cast<AAICharacterController>(OwnerComp.GetAIOwner());
	if (!AICon) return false;
	
	AAICharacter* AICharacter = Cast<AAICharacter>(AICon->GetPawn());
	if (!AICharacter) return false;
if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Check");
	return AICharacter->IsUnderFire;
}
