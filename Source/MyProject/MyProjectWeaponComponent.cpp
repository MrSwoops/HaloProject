// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyProjectWeaponComponent.h"
#include "MyProjectCharacter.h"
#include "ActorPool.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
//#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "CustomGameMode.h"
#include "MyProjectPickUpComponent.h"
#include "WeaponUIWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UMyProjectWeaponComponent::UMyProjectWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	PickUpComponent = CreateDefaultSubobject<UMyProjectPickUpComponent>(TEXT("PickUpComp"));
	//PickUpComponent->AttachedWeapon = this;
}

void UMyProjectWeaponComponent::BeginPlay()
{
	GameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	CurrentMagAmmo = MaxMagSize;
	CurrentReserveAmmo = (MaxReserveMags - 1) * MaxMagSize;
}


void UMyProjectWeaponComponent::Fire()
{
	if (CurrentMagAmmo <= 0 || Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		CurrentMagAmmo--;
		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// Spawn the projectile at the muzzle
		//GameMode->BulletPool->SpawnFromLocationAndRotation(SpawnLocation, SpawnRotation);
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
	WeaponUI->UpdateAmmoUI(CurrentMagAmmo);
}

void UMyProjectWeaponComponent::Reload()
{
	int32 ammoNeeded = MaxMagSize - CurrentMagAmmo;
	if (ammoNeeded > CurrentReserveAmmo) ammoNeeded = CurrentReserveAmmo;
	if (ammoNeeded <= 0) return;
	CurrentReserveAmmo -= ammoNeeded;
	CurrentMagAmmo += ammoNeeded;
	WeaponUI->UpdateAmmoUI(CurrentMagAmmo);
	WeaponUI->SetReserveText(CurrentReserveAmmo);
}

void UMyProjectWeaponComponent::Melee()
{
	
}



bool UMyProjectWeaponComponent::AttachWeapon(AMyProjectCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	
	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr)// || Character->GetInstanceComponents().FindItemByClass<UMyProjectWeaponComponent>())
	{
		return false;
	}
	//SetSimulatePhysics(true);
	WeaponUI = CreateWidget<UWeaponUIWidget>(Cast<APlayerController>(Character->GetController()), WeaponUIClass);
	//Character->PickUpWeapon(this);
	if (WeaponUI)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("SetWeaponUI"));	
		WeaponUI->AddToPlayerScreen();
		WeaponUI->SetReserveText(CurrentReserveAmmo);
		WeaponUI->SetMagAmmo(MaxMagSize);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NoWeaponUI"));
	}
	
	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	if (Character->IsLocallyControlled())
	{
		AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	}
	else
	{
		AttachToComponent(Character->GetMesh2P(), AttachmentRules, FName(TEXT("hand_rSocket")));
	}

	/* Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UMyProjectWeaponComponent::Fire);
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &UMyProjectWeaponComponent::Reload);
			EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &UMyProjectWeaponComponent::Melee);
		}
	}
	*/

	return true;
}
