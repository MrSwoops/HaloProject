#include "WeaponAmmoHandler.h"

#include "FMODBlueprintStatics.h"
#include "MyProject/PlayerCharacter.h"
#include "MyProject/UI/WeaponUIWidget.h"
#include "MyProject/Weapons/Weapon.h"
#include "MyProject/Weapons/WeaponData/WeaponAmmoData.h"

void UWeaponAmmoHandler::Initialize(UWeaponAmmoData* AmmoData, UWeaponUIData* InUIData)
{
	MaxMagSize = AmmoData->MaxMagSize;
	MaxReserveMags = AmmoData->MaxReserveMags;
	CurrentReserveAmmo = (MaxReserveMags - 1) * MaxMagSize;
	CurrentMagAmmo = MaxMagSize;

	CanLootAmmo = AmmoData->CanLootAmmo;
	LootAmmoSound = AmmoData->LootAmmoSound;

	ReloadAnimation = AmmoData->ReloadAnimation;
	ReloadSoundEvent = AmmoData->ReloadSoundEvent;

	UIData = InUIData;
}

void UEnergyWeaponAmmoHandler::Initialize(UWeaponAmmoData* AmmoData, UWeaponUIData* InUIData)
{
	Super::Initialize(AmmoData, InUIData);
	if (const UEnergyAmmoData* EAmmoData = Cast<UEnergyAmmoData>(AmmoData))
	{
		MaxHeatLevel = EAmmoData->MaxHeatLevel;
		HeatBuildupPerShot = EAmmoData->HeatBuildupPerShot;
		HeatDissipationSpeed = EAmmoData->HeatDissipationSpeed;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Non-UEnergyAmmoData passed to AmmoData UEnergyWeaponAmmoHandler::Initialize"));
	}
}

void UWeaponAmmoHandler::OnShot()
{
	CurrentMagAmmo--;
	if (WeaponUI) WeaponUI->UpdateAmmoUI(CurrentMagAmmo);
}

void UEnergyWeaponAmmoHandler::OnShot()
{
	CurrentMagAmmo--;
	CurrentHeatLevel += HeatBuildupPerShot;
	if (CurrentHeatLevel >= MaxHeatLevel) SetOverheated();
	if (WeaponUI)
	{
		WeaponUI->UpdateAmmoUI(CurrentHeatLevel / MaxHeatLevel);
		WeaponUI->SetReserveText((static_cast<float>(CurrentMagAmmo) / static_cast<float>(MaxMagSize)) * 100.0f);
	}
}


void UWeaponAmmoHandler::TriggerReload()
{
	if (CurrentMagAmmo == MaxMagSize || CurrentReserveAmmo <= 0 || GetWorld()->GetTimerManager().IsTimerActive(ReloadTimerHandle)) return;
	if (IsPlayerOwned && ReloadAnimation != nullptr)
	{
		if (UAnimInstance* AnimInstance = Cast<APlayerCharacter>(CharacterOwner)->GetMesh1P()->GetAnimInstance()) // Get the animation object for the arms mesh
		{
			AnimInstance->Montage_Play(ReloadAnimation, 1.f, EMontagePlayReturnType::MontageLength, 0.f, true);
		}
	}
	ReloadFMODInstance = UFMODBlueprintStatics::PlayEventAttached(
		ReloadSoundEvent,
		WeaponOwner->GetRootComponent(),
		"",
		FVector::ZeroVector,
		EAttachLocation::Type::SnapToTarget,
		true,
		true,
		true
	);
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &UWeaponAmmoHandler::OnReload, (ReloadAnimation) ? ReloadAnimation->GetPlayLength() + ReloadOffsetTime : 1.0f, false);
}

void UWeaponAmmoHandler::OnReload()
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

void UWeaponAmmoHandler::CancelReload()
{
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	if (ReloadFMODInstance) ReloadFMODInstance->Stop();
}

bool UWeaponAmmoHandler::LootWeapon(UWeaponAmmoHandler* LootedWeapon)
{
	int32 AmmoNeeded = MaxReserveMags * MaxMagSize - CurrentReserveAmmo;
	if (AmmoNeeded <= 0) return false;
	int32 AmmoAvailable = LootedWeapon->CurrentReserveAmmo + LootedWeapon->CurrentMagAmmo;

	FFMODEventInstance FMInstance = UFMODBlueprintStatics::PlayEvent2D(
		GetWorld(),
		LootAmmoSound,
		true
	);
	// FFMODEventInstance FMODInstance = UFMODBlueprintStatics::PlayEventAtLocation(
	// 	GetWorld(), // Or a relevant UObject* from your current world context
	// 	LootedWeapon->ScavageSoundEvent,
	// 	GetActorTransform(),
	// 	true // bAutoPlay: true to start playing immediately
	// );
	
	if (AmmoAvailable <= AmmoNeeded) // Take all ammo and delete looted gun
	{
		CurrentReserveAmmo += AmmoAvailable;
		if (WeaponUI) WeaponUI->SetReserveText(CurrentReserveAmmo);
		return true;
	}
	else // Take needed ammo
	{
		CurrentReserveAmmo += AmmoNeeded;
		LootedWeapon->CurrentReserveAmmo -= AmmoNeeded;
		if (LootedWeapon->CurrentReserveAmmo < 0)
		{
			LootedWeapon->CurrentMagAmmo += LootedWeapon->CurrentReserveAmmo;
			LootedWeapon->CurrentReserveAmmo = 0;
		}
		if (WeaponUI) WeaponUI->SetReserveText(CurrentReserveAmmo);
		return false;
	}
}

void UWeaponAmmoHandler::AttachWeaponUI(UWeaponUIWidget* InWeaponUI)
{
	if (InWeaponUI == nullptr) return;

	WeaponUI = InWeaponUI;
	WeaponUI->InitializeWeaponUI(CurrentMagAmmo, MaxMagSize, CurrentReserveAmmo, UIData);
}

void UEnergyWeaponAmmoHandler::AttachWeaponUI(UWeaponUIWidget* InWeaponUI)
{
	if (InWeaponUI == nullptr) return;

	WeaponUI = InWeaponUI;
	WeaponUI->InitializeWeaponUI(CurrentHeatLevel / MaxHeatLevel, (CurrentMagAmmo / MaxMagSize) * 100.0f, UIData);
}

void UEnergyWeaponAmmoHandler::UpdateAmmoHandler(float DeltaTime)
{
	if (CurrentHeatLevel > 0)
	{
		CurrentHeatLevel -= HeatDissipationSpeed * DeltaTime;
		if (WeaponUI) WeaponUI->UpdateAmmoUI(CurrentHeatLevel / MaxHeatLevel);
		if (CurrentHeatLevel <= 0)
		{
			CurrentHeatLevel = 0;
			IsOverHeated = false;
		} 
	}
}

void UEnergyWeaponAmmoHandler::SetOverheated()
{
	CurrentHeatLevel = MaxHeatLevel;
	IsOverHeated = true;
}

void UEnergyWeaponAmmoHandler::TriggerReload()
{
	IsOverHeated = true;
}
