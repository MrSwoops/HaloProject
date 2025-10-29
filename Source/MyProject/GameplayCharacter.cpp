// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCharacter.h"

#include "CharacterAnimInstance.h"
#include "Components/CharacterInteractableComponent.h"
#include "Components/EnergyShield.h"
#include "Components/EnergyShieldShellSKM.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/HurtBox.h"
#include "InputActionValue.h"
#include "PlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Interfaces/DamageDealer.h"
#include "Engine/World.h"
#include "Components/WeaponInventory.h"
#include "EventSystem/EventDefinitions.h"
#include "EventSystem/GlobalEventManager.h"
#include "GameModes/BaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Weapons/Grenade.h"
#include "Weapons/WeaponData/ProjectileData.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter

AGameplayCharacter::AGameplayCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName("CharacterWorldInteraction");
	GetCapsuleComponent()->CanCharacterStepUpOn = ECB_Yes;

	WeaponInventory = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));

	//CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh2P"));
	GetMesh()->SetCollisionProfileName("NoCollision");
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -94.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	GetMesh()->SetNotifyRigidBodyCollision(true);
	EnergyShieldShell = CreateDefaultSubobject<UEnergyShieldShellSKM>(TEXT("EShieldShell"));
	EnergyShieldShell->SetupAttachment(GetMesh());
	EnergyShieldShell->SetLeaderPoseComponent(GetMesh());
	EnergyShieldShell->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	EnergyShieldShell->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	if (EnergyShield && !EnergyShield->ShieldMesh) EnergyShield->ShieldMesh = EnergyShieldShell;
	EnergyShieldShell->SetShieldVisibility(false);

	AIPerceptionStimuli = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuli"));
}

void AGameplayCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;

	MeshLocation = GetMesh()->GetRelativeLocation();
	MeshRotation = GetMesh()->GetRelativeRotation();
}

void AGameplayCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bShouldRotateToCamera)
	{
		FRotator CurrentRotation = GetActorRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DesiredRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);
		SetActorRotation(NewRotation);
		if (CurrentRotation.Equals(DesiredRotation, 0.5f)) // close enough
		{
			bShouldRotateToCamera = false;
		}
	}
}


void AGameplayCharacter::Respawn(const FVector& Location, const FRotator& Rotation)
{
	SetRagdoll(false);
	Health = MaxHealth;
	IsDead = false;
	WeaponInventory->InitializeInventory();
	if (EnergyShield) EnergyShield->EnableShieldComponent();
	SetActorLocationAndRotation(Location, Rotation);
}

float AGameplayCharacter::GetHealthPercent()
{
	if (EnergyShield)
		return (EnergyShield->CurrentEnergy / EnergyShield->MaxEnergy) * (Health / MaxHealth);
	else
		return Health / MaxHealth;
}


void AGameplayCharacter::SetRagdoll(bool Active)
{
	if (Active)
	{
		FVector Velocity = GetVelocity();
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->AddImpulse(Velocity, "pelvis", true);
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	}
	else
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("CharacterWorldInteraction"));
		GetMesh()->SetRelativeLocation(MeshLocation);
		GetMesh()->SetRelativeRotation(MeshRotation);
	}
}

void AGameplayCharacter::CharacterCrouch()
{
	ACharacter::Crouch(false);
	Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance())->IsCrouching = true;
}

void AGameplayCharacter::CharacterUnCrouch()
{
	ACharacter::UnCrouch(false);
	Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance())->IsCrouching = false;
}

void AGameplayCharacter::FellOutOfWorld(const class UDamageType& dmgType)
{
	Die();
}


void AGameplayCharacter::Die()
{
	if (IsDead) return;
	IsDead = true;
	WeaponInventory->DropInventory();
	if (EnergyShield) EnergyShield->DisableShieldComponent();
	SetRagdoll(true);
	Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->RespawnCharacter(this);

	using namespace GlobalEventManager;
	const FPlayerKilledMessage Msg = FPlayerKilledMessage(this, this);
	FGlobalEventManager::RaiseEvent(Msg);
}


void AGameplayCharacter::TakeDamage(IDamageDealer* dd)
{
	TakeDamage(dd->GetDamage());
}
void AGameplayCharacter::TakeDamage(const int32& Damage)
{
	if (EnergyShield)
		Health -= EnergyShield->TakeDamage(Damage);
	else
		Health -= Damage;
	if (Health <= 0 && !IsDead) { Die(); return; }
	
	// Mark the player as under fire
	IsUnderFire = true;
	FTimerManager* TManager = &GetWorld()->GetTimerManager();
	if (TManager->IsTimerActive(UnderFireTimer)) TManager->ClearTimer(UnderFireTimer);
	TManager->SetTimer(UnderFireTimer, [this]() { IsUnderFire = false; }, UnderFireTime, false);
}
void AGameplayCharacter::TakeProjectileDamage(const UProjectileData* BulletData, const EHurtboxType& HitRegion)
{
	if (EnergyShield)
	{
		int32 Damage = EnergyShield->TakeProjectileDamage(BulletData, HitRegion);
		if (Damage > 0)
		{
			if (BulletData->CritHitBehavior & InstaKHeadHealth && HitRegion == Head)
				Health = 0;
			else
				Health -= Damage;
		}
	}
	else
	{
		if (BulletData->CritHitBehavior & InstaKHeadHealth && HitRegion == Head)
			Health = 0;
		else
			Health -= BulletData->Damage;
		
	}
	if (Health <= 0 && !IsDead) Die();
}

