// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectCharacter.h"

#include "ActorPool.h"
#include "BulletPoolManager.h"
#include "CharacterInteractableComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Interfaces/DamageDealer.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "CustomGameMode.h"
#include "Weapons/Grenade.h"
#include "WeaponUIWidget.h"
#include "Weapons/Weapon.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/SpringArmComponent.h"


//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter

AMyProjectCharacter::AMyProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("3PCamArm"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent(), FName("SpringEndpoint"));
	SpringArmComp->bUsePawnControlRotation = true;
	
	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCameraComponent->SetupAttachment(SpringArmComp);
	ThirdPersonCameraComponent->SetAutoActivate(false);
	//ThirdPersonCameraComponent->;
	

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	Mesh2P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh2P"));
	Mesh2P->SetupAttachment(GetCapsuleComponent());
	Mesh2P->SetOwnerNoSee(true);
	Mesh2P->SetRelativeLocation(FVector(0.f, 0.f, -94.f));
	Mesh2P->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
}

void AMyProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	//UpdateGrenadesUI.Broadcast(CurrentGrenade, RegularGrenades, PlasmaGrenades);
}


//////////////////////////////////////////////////////////////////////////// Input

void AMyProjectCharacter::NotifyControllerChanged()
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

void AMyProjectCharacter::SwapCam()
{
	SetCameraPersp(!FirstPersonCameraComponent->IsActive());
}


void AMyProjectCharacter::SetCameraPersp(const bool& FirstPerson)
{
	if (FirstPerson)
	{
		Mesh2P->SetOwnerNoSee(true);
		Mesh1P->SetOwnerNoSee(false);
		FirstPersonCameraComponent->SetActive(true);
		ThirdPersonCameraComponent->SetActive(false);
		//bUseControllerRotationYaw = true;
	}
	else
	{
		Mesh2P->SetOwnerNoSee(false);
		Mesh1P->SetOwnerNoSee(true);
		ThirdPersonCameraComponent->SetActive(true);
		FirstPersonCameraComponent->SetActive(false);
		//bUseControllerRotationYaw = false;
	}
}


void AMyProjectCharacter::TakeDamage(IDamageDealer* dd)
{
	Health -= dd->GetDamage();
	if (Health <= 0) Health = 0;
	
	//OnReceiveDamage.Broadcast(Health, MaxHealth, dd->GetDamage());
}
void AMyProjectCharacter::TakeDamage(const int32& Damage)
{
	Health -= Damage;
	if (Health <= 0) Health = 0;
	
	//OnReceiveDamage.Broadcast(Health, MaxHealth, Damage);
}

void AMyProjectCharacter::AddInteractable(UCharacterInteractableComponent* Interactable)
{
	if (Interactables.Contains(Interactable)) return;
	Interactables.Push(Interactable);
	CurrentInteraction = Interactable;
	//UpdateInteractions.Broadcast(true, CurrentInteraction->InteractText);
}

void AMyProjectCharacter::RemoveInteractable(UCharacterInteractableComponent* Interactable)
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


void AMyProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::Look);
		EnhancedInputComponent->BindAction(SwapCamAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::SwapCam);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::TryInteract);
		//Interact
		EnhancedInputComponent->BindAction(SwapWeaponsAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::SwapWeapons);
		EnhancedInputComponent->BindAction(SwapGrenadesAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::SwapGrenades);
		//Interact
		EnhancedInputComponent->BindAction(ThrowGrenadeAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::ThrowGrenade);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::Reload);
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::Melee);
	}
	else
	{
		//UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMyProjectCharacter::Reload()
{
	if (CurrentWeapon)
		CurrentWeapon->Reload();
}

void AMyProjectCharacter::Melee()
{
	if (CurrentWeapon)
		CurrentWeapon->Melee();
}




void AMyProjectCharacter::ThrowGrenade()
{
	switch (CurrentGrenade)
	{
		case 0: // Regular
			if (RegularGrenades == 0) return;
			RegularGrenades--;
			break;
		case 1: // Plasma
			if (PlasmaGrenades == 0) return;
			PlasmaGrenades--;
			break;
		default:

			break;
	}

	const FRotator SpawnRotation = Cast<APlayerController>(GetController())->PlayerCameraManager->GetCameraRotation();
	const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(100.0f, 0.0f, 10.0f));

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	// Spawn the projectile at the muzzle
	Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->BulletPoolManager->SpawnBullet(SpawnLocation, SpawnRotation, M9);
	
	//UpdateGrenadesUI.Broadcast(CurrentGrenade, RegularGrenades, PlasmaGrenades);
}
void AMyProjectCharacter::SwapGrenades()
{
	if (RegularGrenades > 0 && PlasmaGrenades > 0)
	{
		CurrentGrenade = (CurrentGrenade == 0) ? 1 : 0; // 0 = Regular, 1 = Plasma
	}
	else
	{
		CurrentGrenade = -1; // No Grenades
	}
	//UpdateGrenadesUI.Broadcast(CurrentGrenade, RegularGrenades, PlasmaGrenades);
}

