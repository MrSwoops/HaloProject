// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "../Components/MyProjectPickUpComponent.h"
#include "../GameplayCharacter.h"
#include "../Components/BulletPoolManager.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "../FirstPersonAnimInstance.h"
#include "../UI/WeaponUIWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "MyProject/CharacterAnimInstance.h"
#include "MyProject/PlayerCharacter.h"
#include "FMODBlueprintStatics.h"
#include "MyProject/GameModes/BaseGameMode.h"
#include "WeaponData/WeaponAmmoData.h"
#include "WeaponData/WeaponFireData.h"
#include "WeaponUI/WeaponUIData.h"

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
	GameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	PickUpComp->AttachedWeapon = this;
	PickUpComp->OnInteractDelegate.AddDynamic(this, &AWeapon::AttachWeapon);
	//SkeletalMeshComp->SetCollisionProfileName(FName("DroppedWeapon"));

	InitializeWeapon();
}

void AWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (FireHandler && FireHandler->IsFireHeld) FireHandler->FireHeld(DeltaSeconds);
	if (AmmoHandler) AmmoHandler->UpdateAmmoHandler(DeltaSeconds);
}

void AWeapon::Reload()
{
	if (AmmoHandler != nullptr) AmmoHandler->TriggerReload();
}

void AWeapon::Melee()
{
	FFMODEventInstance FMODInstance = UFMODBlueprintStatics::PlayEventAtLocation(
		GetWorld(), // Or a relevant UObject* from your current world context
		MeleeSoundEvent,
		GetActorTransform(),
		true // bAutoPlay: true to start playing immediately
	);
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
	if (FireHandler) FireHandler->CharacterOwner = TargetCharacter;
	if (AmmoHandler) AmmoHandler->CharacterOwner = TargetCharacter;

	SkeletalMeshComp->SetEnableGravity(false);
	SkeletalMeshComp->SetSimulatePhysics(false);
	SkeletalMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Character->PickUpWeapon(this);
	
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;//(, true);
	AttachmentRules.bWeldSimulatedBodies = true;
	if (APlayerCharacter* PC = Cast<APlayerCharacter>(TargetCharacter))
	{
		IsPlayerOwned = true;
		if (FireHandler) FireHandler->IsPlayerOwned = true;
		if (AmmoHandler) AmmoHandler->IsPlayerOwned = true;
		WeaponUI = CreateWidget<UWeaponUIWidget>(Cast<APlayerController>(PC->GetController()), UIData->WeaponUIClass);
		if (AmmoHandler)
		{
			if (WeaponUI) AmmoHandler->AttachWeaponUI(WeaponUI);
		}
		AttachToComponent(PC->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
		Cast<UFirstPersonAnimInstance>(PC->GetMesh1P()->GetAnimInstance())->HasRifle = true;
	}
	else
	{
		if (FireHandler) FireHandler->AimPoint = &Cast<UCharacterAnimInstance>(Character->GetMesh()->GetAnimInstance())->TargetLookRigPoint;
		AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("hand_rSocket")));
	}
	if (auto* AnimInstance = Cast<UCharacterAnimInstance>(TargetCharacter->GetMesh()->GetAnimInstance())) AnimInstance->HasRifle = true;
}

void AWeapon::DropWeapon()
{
	if (IsPlayerOwned)
	{
		if (auto* IC = Cast<UEnhancedInputComponent>(Character->GetController()->InputComponent))
		UnbindActions(IC);
		IsPlayerOwned = false;
	}
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
	GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
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

void AWeapon::BindActions(UEnhancedInputComponent* InpComp)
{
	Bindings2.Add("FirePressedAction", &InpComp->BindAction(FireAction, ETriggerEvent::Started, this, &AWeapon::FirePressed));
	Bindings2.Add("FireReleasedAction", &InpComp->BindAction(FireAction, ETriggerEvent::Completed, this, &AWeapon::FireReleased));
}
void AWeapon::UnbindActions(UEnhancedInputComponent* InpComp)
{
	FEnhancedInputActionEventBinding** FirePressedBinding = Bindings2.Find("FirePressedAction");
	if (FirePressedBinding && *FirePressedBinding)
	{
		InpComp->RemoveBinding(*(*FirePressedBinding));
		Bindings2.Remove("FirePressedAction");
	}

	if (FireHandler->IsFireHeld) {FireHandler->FireReleased();}
	FEnhancedInputActionEventBinding** FireReleasedBinding = Bindings2.Find("FireReleasedAction");
	if (FireReleasedBinding && *FireReleasedBinding)
	{
		InpComp->RemoveBinding(*(*FireReleasedBinding));
		Bindings2.Remove("FireReleasedAction");
	}

	for (const auto& Binding : Bindings2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Binding: %s"), *Binding.Key);
	}
	Bindings2.Empty();

}

void AWeapon::DisableWeapon()
{
	SkeletalMeshComp->SetVisibility(false);
	if (IsPlayerOwned && WeaponUI) WeaponUI->SetVisibility(ESlateVisibility::Hidden);
	if (AmmoHandler) AmmoHandler->CancelReload();
}

void AWeapon::EnableWeapon()
{
	SkeletalMeshComp->SetVisibility(true);
	if (IsPlayerOwned && WeaponUI) WeaponUI->SetVisibility(ESlateVisibility::Visible);
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
