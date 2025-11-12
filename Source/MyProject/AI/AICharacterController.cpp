// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterController.h"

#include "MyProject/GameplayCharacter.h"
#include "NavigationSystem.h"
#include "TimerManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyProject/Components/WeaponInventory.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "MyProject/AI/BaseAIBlackboardKeyNames.h"
#include "MyProject/Weapons/Weapon.h"


AAICharacterController::AAICharacterController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	UE_LOG(LogTemp, Warning, TEXT("Constructed controller %s with AIPerception=%p"), *GetName(), AIPerception);
	
}


void AAICharacterController::BeginPlay()
{
	Super::BeginPlay();
	//AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AAICharacterController::OnTargetPerceptionUpdatedCallback);
	if (BehaviorTree) RunBehaviorTree(BehaviorTree);
	// Start the random movement loop
	//RandomMovePoint();
	//GetWorldTimerManager().SetTimer(ConfidenceCheckTimerHandle, this, &AAICharacterController::UpdateConfidence, ConfidenceCheckTimer, true);
}

void AAICharacterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//if (AIPerception)
	{
		AIPerception->OnTargetPerceptionUpdated.RemoveDynamic(this, &AAICharacterController::OnTargetPerceptionUpdatedCallback);
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AAICharacterController::OnTargetPerceptionUpdatedCallback);
	}
}


void AAICharacterController::OnTargetPerceptionUpdatedCallback(AActor* TargetActor, FAIStimulus Stimulus)
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "UpdatePerception");
	AGameplayCharacter* SensedCharacter = Cast<AGameplayCharacter>(TargetActor);
	AGameplayCharacter* OwnerCharacter = Cast<AGameplayCharacter>(GetPawn());
	if (SensedCharacter == nullptr || OwnerCharacter == nullptr || OwnerCharacter->Team == SensedCharacter->Team) return;
	//if (SensedCharacter == nullptr){if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "NoSensedCharacter"); return;}
	//if (OwnerCharacter == nullptr) {if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "NoOwnerCharacter");return;}
	//if (OwnerCharacter->Team == SensedCharacter->Team){ if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "SameTeam");return;}
	
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	
	if (Stimulus.WasSuccessfullySensed())
	{
		BlackboardComp->SetValueAsBool(HasLOSKeyName, true);
		BlackboardComp->SetValueAsObject(TargetEnemyKeyName, SensedCharacter);
		OwnerCharacter->EnterCombat();
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Sensed");
	}
	else
	{
		BlackboardComp->SetValueAsVector(LastKnownLocationKeyName, SensedCharacter->GetActorLocation());
		BlackboardComp->SetValueAsBool(HasLOSKeyName, false);
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "LostSense");
	}
}


void AAICharacterController::UpdateConfidence()
{
	float ConfidenceLevel = 1; // Ranged from 0 to 1; 1 is giga chad, 0 is pants pisser
	if (AGameplayCharacter* GCharacter = Cast<AGameplayCharacter>(GetPawn()))
	{
		ConfidenceLevel *= FMath::Pow(GCharacter->GetHealthPercent(), HealthWeight);
		
		if (auto* Weapon = GCharacter->WeaponInventory->CurrentWeapon)
		{
			ConfidenceLevel *= FMath::Pow(Weapon->AmmoHandler->GetCurrentMagPercent(), AmmoWeight); 
		}
	}
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	BlackboardComp->SetValueAsFloat(ConfidenceKeyName, ConfidenceLevel);
}
// ConfidenceLevel = 1 * (HealthPercent^HealthWeight * AmmoPercent^AmmoWeight * TargetDistancePercent^DistanceWeight);

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