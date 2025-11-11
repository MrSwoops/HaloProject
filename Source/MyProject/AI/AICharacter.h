// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThreatTable.h"
#include "MyProject/GameplayCharacter.h"
#include "AICharacter.generated.h"

enum class ELookRigBehaviour : uint8;

UENUM(BlueprintType)
enum class EDefaultAIBehavior : uint8
{
	RandomMovement,
	FollowPath,
	Stationary,
};
/**
 * 
 */
UCLASS()
class MYPROJECT_API AAICharacter : public AGameplayCharacter
{
	GENERATED_BODY()

public:

	AAICharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FThreatTable ThreatTable = FThreatTable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDefaultAIBehavior DefaultBehavior;

	UFUNCTION(BlueprintCallable)
	virtual void AIMove(const FVector2D& Value);

	UFUNCTION(BlueprintCallable)
	bool TryFireWeapon(float HoldTime = -1);
	
	void MoveToRandomPoint();

	void SetConfidence();
	UFUNCTION(BlueprintCallable)
	void JumpToDestination(FVector Destination);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void LookAtPoint(const FVector& TargetPoint, bool bUsePitch);
	UFUNCTION(BlueprintCallable)
	void LookAtActor(AActor* TargetActor, bool bUsePitch);
private:
	void UpdateCharacterLookRotation();
	bool UsePitch = false;
	ELookRigBehaviour* LookRigBehavior;
	FVector CachedTargetPoint;
	AActor* CachedTargetActor;
};