void AGameplayCharacter::AddInteractable(UCharacterInteractableComponent* Interactable)
{
	if (Interactables.Contains(Interactable)) return;
	Interactables.Push(Interactable);
	CurrentInteraction = Interactable;
	//UpdateInteractions.Broadcast(true, CurrentInteraction->InteractText);
}

void AGameplayCharacter::RemoveInteractable(UCharacterInteractableComponent* Interactable)
{
	Interactables.Remove(Interactable);
	if (Interactables.Num() > 0)
	{
		// Assign previous if possible 
		CurrentInteraction = Interactables[Interactables.Num() - 1];
		//UpdateInteractions.Broadcast(true, CurrentInteraction->InteractText);
	}
	else
	{
		//UpdateInteractions.Broadcast(false, "");
		CurrentInteraction = nullptr;
	}
}

void AGameplayCharacter::Reload()
{
	WeaponInventory->TryReloadWeapon();
}

void AGameplayCharacter::Melee()
{
	if (WeaponInventory->TryMeleeWeapon()) return;
	
}

void AGameplayCharacter::ThrowGrenade()
{
	const FRotator SpawnRotation = Cast<APlayerController>(GetController())->PlayerCameraManager->GetCameraRotation();
	const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(100.0f, 0.0f, 10.0f));
	AGrenade* OutNade = nullptr;
	if (WeaponInventory->TryGetGrenade(SpawnLocation, SpawnRotation, OutNade))
	{
		OutNade->Throw();
		OutNade->Arm(OutNade->FuseTime);
	}
}
void AGameplayCharacter::SwapGrenades()
{
	WeaponInventory->SwapGrenades();
}

void AGameplayCharacter::SwapWeapons()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetController()->InputComponent))
	{
		WeaponInventory->SwapWeapons(EnhancedInputComponent);
	}
	else
		WeaponInventory->SwapWeapons();
}

void AGameplayCharacter::PickUpWeapon(AWeapon* Weapon)
{
	if (Cast<APlayerCharacter>(this))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetController()->InputComponent))
			WeaponInventory->PickUpWeapon(EnhancedInputComponent, Weapon);
	}
	else
		WeaponInventory->PickUpWeapon(Weapon);
}

bool AGameplayCharacter::ScavageWeapon(AWeapon* Weapon) // Returns true if scavenged weapon is empty and needs to be deleted
{
	return WeaponInventory->ScavageWeapon(Weapon);
}


bool AGameplayCharacter::PickUpGrenade(AGrenade* Grenade)
{
	return WeaponInventory->PickUpGrenade(Grenade);
}

void AGameplayCharacter::TryInteract()
{
	if (CurrentInteraction == nullptr) return;
	CurrentInteraction->OnInteract(this);
}


void AGameplayCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	Move(Value.Get<FVector2D>());
	
}

void AGameplayCharacter::Move(const FVector2D& Value)
{
	if (Controller == nullptr) return;

	// Get the controller's control rotation (ignoring pitch for movement)
	FRotator ControlRotation = Controller->GetControlRotation();

	// Use only the yaw rotation (ignore pitch/roll)
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	// Get the direction vectors based on the controller's yaw
	FVector ForwardDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);  // Forward (X)
	FVector RightDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);    // Right (Y)
	
	AddMovementInput(ForwardDirection, Value.Y);
	AddMovementInput(RightDirection, Value.X);
}


void AGameplayCharacter::Look(const FInputActionValue& Value)
{
	if (Controller == nullptr) return;
	
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// Create Look rotation
	FRotator ControlRotation = Controller->GetControlRotation();
	float NewYaw = ControlRotation.Yaw + LookAxisVector.X;
	float NewPitch = ControlRotation.Pitch - LookAxisVector.Y;
	NewPitch = FMath::Clamp(NewPitch, -89.0f, 89.0f);

	Controller->SetControlRotation(FRotator(NewPitch, NewYaw, 0.0f));

	// IK rig check
	FRotator CharacterRotation = GetActorRotation();
	float NormalizedYaw = FMath::UnwindDegrees(NewYaw);
	float NormalizedCharYaw = FMath::UnwindDegrees(CharacterRotation.Yaw);
	float YawDifference = FMath::Abs(FMath::FindDeltaAngleDegrees(NormalizedCharYaw, NormalizedYaw));
	if (YawDifference > IKRigYawFreedom)
	{
		DesiredRotation = FRotator(0.f, NewYaw, 0.f);
		bShouldRotateToCamera = true;
	}
}

void AGameplayCharacter::EnterCombat()
{
	IsInCombat = true;
	if (auto* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ExitCombatTimer);
	}
}


void AGameplayCharacter::StartExitCombatTimer(bool OverwriteCurrent)
{
	auto& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(ExitCombatTimer))
	{
		if (!OverwriteCurrent) return;
		TimerManager.ClearTimer(ExitCombatTimer);
	}
	TimerManager.SetTimer(ExitCombatTimer, this, &AGameplayCharacter::ExitCombat, ExitCombatTime, false);
}
