#include "BTD_IsInCombat.h"

#include "MyProject/AI/AICharacter.h"
#include "MyProject/AI/AICharacterController.h"

bool UBTD_IsInCombat::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAICharacterController* AICon = Cast<AAICharacterController>(OwnerComp.GetAIOwner());
	if (!AICon) return false;
	
	AAICharacter* AICharacter = Cast<AAICharacter>(AICon->GetPawn());
	if (!AICharacter) return false;

	return AICharacter->IsInCombat;
}
