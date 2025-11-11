// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"


UENUM(BlueprintType)
enum class ELookRigBehaviour : uint8
{
	CharacterForwardVector,
	TargetPointLocation,
	TargetActorLocation,
	TargetActorForwardVector,
	TargetComponentForwardVector,
};

UENUM(BlueprintType)
enum class EMovementDirection : uint8
{
	N,
	NE,
	E,
	SE,
	S,
	SW,
	W,
	NW
};
/**
 * 
 */
UCLASS()
class MYPROJECT_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")	
	bool HasRifle = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")	
	bool IsCrouching = false;

	void OnMovement(const FVector2D& Value);
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")	
	EMovementDirection MovementDirection;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")	
	float MovementSpeed;

	UFUNCTION(BlueprintPure, Category = "Animation")
	FVector GetLookRigLocation() const;

	UFUNCTION(BlueprintPure, Category = "Animation")
	bool IsRigPointBehindActor() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")	
	FVector TargetLookRigPoint;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")	
	ELookRigBehaviour LookRigBehavior = ELookRigBehaviour::CharacterForwardVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")	
	AActor* TargetLookRigObject;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")	
	USceneComponent* TargetLookRigComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")	
	float ForwardMultiplier = 1;

private:
	FVector GetForwardActorLocation(const AActor* Actor) const;
	FVector GetForwardComponentLocation(const USceneComponent* Comp) const;
	
	
};
