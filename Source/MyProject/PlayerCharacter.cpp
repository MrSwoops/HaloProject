// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "CharacterAnimInstance.h"
#include "Components/CharacterInteractableComponent.h"
#include "Components/EnergyShield.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/WeaponInventory.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Weapons/Grenade.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter()
{
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("3PCamArm"));
	SpringArmComp->SetupAttachment(GetMesh());
	SpringArmComp->bUsePawnControlRotation = true;
	
	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCameraComponent->SetupAttachment(SpringArmComp);
	ThirdPersonCameraComponent->SetAutoActivate(false);
	//ThirdPersonCameraComponent->;

	EnergyShield = CreateDefaultSubobject<UEnergyShield>(TEXT("Energy Shield"));
	EnergyShield->Player = this;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	FirstPersonMesh->SetCollisionProfileName("NoCollision");
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetupAttachment(FirstPersonCameraComponent);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;
	FirstPersonMesh->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	Tags.Add("Player");
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (auto* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->LookRigBehavior = ELookRigBehaviour::TargetComponentForwardVector;
		AnimInstance->TargetLookRigComponent = FirstPersonCameraComponent;
	}
	UpdateGrenadesUI.Broadcast(WeaponInventory->CurrentGrenade, WeaponInventory->RegularGrenades, WeaponInventory->PlasmaGrenades);
}

void APlayerCharacter::Respawn(const FVector& Location, const FRotator& Rotation)
{
	Super::Respawn(Location, Rotation);
	SetCameraPersp(true);
}


void APlayerCharacter::Die()
{
	SetCameraPersp(false);
	Super::Die();
}


void APlayerCharacter::TakeDamage(const int32& Damage)
{
	Super::TakeDamage(Damage);
	OnReceiveDamage.Broadcast(EnergyShield->CurrentEnergy, EnergyShield->MaxEnergy, Damage);
}

void APlayerCharacter::AddInteractable(UCharacterInteractableComponent* Interactable)
{
	if (Interactables.Contains(Interactable)) return;
	Interactables.Push(Interactable);
	CurrentInteraction = Interactable;
	UpdateInteractions.Broadcast(true, CurrentInteraction->InteractText);
}

void APlayerCharacter::RemoveInteractable(UCharacterInteractableComponent* Interactable)
{
	Interactables.Remove(Interactable);
	if (Interactables.Num() > 0)
	{
		// Assign previous if possible 
		CurrentInteraction = Interactables[Interactables.Num() - 1];
		UpdateInteractions.Broadcast(true, CurrentInteraction->InteractText);
	}
	else
	{
		UpdateInteractions.Broadcast(false, "");
		CurrentInteraction = nullptr;
	}
}

void APlayerCharacter::SetRagdoll(bool Active)
{
	Super::SetRagdoll(Active);
	// if (Active)
	// {
	// 	FVector Velocity = GetVelocity();
	// 	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	// 	GetMesh()->SetSimulatePhysics(true);
	// 	GetMesh()->AddImpulse(Velocity * 10, "pelvis", true);
	// 	//FirstPersonMesh->SetCollisionProfileName(TEXT("Ragdoll"));
	// 	//FirstPersonMesh->SetSimulatePhysics(true);
	// 	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	// }
	// else
	// {
	// 	GetMesh()->SetSimulatePhysics(false);
	// 	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	// 	//FirstPersonMesh->SetSimulatePhysics(false);
	// 	//FirstPersonMesh->SetCollisionProfileName(TEXT("NoCollision"));
	// 	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CharacterWorldInteraction"));
	// }
}

void APlayerCharacter::SwapCam()
{
	SetCameraPersp(!FirstPersonCameraComponent->IsActive());
}

void APlayerCharacter::SetCameraPersp(const bool& FirstPerson)
{
	if (FirstPerson)
	{
		GetMesh()->SetOwnerNoSee(true);
		FirstPersonMesh->SetOwnerNoSee(false);
		FirstPersonCameraComponent->SetActive(true);
		ThirdPersonCameraComponent->SetActive(false);
		Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance())->TargetLookRigComponent = FirstPersonCameraComponent;
		//bUseControllerRotationYaw = true;
	}
	else
	{
		GetMesh()->SetOwnerNoSee(false);
		FirstPersonMesh->SetOwnerNoSee(true);
		ThirdPersonCameraComponent->SetActive(true);
		FirstPersonCameraComponent->SetActive(false);
		Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance())->TargetLookRigComponent = ThirdPersonCameraComponent;
		//bUseControllerRotationYaw = false;
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);
		
		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::Crouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::UnCrouch);
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(SwapCamAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SwapCam);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacter::TryInteract);
		//Interact
		EnhancedInputComponent->BindAction(SwapWeaponsAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SwapWeapons);
		EnhancedInputComponent->BindAction(SwapGrenadesAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SwapGrenades);
		//Interact
		EnhancedInputComponent->BindAction(ThrowGrenadeAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ThrowGrenade);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Reload);
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Melee);
	}
	else
	{
		//UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlayerCharacter::ThrowGrenade()
{
	const FRotator SpawnRotation = Cast<APlayerController>(GetController())->PlayerCameraManager->GetCameraRotation();
	const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(100.0f, 0.0f, 10.0f));
	AGrenade* OutNade = nullptr;
	if (WeaponInventory->TryGetGrenade(SpawnLocation, SpawnRotation, OutNade))
	{
		OutNade->Throw();
		OutNade->Arm(OutNade->FuseTime);
		UpdateGrenadesUI.Broadcast(WeaponInventory->CurrentGrenade, WeaponInventory->RegularGrenades, WeaponInventory->PlasmaGrenades);
	}
}

void APlayerCharacter::SwapGrenades()
{
	Super::SwapGrenades();
	UpdateGrenadesUI.Broadcast(WeaponInventory->CurrentGrenade, WeaponInventory->RegularGrenades, WeaponInventory->PlasmaGrenades);
}

bool APlayerCharacter::PickUpGrenade(AGrenade* Grenade)
{
	if (Super::PickUpGrenade(Grenade))
	{
		UpdateGrenadesUI.Broadcast(WeaponInventory->CurrentGrenade, WeaponInventory->RegularGrenades, WeaponInventory->PlasmaGrenades);
		return true;
	}
	return false;
}

