#include "WeaponAmmoHandler.h"

#include "FMODBlueprintStatics.h"
#include "MyProject/PlayerCharacter.h"
#include "MyProject/Weapons/Weapon.h"
#include "MyProject/Weapons/WeaponData/WeaponAmmoData.h"

void UWeaponAmmoHandler::Initialize(UWeaponAmmoData* AmmoData)
{
	MaxMagSize = AmmoData->MaxMagSize;
	MaxReserveMags = AmmoData->MaxReserveMags;
}

void UEnergyWeaponAmmoHandler::Initialize(UWeaponAmmoData* AmmoData)
{
	Super::Initialize(AmmoData);
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

void UWeaponAmmoHandler::TriggerReload()
{
	if (CurrentMagAmmo == MaxMagSize || CurrentReserveAmmo <= 0) return;
	if (ReloadAnimation != nullptr)
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
	// if (WeaponUI)
	// {
	// 	WeaponUI->UpdateAmmoUI(CurrentMagAmmo);
	// 	WeaponUI->SetReserveText(CurrentReserveAmmo);
	// }
}

void UWeaponAmmoHandler::CancelReload()
{
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	if (ReloadFMODInstance) ReloadFMODInstance->Stop();
}
