// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"
#include "../GameplayCharacter.h"
#include "../CustomGameMode.h"
#include "../BulletPoolManager.h"
#include "TimerManager.h"
#include "../UI/WeaponUIWidget.h"
#include "Engine/World.h"

// void AShotgun::Fire()
// {
// 	if (!Racked || CurrentMagAmmo <= 0 || Character == nullptr || Character->GetController() == nullptr)
// 	{
// 		return;
// 	}
//
// 	// Try and fire a projectile
// 	
// 	FTimerHandle TimerHandle;
// 	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { Racked = true; }, FireRate, false);
// }

void AShotgun::ShootBullet()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		Racked = false;
		CurrentMagAmmo--;
		WeaponUI->UpdateAmmoUI(CurrentMagAmmo);
		const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

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
			
			GameMode->BulletPoolManager->SpawnBullet(SpawnLocation, SpreadRotation, WeaponType);
			//DrawDebugLine(World, SpawnLocation, SpawnLocation + RandomDirection * 1000.0f, FColor::Red, false, 1.0f, 0, 1.0f);
		}
	}
}
