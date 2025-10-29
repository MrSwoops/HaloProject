// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCharacter.h"
#include "AICharacter.generated.h"

enum class ELookRigBehaviour : uint8;
/**
 * 
 */
UCLASS()
class MYPROJECT_API AAICharacter : public AGameplayCharacter
{
	GENERATED_BODY()

public:

	AAICharacter();

	UFUNCTION(BlueprintCallable)
	virtual void AIMove(const FVector2D& Value);

	UFUNCTION(BlueprintCallable)
	bool TryFireWeapon(float HoldTime = -1);
	
	void MoveToRandomPoint();

	void SetConfidence();

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
