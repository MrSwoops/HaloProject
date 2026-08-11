// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCharacter.h"

#include "CharacterAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Actions/ActionHandler.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject/Combat/DamageMessage.h"
#include "MyProject/Components/CharacterInteractableComponent.h"
#include "MyProject/Components/EnergyShield.h"
#include "MyProject/Components/EnergyShieldShellSKM.h"
#include "MyProject/EventSystem/EventDefinitions.h"
#include "MyProject/EventSystem/GlobalEventManager.h"
#include "MyProject/Weapons/Grenade.h"
#include "MyProject/Weapons/WeaponData/ProjectileData.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "MyProject/Combat/HurtBox.h"
#include "MyProject/Weapons/WeaponInventory.h"

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
	WeaponInventory->OwnerCharacter = this;


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

	VisionPoint = CreateDefaultSubobject<USceneComponent>(TEXT("VisionPoint"));
	VisionPoint->SetupAttachment(GetMesh());
	
	AIPerceptionStimuli = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuli"));
	
	ActionHandler = CreateDefaultSubobject<UActionHandler>(TEXT("ActionHandler"));
}

void AGameplayCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;

	MeshLocation = GetMesh()->GetRelativeLocation();
	MeshRotation = GetMesh()->GetRelativeRotation();

	AssignDynamicMaterials();

	ActionHandler->InitializeActionComponent(this);

	WeaponInventory->OwnerCharacter = this;
	WeaponInventory->InitializeInventory();
}

void AGameplayCharacter::AssignDynamicMaterials()
{
	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		const int32 NumMaterials = CharacterMesh->GetNumMaterials();
		for (int32 i = 0; i < NumMaterials; ++i)
		{
			if (UMaterialInstanceDynamic* DynamicMaterial = CharacterMesh->CreateDynamicMaterialInstance(i))
			{
				DynamicMaterialInstances.Add(DynamicMaterial);
			}
		}
	}
}

void AGameplayCharacter::SetCharacterColor(const FLinearColor& NewColor, int32 MaterialIndex)
{
	if (DynamicMaterialInstances.Num() <= 0) AssignDynamicMaterials();
	if (MaterialIndex >= 0)
	{
		if (DynamicMaterialInstances.IsValidIndex(MaterialIndex) && DynamicMaterialInstances[MaterialIndex])
			DynamicMaterialInstances[MaterialIndex]->SetVectorParameterValue(FName("Color"), NewColor);
	}
	else
	{
		for (int i = 0; i < DynamicMaterialInstances.Num(); ++i)
		{
			if (DynamicMaterialInstances.IsValidIndex(i) && DynamicMaterialInstances[i])
				DynamicMaterialInstances[i]->SetVectorParameterValue(FName("Color"), NewColor);
		}
	}
}


void AGameplayCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateLookRotation(DeltaSeconds);
	
	// if (bShouldRotateToCamera)
	// {
	// 	FRotator CurrentRotation = GetActorRotation();
	// 	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DesiredRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);
	// 	SetActorRotation(NewRotation);
	// 	if (CurrentRotation.Equals(DesiredRotation, 0.5f)) // close enough
	// 	{
	// 		bShouldRotateToCamera = false;
	// 	}
	// }
	// // IK rig check
	// FRotator CharacterRotation = GetActorRotation();
	// float NormalizedYaw = FMath::UnwindDegrees(NewYaw);
	// float NormalizedCharYaw = FMath::UnwindDegrees(CharacterRotation.Yaw);
	// float YawDifference = FMath::Abs(FMath::FindDeltaAngleDegrees(NormalizedCharYaw, NormalizedYaw));
	// if (YawDifference > IKRigYawFreedom || ActionHandler->HasActiveAction() || GetVelocity().Length() > 0)
	// {
	// 	DesiredRotation = FRotator(0.f, NewYaw, 0.f);
	// 	bShouldRotateToCamera = true;
	// }
}
void AGameplayCharacter::UpdateLookRotation(const float& DeltaSeconds)
{
	const float ControlYaw = Controller->GetControlRotation().Yaw;
	const float CharacterYaw = GetActorRotation().Yaw;
	const float YawDifference = FMath::Abs(FMath::FindDeltaAngleDegrees(CharacterYaw, ControlYaw));

	const bool bForceRotation = ActionHandler->HasActiveAction() || !GetVelocity().IsNearlyZero();

	if (bForceRotation || YawDifference > IKRigYawFreedom)
	{
		DesiredRotation = FRotator(0.f, ControlYaw, 0.f);
		bShouldRotateToCamera = true;
	}

	if (bShouldRotateToCamera)
	{
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), DesiredRotation, DeltaSeconds, RotationSpeed);
		SetActorRotation(NewRotation);
		if (NewRotation.Equals(DesiredRotation, 0.5f)) bShouldRotateToCamera = false;
	}
}


