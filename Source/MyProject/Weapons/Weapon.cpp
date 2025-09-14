// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "../MyProjectPickUpComponent.h"
#include "../GameplayCharacter.h"
#include "../BulletPoolManager.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "../CustomGameMode.h"
#include "EnhancedInputComponent.h"
#include "../FirstPersonAnimInstance.h"
#include "../UI/WeaponUIWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "MyProject/CharacterAnimInstance.h"
#include "MyProject/PlayerCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeleMesh"));
	SkeletalMeshComp->SetSimulatePhysics(true);
	SkeletalMeshComp->SetCollisionProfileName(FName("DroppedWeapon"));

	RootComponent = SkeletalMeshComp;

	PickUpComp = CreateDefaultSubobject<UMyProjectPickUpComponent>(TEXT("Interaction"));
	PickUpComp->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	PickUpComp->AttachedWeapon = this;
	CurrentMagAmmo = MaxMagSize;
	PickUpComp->OnPickUp.AddDynamic(this, &AWeapon::AttachWeapon);
	CurrentReserveAmmo = (MaxReserveMags - 1) * MaxMagSize;
	//SkeletalMeshComp->SetCollisionProfileName(FName("DroppedWeapon"));
}


void AWeapon::Fire()
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

void AWeapon::ShootBullet()
{
	// Try and fire a projectile
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		Racked = false;
		CurrentMagAmmo--;
		if (WeaponUI) WeaponUI->UpdateAmmoUI(CurrentMagAmmo);
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

void AWeapon::Reload()
{
	if (CurrentMagAmmo == MaxMagSize || CurrentReserveAmmo <= 0) return;
	if (ReloadAnimation != nullptr)
	{
		if (UAnimInstance* AnimInstance = Cast<APlayerCharacter>(Character)->GetMesh1P()->GetAnimInstance()) // Get the animation object for the arms mesh
		{
			AnimInstance->Montage_Play(ReloadAnimation, 1.f, EMontagePlayReturnType::MontageLength, 0.f, true);
		}
	}
	if (ReloadSounds.Num() > 0)
	{
		int32 RandomSoundIndex = FMath::RandRange(0, ReloadSounds.Num() - 1);
		USoundBase* FireSound = ReloadSounds[RandomSoundIndex];
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
		
	}
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeapon::ReloadMag, (ReloadAnimation) ? ReloadAnimation->GetPlayLength() : 1.0f, false);
}
void AWeapon::ReloadMag()
{
	int32 ammoNeeded = MaxMagSize - CurrentMagAmmo;
	if (ammoNeeded > CurrentReserveAmmo) ammoNeeded = CurrentReserveAmmo;
	CurrentReserveAmmo -= ammoNeeded;
	CurrentMagAmmo += ammoNeeded;
	if (WeaponUI)
	{
		WeaponUI->UpdateAmmoUI(CurrentMagAmmo);
		WeaponUI->SetReserveText(CurrentReserveAmmo);
	}
}

void AWeapon::Melee()
{
	if (MeleeSounds.Num() > 0)
	{
		int32 RandomSoundIndex = FMath::RandRange(0, MeleeSounds.Num() - 1);
		USoundBase* FireSound = MeleeSounds[RandomSoundIndex];
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
		
	}
	if (UAnimInstance* AnimInstance = Cast<APlayerCharacter>(Character)->GetMesh1P()->GetAnimInstance()) // Get the animation object for the arms mesh
	{
		if (FPMeleeAnimations.Num() <= 0) return;
		UAnimMontage* RandomMeleeAnim = FPMeleeAnimations[FMath::RandRange(0, FPMeleeAnimations.Num() - 1)];
		AnimInstance->Montage_Play(RandomMeleeAnim, 1.f, EMontagePlayReturnType::MontageLength, 0.f, true);
	}
}

void AWeapon::AttachWeapon(AGameplayCharacter* TargetCharacter)
{
	if (!TargetCharacter) return;
	Character = TargetCharacter;

	SkeletalMeshComp->SetEnableGravity(false);
	SkeletalMeshComp->SetSimulatePhysics(false);
	SkeletalMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Character->PickUpWeapon(this);
	
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;//(, true);
	AttachmentRules.bWeldSimulatedBodies = true;
	if (APlayerCharacter* PC = Cast<APlayerCharacter>(TargetCharacter))
	{
		IsPlayerOwned = true;
		WeaponUI = CreateWidget<UWeaponUIWidget>(Cast<APlayerController>(PC->GetController()), WeaponUIClass);
		if (WeaponUI)
		{
			WeaponUI->InitializeWeaponUI(CurrentMagAmmo, MaxMagSize, CurrentReserveAmmo);
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

void AWeapon::DropWeapon()
{
	if (WeaponUI) WeaponUI->RemoveFromParent();
	if (IsPlayerOwned)
	{
		if (auto* IC = Cast<UEnhancedInputComponent>(Character->GetController()))
		UnbindActions(IC);
		IsPlayerOwned = false;
	}
	
	Character = nullptr;
	GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	if (CurrentReserveAmmo <= 0 && CurrentMagAmmo <= 0)
	{
		this->Destroy();
	}
	else
	{
		SkeletalMeshComp->SetSimulatePhysics(true);
		SkeletalMeshComp->SetEnableGravity(true);
		SkeletalMeshComp->SetCollisionProfileName(FName("DroppedWeapon"));
		PickUpComp->SetCollisionProfileName(FName("Interaction"));
	}
}

void AWeapon::BindActions(UEnhancedInputComponent* InpComp)
{
	Bindings.Add(FireAction, &InpComp->BindAction(FireAction, ETriggerEvent::Triggered, this, &AWeapon::Fire));
}
void AWeapon::UnbindActions(UEnhancedInputComponent* InpComp)
{
	FEnhancedInputActionEventBinding** Temp = Bindings.Find(FireAction);
	InpComp->RemoveBinding(*(*Temp));

	Bindings.Empty();
}

void AWeapon::DisableWeapon()
{
	SkeletalMeshComp->SetVisibility(false);
	if (IsPlayerOwned) WeaponUI->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}

void AWeapon::EnableWeapon()
{
	SkeletalMeshComp->SetVisibility(true);
	if (IsPlayerOwned)
	{
		WeaponUI->SetVisibility(ESlateVisibility::Visible);
	}
}

void AWeapon::UpdateReserveAmmoUI()
{
	if (WeaponUI)
	{
		WeaponUI->SetReserveText(CurrentReserveAmmo);
	}
}

bool AWeapon::IsSameWeaponType(AWeapon* OtherWeapon)
{
	if (OtherWeapon == nullptr)
		return false;

	return IsSameWeaponType(OtherWeapon->WeaponType);
}

bool AWeapon::IsSameWeaponType(FGameplayTag TagToCheck)
{
	return TagToCheck == WeaponType;
}
