// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterController.h"

#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Navigation/PathFollowingComponent.h"

void AAICharacterController::BeginPlay()
{
	Super::BeginPlay();

	// Start the random movement loop
	RandomMovePoint();
}

void AAICharacterController::RandomMovePoint()
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	FVector Origin = ControlledPawn->GetActorLocation();

	// Get a random point in the nav mesh
	FNavLocation RandomLocation;
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (NavSystem && NavSystem->GetRandomReachablePointInRadius(Origin, MoveRadius, RandomLocation))
	{
		MoveToRandomLocation(RandomLocation.Location);
	}
	else
	{
		// Retry after a short delay if no point was found
		GetWorld()->GetTimerManager().SetTimer(MoveRetryTimerHandle, this, &AAICharacterController::RandomMovePoint, DelayOnFail, false);
	}
}

void AAICharacterController::MoveToRandomLocation(const FVector& Destination)
{
	FNavPathSharedPtr NavPath;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(Destination);
	MoveRequest.SetAcceptanceRadius(5.0f);
	MoveRequest.SetCanStrafe(true);

	FPathFollowingRequestResult MoveResult = MoveTo(MoveRequest);

	// Use .Code to access the enum result
	float Delay = (MoveResult.Code == EPathFollowingRequestResult::Failed) ? DelayOnFail : DelayOnSuccess;
	GetWorld()->GetTimerManager().SetTimer(MoveRetryTimerHandle, this, &AAICharacterController::RandomMovePoint, Delay, false);
}