void AGameplayCharacter::Respawn(const FVector& Location, const FRotator& Rotation)
{
	SetRagdoll(false);
	Health = MaxHealth;
	bIsDead = false;
	WeaponInventory->InitializeInventory();
	if (EnergyShield) EnergyShield->EnableShieldComponent();
	SetActorLocationAndRotation(Location, Rotation);
	DamageLog.ClearLog();
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
	Execute_Die(this);
}


void AGameplayCharacter::Die_Implementation()
{
	if (Execute_IsDead(this)) return;
	bIsDead = true;
	WeaponInventory->DropInventory();
	if (EnergyShield) EnergyShield->DisableShieldComponent();
	SetRagdoll(true);
	Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->RespawnCharacter(this);

	using namespace GlobalEventManager;
	AGameplayCharacter* Killer = nullptr;
	TArray<AGameplayCharacter*> Assists;
	DamageLog.GetDamageInformation(Killer, Assists);
	const FPlayerKilledMessage Msg = FPlayerKilledMessage(DamageLog, Killer, Assists, this);
	FGlobalEventManager::RaiseEvent(Msg);
}

void AGameplayCharacter::OnStartTakingFire(const FDamageMessage& DmgMsg)
{
	IsUnderFire = true;
	FTimerManager* TManager = &GetWorld()->GetTimerManager();
	if (TManager->IsTimerActive(UnderFireTimer)) TManager->ClearTimer(UnderFireTimer);
	TManager->SetTimer(UnderFireTimer, this, &AGameplayCharacter::OnStopTakingFire, UnderFireTime, false);
}
void AGameplayCharacter::OnStopTakingFire()
{
	IsUnderFire = false;
}

