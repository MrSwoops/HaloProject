// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

FVector UCharacterAnimInstance::GetLookRigLocation() const
{
	switch (LookRigBehavior)
	{
		case ELookRigBehaviour::CharacterForwardVector:
			return GetForwardActorLocation(GetOwningActor());
			
		case ELookRigBehaviour::TargetActorLocation:
			if (TargetLookRigObject) return TargetLookRigObject->GetActorLocation();
			break;
			
		case ELookRigBehaviour::TargetPointLocation:
			return TargetLookRigPoint;

		case ELookRigBehaviour::TargetActorForwardVector:
			if (TargetLookRigObject) return GetForwardActorLocation(TargetLookRigObject);
			break;
		case ELookRigBehaviour::TargetComponentForwardVector:
			if (TargetLookRigComponent) return GetForwardComponentLocation(TargetLookRigComponent);
			break;
		
		default:
			return GetForwardActorLocation(GetOwningActor());
	};
	return GetForwardActorLocation(GetOwningActor());
}

FVector UCharacterAnimInstance::GetForwardActorLocation(const AActor* Actor) const
{
	FVector Forward = FVector::ZeroVector;
	if (Actor)
		Forward = Actor->GetActorLocation() + (Actor->GetActorForwardVector() * ForwardMultiplier);
	return Forward;
}

FVector UCharacterAnimInstance::GetForwardComponentLocation(const USceneComponent* Comp) const
{
	FVector Forward = FVector::ZeroVector;
	if (Comp)
		Forward = Comp->GetComponentLocation() + (Comp->GetForwardVector() * ForwardMultiplier);
	return Forward;
}

bool UCharacterAnimInstance::IsRigPointBehindActor() const
{
	FVector TargetDirection = (TargetLookRigPoint - GetOwningActor()->GetActorLocation()).GetSafeNormal();
	float DotProduct = FVector::DotProduct(GetOwningActor()->GetActorForwardVector(), TargetDirection);
	return (DotProduct < 0.5f);
}

void UCharacterAnimInstance::OnMovement(const FVector2D& Value)
{
	float Angle = FMath::Atan2(Value.Y, Value.X);
	float AngleDegrees = FMath::RadiansToDegrees(Angle);
	AngleDegrees -= 90.0f; // Unreal needs to be extra and can't use standard euclidean defaults
	if (AngleDegrees < 0)
	{
		AngleDegrees += 360;
	}
	EMovementDirection Direction;
	if (AngleDegrees >= 337.5 || AngleDegrees < 22.5)
	{
		Direction = EMovementDirection::N; // North
	}
	else if (AngleDegrees >= 22.5 && AngleDegrees < 67.5)
	{
		Direction = EMovementDirection::NE; // North-East
	}
	else if (AngleDegrees >= 67.5 && AngleDegrees < 112.5)
	{
		Direction = EMovementDirection::E; // East
	}
	else if (AngleDegrees >= 112.5 && AngleDegrees < 157.5)
	{
		Direction = EMovementDirection::SE; // South-East
	}
	else if (AngleDegrees >= 157.5 && AngleDegrees < 202.5)
	{
		Direction = EMovementDirection::S; // South
	}
	else if (AngleDegrees >= 202.5 && AngleDegrees < 247.5)
	{
		Direction = EMovementDirection::SW; // South-West
	}
	else if (AngleDegrees >= 247.5 && AngleDegrees < 292.5)
	{
		Direction = EMovementDirection::W; // West
	}
	else// (AngleDegrees >= 292.5 && AngleDegrees < 337.5)
	{
		Direction = EMovementDirection::NW; // North-West
	}

	MovementDirection = Direction;
	MovementSpeed = Value.Length();
}
