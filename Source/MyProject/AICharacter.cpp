// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

#include "AIController.h"
#include "CharacterAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WeaponInventory.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/Weapon.h"

AAICharacter::AAICharacter()
{
	Tags.Add("AIPlayer");
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	if (auto* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->LookRigBehavior = ELookRigBehaviour::TargetPointLocation;
		AnimInstance->TargetLookRigObject = Controller;
	}
}


void AAICharacter::MoveToRandomPoint()
{
	
}

void AAICharacter::SetConfidence()
{
	if (auto* AICont = Cast<AAIController>(GetController()))
	{
		float ConfidenceLevel = 1; // Ranged from 0 to 1; 1 is giga chad and 0 is pants pisser
		ConfidenceLevel *= GetHealthPercent();
		if (auto* Weapon = WeaponInventory->CurrentWeapon)
		{
			ConfidenceLevel *= Weapon->AmmoHandler->GetCurrentMagPercent();
		}
		
		FName KeyName = TEXT("Confidence");
		UBlackboardComponent* BlackboardComp = AICont->GetBlackboardComponent();
		BlackboardComp->SetValueAsFloat(KeyName, ConfidenceLevel);
	}
}

void AAICharacter::AIMove(const FVector2D& Value)
{
	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(FVector::ForwardVector, Value.Y);
		AddMovementInput(FVector::RightVector, Value.X);
	}
}

void AAICharacter::LookAtPoint(const FVector& TargetPoint, bool bUsePitch)
{
	if (Controller == nullptr) return;

	if (auto* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->LookRigBehavior = ELookRigBehaviour::TargetPointLocation;
		if (!bUsePitch)
		{
			FVector PitchlessPoint = TargetPoint;
			PitchlessPoint.Z = 0; 
			AnimInstance->TargetLookRigPoint = PitchlessPoint;
		}
		else
		{
			AnimInstance->TargetLookRigPoint = TargetPoint;
		}
	}
	
	// Create Look rotation
	FVector ToTarget = TargetPoint - GetActorLocation();
	ToTarget.Normalize();
	FRotator LookAtRotation = ToTarget.Rotation();
	if (!bUsePitch) LookAtRotation.Pitch = 0.f;
	LookAtRotation.Pitch = FMath::Clamp(LookAtRotation.Pitch, -89.0f, 89.0f);

	// Set controller rotation
	Controller->SetControlRotation(LookAtRotation);

	// --- IK rig / character body catchup check ---
	FRotator CharacterRotation = GetActorRotation();
	float NormalizedYaw = FMath::UnwindDegrees(LookAtRotation.Yaw);
	float NormalizedCharYaw = FMath::UnwindDegrees(CharacterRotation.Yaw);

	float YawDifference = FMath::Abs(FMath::FindDeltaAngleDegrees(NormalizedCharYaw, NormalizedYaw));
	if (YawDifference > IKRigYawFreedom)
	{
		DesiredRotation = FRotator(0.f, NormalizedYaw, 0.f);
		bShouldRotateToCamera = true;
	}
}