void AGameplayCharacter::TakeDamage_Implementation(const FDamageMessage& DmgMsg)
{
	if (Execute_IsDead(this)) return;
	if (EnergyShield)
	{
		int32 Damage = EnergyShield->TakeDamage(DmgMsg);
		if (Damage > 0)
		{
			if (DmgMsg.CritBehavior & InstaKHeadHealth && DmgMsg.HitPart == EBodyPart::Head)
				Health = 0;
			else
				Health -= Damage;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No energy shield on character AGameplayCharacter::TakeDamage_Implementation"));
		if (DmgMsg.CritBehavior & InstaKHeadHealth && DmgMsg.HitPart == EBodyPart::Head)
			Health = 0;
		else
			Health -= DmgMsg.Damage;
		
	}
	if (DmgMsg.HitForce > 1.0f) GetCharacterMovement()->Launch(DmgMsg.HitDirection * DmgMsg.HitForce);
	if (Health <= 0) { Execute_Die(this); return; }
	OnStartTakingFire(DmgMsg);
	//FDamageLogEntry LogEntry = FDamageLogEntry(Weapon, DmgMsg.Damage, DmgMsg.Attacker, Projectile->Weapon);
	//DamageLog.AddEntry(LogEntry);
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

void AGameplayCharacter::TriggerReloadAction()
{
	WeaponInventory->TryTriggerReloadAction(ActionHandler);
}
void AGameplayCharacter::ReloadCurrentWeapon()
{
	WeaponInventory->ReloadCurrentWeapon();
}

void AGameplayCharacter::Melee()
{
	WeaponInventory->TryMeleeWeapon(ActionHandler);
	
}

void AGameplayCharacter::TriggerGrenadeThrow()
{
	
	if (WeaponInventory->HasGrenade(0))
	{
		if (GrenadeThrowAction && ActionHandler)
		{
			FActionRequest GrenadeAction = FActionRequest(GrenadeThrowAction);
			GrenadeAction.OnCanceled.BindLambda([this]()
			{
				if (GrenadeToThrow) GrenadeToThrow->GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			});
			ActionHandler->TriggerActionEvent(GrenadeAction);
		}
		else
		{
			const FRotator SpawnRotation = Cast<APlayerController>(GetController())->PlayerCameraManager->GetCameraRotation();
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(100.0f, 0.0f, 10.0f));
			AGrenade* OutNade = nullptr;
			if (WeaponInventory->TryGetGrenade(SpawnLocation, SpawnRotation, OutNade))
			{
				UE_LOG(LogTemp, Warning, TEXT("GrenadeThrowAction is null"));
				OutNade->Throw();
				UpdateGrenadesUI.Broadcast(WeaponInventory->CurrentGrenade, WeaponInventory->RegularGrenades, WeaponInventory->PlasmaGrenades);
			}
		}
	}
}

void AGameplayCharacter::PrepGrenade()
{
	const FRotator SpawnRotation = Cast<APlayerController>(GetController())->PlayerCameraManager->GetCameraRotation();
	const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(100.0f, 0.0f, 10.0f));
	AGrenade* OutNade = nullptr;
	if (WeaponInventory->TryGetGrenade(SpawnLocation, SpawnRotation, OutNade))
	{
		GrenadeToThrow = OutNade;
		FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;//(, true);
		AttachmentRules.bWeldSimulatedBodies = true;
		GrenadeToThrow->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("hand_rSocket")));
		UpdateGrenadesUI.Broadcast(WeaponInventory->CurrentGrenade, WeaponInventory->RegularGrenades, WeaponInventory->PlasmaGrenades);
	}
}

void AGameplayCharacter::ReleaseGrenade()
{
	if (!GrenadeThrowAction) { UE_LOG(LogTemp, Warning, TEXT("Tried to release a non existent grenade in AGameplayCharacter::ReleaseGrenade")); return; }

	const FRotator SpawnRotation = Cast<APlayerController>(GetController())->PlayerCameraManager->GetCameraRotation();
	GrenadeToThrow->GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	GrenadeToThrow->Throw(true, SpawnRotation);
	GrenadeToThrow = nullptr;
}

void AGameplayCharacter::SwapGrenades()
{
	WeaponInventory->SwapGrenades();
}

void AGameplayCharacter::SwapWeapons()
{
	WeaponInventory->TryTriggerSwapWeaponsAction(ActionHandler);
}
void AGameplayCharacter::FirePressed()
{
	if (WeaponInventory->CurrentWeapon) WeaponInventory->CurrentWeapon->FirePressed();	
}
void AGameplayCharacter::FireReleased()
{
	if (WeaponInventory->CurrentWeapon) WeaponInventory->CurrentWeapon->FireReleased();
}
void AGameplayCharacter::PickUpWeapon(AWeapon* Weapon)
{
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

	FRotator ControlRotation = Controller->GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	FVector ForwardDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);  // Forward (X)
	FVector RightDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);    // Right (Y)

	if (auto* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance())) AnimInstance->OnMovement(Value);
	
	AddMovementInput(ForwardDirection, Value.Y);
	AddMovementInput(RightDirection, Value.X);
}


void AGameplayCharacter::Look(const FInputActionValue& Value)
{
	if (Controller == nullptr) return;
	
	FVector2D LookAxis = Value.Get<FVector2D>();

	// Create Look rotation
	FRotator ControlRotation = Controller->GetControlRotation();
	ControlRotation.Yaw += LookAxis.X;//float NewYaw = ControlRotation.Yaw + LookAxis.X;
	ControlRotation.Pitch = FMath::Clamp(ControlRotation.Pitch - LookAxis.Y, -89.f, 89.f);
	ControlRotation.Roll = 0.0f;

	Controller->SetControlRotation(ControlRotation);//FRotator(NewPitch, NewYaw, 0.0f));
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

void AGameplayCharacter::OnActionEndCleanup()
{
	
}
