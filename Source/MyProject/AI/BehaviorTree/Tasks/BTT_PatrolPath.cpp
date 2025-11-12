#include "BTT_PatrolPath.h"

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MoveToTarget.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Components/SplineComponent.h"
#include "MyProject/AI/AICharacter.h"
#include "MyProject/AI/AICharacterController.h"
#include "MyProject/AI/PatrolPath.h"

UBTT_PatrolPath::UBTT_PatrolPath()
{
	NodeName = "Patrol Path";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_PatrolPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICharacterController* AICon = Cast<AAICharacterController>(OwnerComp.GetAIOwner());
	if (!AICon) return EBTNodeResult::Failed;
	
	AAICharacter* AICharacter = Cast<AAICharacter>(AICon->GetPawn());
	if (!AICharacter) return EBTNodeResult::Failed;

	APatrolPath* PatrolPathActor = AICharacter->PatrolPath;
	if (!PatrolPathActor || PatrolPathActor->SplinePath->GetNumberOfSplinePoints() <= 1) return EBTNodeResult::Failed;

	Loops = PatrolPathActor->SplinePath->IsClosedLoop();

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	CurrentPath.Empty();
	CurrentIndex = 0;
	for (const FVector& Point : PatrolPathActor->GetPathPoints()) // Build proper path from patrol points
	{
		FNavLocation OutLocation;
		NavSys->ProjectPointToNavigation(Point, OutLocation);
		CurrentPath.Add(OutLocation.Location);
		
	}
	return EBTNodeResult::InProgress;
}

void UBTT_PatrolPath::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAICharacterController* AICon = Cast<AAICharacterController>(OwnerComp.GetAIOwner());
	if (!AICon)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	AAICharacter* AICharacter = Cast<AAICharacter>(AICon->GetPawn());
	if (!AICharacter)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	FVector CharacterLocation = AICharacter->GetActorLocation();

	// Check if reached goal
	if (!Loops)
	{
		FVector DestinationPoint = CurrentPath[CurrentPath.Num() - 1];
		float DistanceToDestination = (DestinationPoint - CharacterLocation).Length();
		if (DistanceToDestination <= AcceptableGoalRadius)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}

	// Check if stuck
	if (CharacterLocation.Equals(PreviousLocation, StuckPrecision)) 
	{
		StuckTime += DeltaSeconds;
		if (StuckTime >= TimeUntilStuck)
		{
			StuckTime = 0;
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}
	}
	else StuckTime = 0;

	FVector CurrentTraversalPoint = CurrentPath[CurrentIndex];
	FVector MovementDirection = CurrentTraversalPoint - CharacterLocation;
	
	if (MovementDirection.Length() > PathFollowLenience) // Move to Current Traversal Point
	{
		AICharacter->Move(GetMovementInput(AICharacter, MovementDirection));
		PreviousLocation = CharacterLocation;
		return;
	}
	if (CurrentIndex >= CurrentPath.Num() - 1) // End of path
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// Update and Move to Current Traversal Point
	CurrentIndex++;
	CurrentTraversalPoint = CurrentPath[CurrentIndex];
	MovementDirection = CurrentTraversalPoint - CharacterLocation;
	if (LookAlongPath) AICharacter->LookAtPoint(CurrentTraversalPoint, UsePitch);
	AICharacter->Move(GetMovementInput(AICharacter, MovementDirection));
	PreviousLocation = CharacterLocation;
	return;
}

FVector2D UBTT_PatrolPath::GetMovementInput(AAICharacter* Char, FVector Direction)
{
	FVector2D Input;
	Input.X = FVector::DotProduct(Direction, Char->GetActorRightVector());
	Input.Y = FVector::DotProduct(Direction, Char->GetActorForwardVector());

	return Input.GetSafeNormal() * MoveSpeed;
}