void AMyProjectCharacter::SwapWeapons()
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetController()->InputComponent);
	if (PrimaryWeapon && SecondaryWeapon)
	{
		CurrentWeapon->SkeletalMeshComp->SetVisibility(false);
		CurrentWeapon->WeaponUI->SetVisibility(ESlateVisibility::Hidden);
		if (UAnimInstance* AnimInstance = GetMesh1P()->GetAnimInstance()) // Get the animation object for the arms mesh
		{
			AnimInstance->Montage_Stop(0.0f);
		}
		GetWorld()->GetTimerManager().ClearTimer(CurrentWeapon->ReloadTimerHandle);
		CurrentWeapon->UnbindActions(EnhancedInputComponent);
		
		if (CurrentWeapon == PrimaryWeapon)
		{
			if (SecondaryWeapon == nullptr) return; // No secondary to swap to
		
			CurrentWeapon = SecondaryWeapon;
		}
		else
		{
			if (PrimaryWeapon == nullptr) return; // No primary to swap to
			CurrentWeapon = PrimaryWeapon;
		}

		CurrentWeapon->SkeletalMeshComp->SetVisibility(true);
		CurrentWeapon->WeaponUI->SetVisibility(ESlateVisibility::Visible);
		CurrentWeapon->BindActions(EnhancedInputComponent);
	}
	
}

void AMyProjectCharacter::PickUpWeapon(AWeapon* Weapon)
{
	if (PrimaryWeapon)
	{
		if (SecondaryWeapon)
		{
			//Drop current weapon and pick up new weapon
			UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetController()->InputComponent);
			CurrentWeapon->UnbindActions(EnhancedInputComponent);
			CurrentWeapon->DropWeapon();
			if (CurrentWeapon == PrimaryWeapon)
			{
				PrimaryWeapon = Weapon;
				CurrentWeapon = Weapon;
				CurrentWeapon->BindActions(EnhancedInputComponent);
			} else
			{
				SecondaryWeapon = Weapon;
				CurrentWeapon = Weapon;
				CurrentWeapon->BindActions(EnhancedInputComponent);
			}
			
		}
		else // Add to secondary and swap
		{
			SecondaryWeapon = Weapon;
			SwapWeapons();
		}
	}
	else
	{
		PrimaryWeapon = Weapon;
		CurrentWeapon = Weapon;
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetController()->InputComponent);
		CurrentWeapon->BindActions(EnhancedInputComponent);
	}
}

bool AMyProjectCharacter::ScavengeWeapon(AWeapon* Weapon) // Returns true if scavenged weapon is empty and needs to be deleted
{
	int32 AmmoNeeded;
	if (Weapon->WeaponModel == PrimaryWeapon->WeaponModel)
	{
		AmmoNeeded = PrimaryWeapon->MaxReserveMags * PrimaryWeapon->MaxMagSize - PrimaryWeapon->CurrentReserveAmmo;
		if (Weapon->CurrentReserveAmmo < AmmoNeeded) AmmoNeeded = Weapon->CurrentReserveAmmo;
		if (AmmoNeeded > 0)
		{
			PrimaryWeapon->CurrentReserveAmmo += AmmoNeeded;
			PrimaryWeapon->WeaponUI->SetReserveText(PrimaryWeapon->CurrentReserveAmmo);
		}
		
	}
	else
	{
		AmmoNeeded = SecondaryWeapon->MaxReserveMags * SecondaryWeapon->MaxMagSize - SecondaryWeapon->CurrentReserveAmmo;
		if (Weapon->CurrentReserveAmmo < AmmoNeeded) AmmoNeeded = Weapon->CurrentReserveAmmo;
		if (AmmoNeeded > 0)
		{
			SecondaryWeapon->CurrentReserveAmmo += AmmoNeeded;
			SecondaryWeapon->WeaponUI->SetReserveText(SecondaryWeapon->CurrentReserveAmmo);
		}
		
	}
	Weapon->CurrentReserveAmmo -= AmmoNeeded;
	if (Weapon->CurrentReserveAmmo <= 0) return true;
	return false;
}


bool AMyProjectCharacter::PickUpGrenade(AGrenade* Grenade)
{
	if (Grenade->Sticky)
	{
		if (PlasmaGrenades >= 4) return false;
		PlasmaGrenades++;
	}
	else
	{
		if (RegularGrenades >= 4) return false;
		RegularGrenades++;
	}
	Grenade->ReturnToPool();
	//UpdateGrenadesUI.Broadcast(CurrentGrenade, RegularGrenades, PlasmaGrenades);
	return true;
}



void AMyProjectCharacter::TryInteract()
{
	if (CurrentInteraction == nullptr) return;
	//CurrentInteraction->OnInteract(this);
}


void AMyProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMyProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}