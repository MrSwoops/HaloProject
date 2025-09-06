// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCharacter.h"

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
#include "WeaponInventory.h"
#include "Weapons/Grenade.h"
#include "WeaponUIWidget.h"
#include "Weapons/Weapon.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter

AGameplayCharacter::AGameplayCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName("CharacterWorldInteraction");

	WeaponInventory = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));

	//CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh2P"));
	GetMesh()->SetCollisionProfileName("NoCollision");
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -94.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	GetMesh()->SetNotifyRigidBodyCollision(true);
	
}

void AGameplayCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}




void AGameplayCharacter::SetRagdoll(bool Active)
{
	if (Active)
	{
		FVector Velocity = GetVelocity();
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->AddImpulse(Velocity * 10, "pelvis", true);
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	}
	else
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("CharacterWorldInteraction"));
	}
}

void AGameplayCharacter::Die()
{
	IsDead = true;
	SetRagdoll(true);
}


void AGameplayCharacter::TakeDamage(IDamageDealer* dd)
{
	TakeDamage(dd->GetDamage());
}
void AGameplayCharacter::TakeDamage(const int32& Damage)
{
	Health -= Damage;
	if (!IsDead && Health <= 0) Die();
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
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetController()->InputComponent))
	{
		WeaponInventory->PickUpWeapon(EnhancedInputComponent, Weapon);
	}
	else
		WeaponInventory->PickUpWeapon(Weapon);
}

bool AGameplayCharacter::ScavengeWeapon(AWeapon* Weapon) // Returns true if scavenged weapon is empty and needs to be deleted
{
	return WeaponInventory->ScavengeWeapon(Weapon);
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
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGameplayCharacter::Look(const FInputActionValue& Value)
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