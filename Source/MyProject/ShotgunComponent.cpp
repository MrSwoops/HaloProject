// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunComponent.h"
#include "MyProjectCharacter.h"
#include "CustomGameMode.h"
#include "ActorPool.h"
#include "TimerManager.h"
#include "WeaponUIWidget.h"
#include "Engine/World.h"

void UShotgunComponent::Fire()
{
	if (!Racked || CurrentMagAmmo <= 0 || Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		Racked = false;
		CurrentMagAmmo--;
		WeaponUI->UpdateAmmoUI(CurrentMagAmmo);
		const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// Spawn the projectile at the muzzle
		for (int i = 0; i < PelletsPerShot; i++)
		{
			FVector ForwardVector = SpawnRotation.Vector(); // Converts rotation to direction vector
			float ConeHalfAngleRad = FMath::DegreesToRadians(Spread); // Spread is an angle in radians. Convert degrees if needed:
			FVector RandomDirection = FMath::VRandCone(ForwardVector, ConeHalfAngleRad);
			FRotator SpreadRotation = RandomDirection.Rotation(); // Get new rotation from direction
			
			//GameMode->BulletPool->SpawnFromLocationAndRotation(SpawnLocation, SpreadRotation);
			//DrawDebugLine(World, SpawnLocation, SpawnLocation + RandomDirection * 1000.0f, FColor::Red, false, 1.0f, 0, 1.0f);
		}
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UShotgunComponent::FinishedRacking, FireRate, false);
	}
}

void UShotgunComponent::FinishedRacking()
{
	Racked = true;
}