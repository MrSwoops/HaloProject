#include "BTS_UpdateAgentInformation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "MyProject/AI/AICharacter.h"
#include "MyProject/AI/AICharacterController.h"
#include "MyProject/Components/WeaponInventory.h"
#include "MyProject/Weapons/Weapon.h"

void UBTS_UpdateAgentInformation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAICharacterController* AICon = Cast<AAICharacterController>(OwnerComp.GetAIOwner());
	if (!AICon) return;
	
	AAICharacter* AICharacter = Cast<AAICharacter>(AICon->GetPawn());
	if (!AICharacter) return;
	
	AICon->UpdateConfidence();
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("CurrentHealthPercent", AICharacter->GetHealthPercent());
	if (auto* Weapon = AICharacter->WeaponInventory->CurrentWeapon) OwnerComp.GetBlackboardComponent()->SetValueAsFloat("CurrentAmmoPercent", Weapon->AmmoHandler->GetCurrentMagPercent());
}
