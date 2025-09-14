// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "MyProject/BulletPoolManager.h"
#include "MyProject/CharacterAnimInstance.h"
#include "MyProject/CustomGameMode.h"
#include "MyProject/FirstPersonAnimInstance.h"
#include "MyProject/MyProjectPickUpComponent.h"
#include "MyProject/PlayerCharacter.h"
#include "MyProject/UI/EnergyWeaponUIWidget.h"
#include "MyProject/UI/WeaponUIWidget.h"

void AEnergyWeapon::BeginPlay()
{
	Super::Super::BeginPlay();
	GameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	PickUpComp->AttachedWeapon = this;
	CurrentMagAmmo = MaxMagSize;
	PickUpComp->OnPickUp.AddDynamic(this, &AWeapon::AttachWeapon);
	//SkeletalMeshComp->SetCollisionProfileName(FName("DroppedWeapon"));
}

void AEnergyWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentHeatLevel > 0)
	{
		CurrentHeatLevel -= HeatDissipationSpeed * DeltaSeconds;
		if (WeaponUI) WeaponUI->UpdateAmmoUI(CurrentHeatLevel / MaxHeatLevel);
		if (CurrentHeatLevel <= 0)
		{
			CurrentHeatLevel = 0;
			IsOverHeated = false;
		} 
	}
}

void AEnergyWeapon::Fire()
{
	if (!Racked || IsOverHeated || GetWorld()->GetTimerManager().IsTimerActive(ReloadTimerHandle) || Character == nullptr || Character->GetController() == nullptr)
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
	}

	ShootBullet();
	
	if (FireSounds.Num() > 0)
	{
		int32 RandomSoundIndex = FMath::RandRange(0, FireSounds.Num() - 1);
		USoundBase* FireSound = FireSounds[RandomSoundIndex];
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	if (IsPlayerOwned && FireAnimation != nullptr)
	{
		if (UAnimInstance* AnimInstance = Cast<APlayerCharacter>(Character)->GetMesh1P()->GetAnimInstance()) // Get the animation object for the arms mesh
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { Racked = true; }, FireRate, false);
}

void AEnergyWeapon::ShootBullet()
{
	// Try and fire a projectile
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		Racked = false;
		CurrentMagAmmo--;
		CurrentHeatLevel += HeatBuildupPerShot;
		if (CurrentHeatLevel >= MaxHeatLevel) SetOverheated();
		if (WeaponUI)
		{
			WeaponUI->UpdateAmmoUI(CurrentHeatLevel / MaxHeatLevel);
			WeaponUI->SetReserveText((static_cast<float>(CurrentMagAmmo) / static_cast<float>(MaxMagSize)) * 100.0f);
		}
		
		const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		FVector ForwardVector = SpawnRotation.Vector(); // Converts rotation to direction vector
		float ConeHalfAngleRad = FMath::DegreesToRadians(Spread); // Spread is an angle in radians. Convert degrees if needed:
		FVector RandomDirection = FMath::VRandCone(ForwardVector, ConeHalfAngleRad);
		FRotator SpreadRotation = RandomDirection.Rotation(); // Get new rotation from direction
		
		GameMode->BulletPoolManager->SpawnBullet(SpawnLocation, SpreadRotation, WeaponType);
		//DrawDebugLine(World, SpawnLocation, SpawnLocation + RandomDirection * 1000.0f, FColor::Red, false, 1.0f, 0, 1.0f);
	}
}

void AEnergyWeapon::Reload()
{
	SetOverheated();
}


void AEnergyWeapon::SetOverheated()
{
	CurrentHeatLevel = MaxHeatLevel;
	IsOverHeated = true;
}

void AEnergyWeapon::AttachWeapon(AGameplayCharacter* TargetCharacter)
{
	if (!TargetCharacter) return;
	Character = TargetCharacter;

	SkeletalMeshComp->SetSimulatePhysics(false);
	SkeletalMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Character->PickUpWeapon(this);
	
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;//(, true);
	AttachmentRules.bWeldSimulatedBodies = true;
	if (APlayerCharacter* PC = Cast<APlayerCharacter>(TargetCharacter))
	{
		IsPlayerOwned = true;
		WeaponUI = CreateWidget<UEnergyWeaponUIWidget>(Cast<APlayerController>(PC->GetController()), WeaponUIClass);
		if (WeaponUI)
		{
			WeaponUI->InitializeWeaponUI(CurrentHeatLevel / MaxHeatLevel, (CurrentMagAmmo / MaxMagSize) * 100.0f);
		}
		AttachToComponent(PC->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
		Cast<UFirstPersonAnimInstance>(PC->GetMesh1P()->GetAnimInstance())->HasRifle = true;
	}
	else
	{
		AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("hand_rSocket")));
	}
	Cast<UCharacterAnimInstance>(TargetCharacter->GetMesh()->GetAnimInstance())->HasRifle = true;
}
