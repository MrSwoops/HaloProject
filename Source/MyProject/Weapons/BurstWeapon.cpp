// Fill out your copyright notice in the Description page of Project Settings.


#include "BurstWeapon.h"
#include "../GameplayCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject/PlayerCharacter.h"

void ABurstWeapon::Fire()
{
	if (!Racked || GetWorld()->GetTimerManager().IsTimerActive(ReloadTimerHandle) || Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}
	if (CurrentMagAmmo <= 0)
	{
		if (CurrentReserveAmmo <= 0)
		{
			if (DryFireSound) UGameplayStatics::PlaySoundAtLocation(this, DryFireSound, Character->GetActorLocation());
			return;
		}
		Reload();
		return;
	}
	CurrentBurst = 0;
	ShootBullet();
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { Racked = true; }, FireRate, false);
}

void ABurstWeapon::ShootBullet()
{
	Super::ShootBullet();

	if (FireSounds.Num() > 0)
	{
		int32 RandomSoundIndex = FMath::RandRange(0, FireSounds.Num() - 1);
		USoundBase* FireSound = FireSounds[RandomSoundIndex];
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		if (UAnimInstance* AnimInstance = Cast<APlayerCharacter>(Character)->GetMesh1P()->GetAnimInstance()) // Get the animation object for the arms mesh
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
	
	CurrentBurst++;
	if (CurrentBurst >= BurstShots || CurrentMagAmmo <= 0) return;
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABurstWeapon::ShootBullet, BurstRate, false);
}

