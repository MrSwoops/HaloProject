// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AICharacterController.generated.h"

struct FAIStimulus;
/**
 * 
 */
UCLASS()
class MYPROJECT_API AAICharacterController : public AAIController
{
	GENERATED_BODY()
public:

	AAICharacterController();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* AIPerception;

	UFUNCTION()
	void OnTargetPerceptionUpdatedCallback(AActor* TargetActor, FAIStimulus Stimulus);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float HealthWeight = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AmmoWeight = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DistanceWeight = 1.0f;
	UFUNCTION(BlueprintCallable)
	void UpdateConfidence();
private:
	FTimerHandle ConfidenceCheckTimerHandle;
	UPROPERTY(EditDefaultsOnly)
	float ConfidenceCheckTimer = 1.0f;
protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	// static const FName HasLOSKeyName;
	// static const FName TargetEnemyKeyName;
	// static const FName LastKnownLocationKeyName;



public:
	UFUNCTION()
	void RandomMovePoint();
private:
	FTimerHandle MoveRetryTimerHandle;
	float MoveRadius = 1000.f;
	float DelayOnSuccess = 5.0f;
	float DelayOnFail = 0.2f;

	void MoveToRandomLocation(const FVector& Destination);
	
};
