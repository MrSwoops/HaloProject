// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_PreCombatBehavior.h"

#include "MyProject/AI/AICharacter.h"
#include "MyProject/AI/AICharacterController.h"

FString UBTD_PreCombatBehavior::GetStaticDescription() const
{
	FString Desc;
	FString BehaviorString = StaticEnum<EDefaultAIBehavior>()->GetNameStringByValue(static_cast<int64>(PreCombatBehavior));
	Desc += FString::Printf(TEXT("Behavior: %s\n"), *BehaviorString);
	return Desc;
}

bool UBTD_PreCombatBehavior::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAICharacterController* AICon = Cast<AAICharacterController>(OwnerComp.GetAIOwner());
	if (!AICon) return false;
	
	AAICharacter* AICharacter = Cast<AAICharacter>(AICon->GetPawn());
	if (!AICharacter) return false;

	if (AICharacter->DefaultBehavior == PreCombatBehavior) return true;

	return false;
}
