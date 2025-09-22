// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AICharacterController.generated.h"

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAIPerceptionComponent* AIPerception;
	
	void UpdateConfidence();
private:
	FTimerHandle ConfidenceCheckTimerHandle;
	UPROPERTY(EditDefaultsOnly)
	float ConfidenceCheckTimer = 1.0f;



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
