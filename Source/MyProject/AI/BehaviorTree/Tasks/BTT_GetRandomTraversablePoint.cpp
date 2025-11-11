// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GetRandomTraversablePoint.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

FString UBTT_GetRandomTraversablePoint::GetStaticDescription() const
{
	FString Desc;

	Desc += FString::Printf(TEXT("Search Radius: %.1f\n"), SearchRadius);
	Desc += FString::Printf(TEXT("Target Move Location: %s\n"), *OutPointKey.SelectedKeyName.ToString());
	Desc += FString::Printf(TEXT("Use Self as Origin: %s"), UseSelfAsOrigin ? TEXT("True") : TEXT("False"));

	// Only show the InPointKey if we're using external origin
	if (!UseSelfAsOrigin)
	{
		Desc += FString::Printf(TEXT("\nOrigin Point: %s"), *OriginKey.SelectedKeyName.ToString());
	}

	return Desc;
}

EBTNodeResult::Type UBTT_GetRandomTraversablePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;
	APawn* AIPawn = AICon->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	FVector Origin;
	if (UseSelfAsOrigin)
		Origin = AIPawn->GetActorLocation();
	else
	{
		if (OriginKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
			Origin = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(OriginKey.SelectedKeyName))->GetActorLocation();
		else if (OriginKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
			Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(OriginKey.SelectedKeyName);
		else
			Origin = AIPawn->GetActorLocation();
	}
	FNavLocation RandomLocation;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys && NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, RandomLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(OutPointKey.SelectedKeyName, RandomLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
