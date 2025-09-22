// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

#include "AIController.h"
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
			ConfidenceLevel *= Weapon->GetCurrentMagPercent();
		}
		
		FName KeyName = TEXT("Confidence");
		UBlackboardComponent* BlackboardComp = AICont->GetBlackboardComponent();
		BlackboardComp->SetValueAsFloat(KeyName, ConfidenceLevel);
	}
}
