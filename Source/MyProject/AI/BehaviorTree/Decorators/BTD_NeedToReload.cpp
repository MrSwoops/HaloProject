#include "BTD_NeedToReload.h"

#include "MyProject/AI/AICharacter.h"
#include "MyProject/AI/AICharacterController.h"
#include "MyProject/Components/WeaponInventory.h"
#include "MyProject/Weapons/Weapon.h"

FString UBTD_NeedToReload::GetStaticDescription() const
{
	FString Desc;

	Desc += FString::Printf(TEXT("Ammo Trigger Percent: %.001f\n"), AmmoPercent);

	return Desc;
}


bool UBTD_NeedToReload::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAICharacterController* AICon = Cast<AAICharacterController>(OwnerComp.GetAIOwner());
	if (!AICon) return false;
	
	AAICharacter* AICharacter = Cast<AAICharacter>(AICon->GetPawn());
	if (!AICharacter) return false;

	if (auto* Weapon = AICharacter->WeaponInventory->CurrentWeapon)
	{
		return Weapon->AmmoHandler->GetCurrentMagPercent() <= AmmoPercent;
	}
	return false;
}
