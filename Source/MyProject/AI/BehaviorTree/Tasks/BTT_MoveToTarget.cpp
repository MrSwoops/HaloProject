// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MoveToTarget.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "MyProject/AI/AICharacter.h"
#include "MyProject/AI/AICharacterController.h"

UBTT_MoveToTarget::UBTT_MoveToTarget()
{
	NodeName = "Follow Path";
	bNotifyTick = true;
	
	InTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_MoveToTarget, InTargetKey), AActor::StaticClass());
	InTargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_MoveToTarget, InTargetKey));
}

EBTNodeResult::Type UBTT_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICharacterController* AICon = Cast<AAICharacterController>(OwnerComp.GetAIOwner());
	if (!AICon) return EBTNodeResult::Failed;
	
	AAICharacter* AICharacter = Cast<AAICharacter>(AICon->GetPawn());
	if (!AICharacter) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	CurrentPath.Empty();
	CurrentIndex = 0;
	if (InTargetKey.SelectedKeyType && InTargetKey.SelectedKeyType->IsChildOf(UBlackboardKeyType_Object::StaticClass())) // Path to Actor
	{
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetKey.SelectedKeyName));
		FPathFindingQuery Query;
		Query.StartLocation = AICharacter->GetActorLocation();
		Query.EndLocation = TargetActor->GetActorLocation();
		Query.NavAgentProperties = AICharacter->GetNavAgentPropertiesRef();
		Query.Owner = AICharacter;

		FPathFindingResult Result = NavSys->FindPathSync(Query, EPathFindingMode::Regular);

		if (Result.IsSuccessful())
		{
			const TArray<FNavPathPoint>& FullPoints = Result.Path->GetPathPoints();
			for (const FNavPathPoint& P : FullPoints)
				CurrentPath.Add(P.Location);
			if (LookAtTarget) AICharacter->LookAtActor(TargetActor, UsePitch);
			return EBTNodeResult::InProgress;
		}
		if (auto* BuiltPath = NavSys->FindPathToActorSynchronously(AICharacter, AICharacter->GetActorLocation(), TargetActor))
		{
			if (BuiltPath->PathPoints.Num() > 0)
			{
				const TArray<FNavPathPoint>& FullPoints = BuiltPath->GetPath()->GetPathPoints();

				for (const FNavPathPoint& P : FullPoints)
				{
					CurrentPath.Add(P.Location);
				}
				if (LookAtTarget) AICharacter->LookAtActor(TargetActor, UsePitch);
				return EBTNodeResult::InProgress;
			}
		}
	}
	else // Path to Location
	{
		FVector TargetPoint = OwnerComp.GetBlackboardComponent()->GetValueAsVector(InTargetKey.SelectedKeyName);
		FNavLocation TargetLocation;
		NavSys->ProjectPointToNavigation(TargetPoint, TargetLocation);
		FPathFindingQuery Query;
		Query.StartLocation = AICharacter->GetActorLocation();
		Query.EndLocation = TargetLocation;
		Query.NavAgentProperties = AICharacter->GetNavAgentPropertiesRef();
		Query.Owner = AICharacter;

		FPathFindingResult Result = NavSys->FindPathSync(Query, EPathFindingMode::Regular);

		if (Result.IsSuccessful())
		{
			const TArray<FNavPathPoint>& FullPoints = Result.Path->GetPathPoints();
			for (const FNavPathPoint& P : FullPoints)
				CurrentPath.Add(P.Location);
			if (LookAtTarget) AICharacter->LookAtPoint(TargetLocation.Location, UsePitch);
			return EBTNodeResult::InProgress;
		}
		if (auto* BuiltPath = NavSys->FindPathToLocationSynchronously(AICharacter, AICharacter->GetActorLocation(), TargetLocation.Location))
		{
			if (BuiltPath->PathPoints.Num() > 0)
			{
				const TArray<FNavPathPoint>& FullPoints = BuiltPath->GetPath()->GetPathPoints();

				for (const FNavPathPoint& P : FullPoints) CurrentPath.Add(P.Location); 
				if (LookAtTarget) AICharacter->LookAtPoint(TargetLocation.Location, UsePitch);
				return EBTNodeResult::InProgress;
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UBTT_MoveToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
	FVector DestinationPoint = CurrentPath[CurrentPath.Num() - 1];
	float DistanceToDestination = (DestinationPoint - CharacterLocation).Length();
	if (DistanceToDestination <= AcceptableGoalRadius)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
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

FVector2D UBTT_MoveToTarget::GetMovementInput(AAICharacter* Char, FVector Direction)
{
	FVector2D Input;
	Input.X = FVector::DotProduct(Direction, Char->GetActorRightVector());
	Input.Y = FVector::DotProduct(Direction, Char->GetActorForwardVector());

	return Input.GetSafeNormal() * MoveSpeed;
}
