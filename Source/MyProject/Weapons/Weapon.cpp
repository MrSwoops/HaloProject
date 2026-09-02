// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "WeaponInventory.h"
#include "MyProject/Characters/CharacterAnimInstance.h"
#include "MyProject/Characters/Player/PlayerCharacter.h"
#include "MyProject/GameModes/BaseGameMode.h"
#include "MyProject/Interactions/MyProjectPickUpComponent.h"
#include "WeaponData/WeaponAmmoData.h"
#include "WeaponData/WeaponFireData.h"
#include "WeaponUI/WeaponUIData.h"
#include "WeaponUI/WeaponUIWidget.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeleMesh"));
	SkeletalMeshComp->SetSimulatePhysics(true);
	SkeletalMeshComp->SetCollisionProfileName(FName("DroppedWeapon"));
	
	GunMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Gun Muzzle"));
	GunMuzzle->SetupAttachment(SkeletalMeshComp);

	RootComponent = SkeletalMeshComp;

	PickUpComp = CreateDefaultSubobject<UMyProjectPickUpComponent>(TEXT("Interaction"));
	PickUpComp->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	PickUpComp->AttachedWeapon = this;
	PickUpComp->OnInteractDelegate.AddDynamic(this, &AWeapon::OnPickup);
	//SkeletalMeshComp->SetCollisionProfileName(FName("DroppedWeapon"));

	InitializeWeapon();
}

void AWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (FireHandler && FireHandler->IsFireHeld) FireHandler->FireHeld(DeltaSeconds);
	if (AmmoHandler) AmmoHandler->UpdateAmmoHandler(DeltaSeconds);
}
void AWeapon::OnPickup(AGameplayCharacter* TargetCharacter)
{
	if (!TargetCharacter) { UE_LOG(LogTemp, Warning, TEXT("Tried to pick up weapon with an invalid character in AWeapon::OnPickup")); return; }
	Character = TargetCharacter;
	if (FireHandler) FireHandler->CharacterOwner = TargetCharacter;
	if (AmmoHandler) AmmoHandler->CharacterOwner = TargetCharacter;

	SkeletalMeshComp->SetEnableGravity(false);
	SkeletalMeshComp->SetSimulatePhysics(false);
	SkeletalMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	TargetCharacter->WeaponInventory->PickUpWeapon(this);
}
void AWeapon::SetPlayerOwned(const bool& bPlayerOwned)
{
	IsPlayerOwned = bPlayerOwned;
	if (FireHandler) FireHandler->IsPlayerOwned = bPlayerOwned;
	if (AmmoHandler) AmmoHandler->IsPlayerOwned = bPlayerOwned;
	if (bPlayerOwned)
	{
		WeaponUI = CreateWidget<UWeaponUIWidget>(Cast<APlayerController>(Character->GetController()), UIData->WeaponUIClass);
		if (AmmoHandler)
		{
			if (WeaponUI) AmmoHandler->AttachWeaponUI(WeaponUI);
		}
	}
	else
	{
		if (FireHandler) FireHandler->AimPoint = &Cast<UCharacterAnimInstance>(Character->GetMesh()->GetAnimInstance())->TargetLookRigPoint;
	}
}

void AWeapon::Reload()
{
	if (AmmoHandler != nullptr) AmmoHandler->TriggerReload();
}

void AWeapon::SetMeleeHitBox(const bool& bActive)
{
	
}

void AWeapon::DropWeapon()
{
	IsPlayerOwned = false;
	CurrentStorageSocket = "";
	GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	if (FireHandler)
	{
		FireHandler->OnWeaponDropped();
	}
	if (AmmoHandler)
	{
		AmmoHandler->OnWeaponDropped();
	}
	if (WeaponUI)
	{
		if (AmmoHandler) AmmoHandler->WeaponUI = nullptr;
		WeaponUI->RemoveFromParent();
	}
	
	Character = nullptr;
	if (AmmoHandler == nullptr || AmmoHandler->IsEmpty())
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void AWeapon::InitializeWeapon()
{
	if (AmmoData)
	{
		AmmoHandler = NewObject<UWeaponAmmoHandler>(this, AmmoData->AmmoHandlerType);
		AmmoHandler->Initialize(AmmoData, UIData);
		AmmoHandler->WeaponOwner = this;
	}
	if (FireData)
	{
		FireHandler = NewObject<UWeaponFireHandler>(this, FireData->FireHandlerType);
		FireHandler->Initialize(FireData, AmmoHandler, ProjectileData, Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->BulletPoolManager);
		FireHandler->WeaponOwner = this;
		FireHandler->MuzzleOffset = &MuzzleOffset;
		FireHandler->WeaponType = &WeaponType;
	}
}

void AWeapon::FirePressed()
{
	if (FireHandler != nullptr)
	{
		FireHandler->FirePressed();
	}
}

void AWeapon::FireReleased()
{
	if (FireHandler != nullptr) FireHandler->FireReleased();
}
