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
