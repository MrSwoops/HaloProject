// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_UpdateThreatTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyProject/AI/AICharacter.h"
#include "MyProject/AI/AICharacterController.h"

UBTS_UpdateThreatTarget::UBTS_UpdateThreatTarget()
{
	NodeName = "Update Threat Target";
	//bNotifyBecomeRelevant = true;
}


void UBTS_UpdateThreatTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAICharacterController* AICon = Cast<AAICharacterController>(OwnerComp.GetAIOwner());
	if (!AICon) return;
	
	AAICharacter* AICharacter = Cast<AAICharacter>(AICon->GetPawn());
	if (!AICharacter) return;
	
	if (auto* Character = AICharacter->ThreatTable.GetHighestThreatTarget())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(OutTargetKey.SelectedKeyName, Character);
	}
}
