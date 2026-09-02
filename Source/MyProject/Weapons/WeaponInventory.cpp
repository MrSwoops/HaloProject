// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventory.h"
#include "Grenade.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject/Characters/CharacterAnimInstance.h"
#include "MyProject/Characters/Actions/ActionHandler.h"
#include "MyProject/Characters/Actions/CharacterActionEvent.h"
#include "MyProject/Characters/Player/FirstPersonAnimInstance.h"
#include "MyProject/Characters/Player/PlayerCharacter.h"
#include "MyProject/Equipment/Equipment.h"
#include "MyProject/Interactions/MyProjectPickUpComponent.h"
#include "MyProject/ObjectPooling/BulletPoolManager.h"
#include "MyProject/Weapons/WeaponData/WeaponAmmoData.h"

// Sets default values for this component's properties
UWeaponInventory::UWeaponInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponInventory::BeginPlay()
{
	Super::BeginPlay();
	
}
void UWeaponInventory::InitializeInventory()
{
	if (bOverrideDefaultInventory) InitializeInventoryEquipment(DefaultInventory);
	else
	{
		Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->InitializeCharacterInventory(this);
	}
}

void UWeaponInventory::InitializeInventoryEquipment(FInventoryDefault& Inventory)
{
	AGameplayCharacter* Owner = Cast<AGameplayCharacter>(GetOwner());
	if (!Owner) return;
	if (Inventory.StartingSecondary)
	{
		AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(Inventory.StartingSecondary);
		Weapon->OnPickup(Owner);
	} 
	if (Inventory.StartingPrimary)
	{
		AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(Inventory.StartingPrimary);
		Weapon->OnPickup(Owner);
	}
	if (Inventory.StartingEquipment)
	{
	
	}
	RegularGrenades = Inventory.StartingFragGrenades;
	PlasmaGrenades = Inventory.StartingPlasmaGrenades;
}


void UWeaponInventory::InitializeInventoryEquipment(TSubclassOf<AWeapon>& InStartingPrimary, TSubclassOf<AWeapon>& InStartingSecondary, int32& InStartingFragGrenades, int32& InStartingPlasmaGrenades, TSubclassOf<AEquipment>& InStartingEquipment)
{
	AGameplayCharacter* Owner = Cast<AGameplayCharacter>(GetOwner());
	if (!Owner) return;
	if (InStartingSecondary)
	{
		AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(InStartingSecondary);
		Weapon->OnPickup(Owner);
	} 
	if (InStartingPrimary)
	{
		AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(InStartingPrimary);
		Weapon->OnPickup(Owner);
	}
	if (InStartingEquipment)
	{
	
	}
	RegularGrenades = InStartingFragGrenades;
	PlasmaGrenades = InStartingPlasmaGrenades;
}


void UWeaponInventory::PickUpEquipment(AEquipment* Equipment)
{
	
}

void UWeaponInventory::PickUpWeapon(AWeapon* Weapon)
{
	APlayerCharacter* PC = Cast<APlayerCharacter>(OwnerCharacter);
	Weapon->SetPlayerOwned(PC != nullptr);
	
	if (PC != nullptr) { Cast<UFirstPersonAnimInstance>(PC->GetMesh1P()->GetAnimInstance())->HasRifle = true; }
	if (auto* AnimInstance = Cast<UCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance())) AnimInstance->HasRifle = true;
	
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;//(, true);
	AttachmentRules.bWeldSimulatedBodies = true;
	
	if (CurrentWeapon)
	{
		if (StoredWeapon)
		{
			//Drop current weapon and pick up new weapon
			AWeapon* OldWeapon = CurrentWeapon;
			CurrentWeapon = Weapon;
			OldWeapon->DropWeapon();
			Weapon->AttachToComponent(OwnerCharacter->GetMesh(), AttachmentRules, FName(TEXT("hand_rSocket")));
		}
		else // Add to secondary and swap
		{
			StoredWeapon = Weapon;
			Weapon->AttachToComponent(OwnerCharacter->GetMesh(), AttachmentRules, StoredWeapon->PreferredStorageSocket);
			Weapon->CurrentStorageSocket = StoredWeapon->PreferredStorageSocket;
			StoreCurrentWeapon(CurrentWeapon->PreferredStorageSocket);
			SwapWeapons();
			GrabCurrentWeapon();
		}
	}
	else
	{
		CurrentWeapon = Weapon;
		Weapon->AttachToComponent(OwnerCharacter->GetMesh(), AttachmentRules, FName(TEXT("hand_rSocket")));
	}
}

void UWeaponInventory::TryTriggerSwapWeaponsAction(UActionHandler* InActionHandler)
{
	if (!InActionHandler) return;
	if (CurrentWeapon && StoredWeapon)
	{
		if (InActionHandler->GetCurrentAction().IsValid() && InActionHandler->GetCurrentAction().Action->ActionTypeTag == FGameplayTag::RequestGameplayTag(FName("Actions.StoreWeapon"))) // Handle cancel edge case
		{
			UCharacterActionEvent* GrabNewEvent = DetermineSwapEvent(false, CurrentWeapon->PreferredStorageSocket);
			FActionRequest GrabNewActionRequest = FActionRequest(GrabNewEvent);
			GrabNewActionRequest.OnCanceled.BindUObject(this, &UWeaponInventory::GrabCurrentWeapon);
			InActionHandler->TriggerActionEvent(GrabNewActionRequest);
		}
		else
		{
			UCharacterActionEvent* SwapOffEvent = DetermineSwapEvent(true, (StoredWeapon->CurrentStorageSocket == CurrentWeapon->PreferredStorageSocket) ? CurrentWeapon->BackupStorageSocket : CurrentWeapon->PreferredStorageSocket);
			UCharacterActionEvent* GrabNewEvent = DetermineSwapEvent(false, StoredWeapon->CurrentStorageSocket);
			FActionRequest SwapOffActionRequest = FActionRequest(SwapOffEvent);
			FActionRequest GrabNewActionRequest = FActionRequest(GrabNewEvent);
			GrabNewActionRequest.OnCanceled.BindUObject(this, &UWeaponInventory::GrabCurrentWeapon);
			SwapOffActionRequest.OnCanceled.BindUObject(this, &UWeaponInventory::GrabCurrentWeapon);
			SwapOffActionRequest.OnEnded.BindLambda([this, InActionHandler, GrabNewActionRequest]()
			{
				SwapWeapons();
				InActionHandler->BufferActionEvent(GrabNewActionRequest, true);
			});
			InActionHandler->TriggerActionEvent(SwapOffActionRequest);
		}
	}
}

UCharacterActionEvent* UWeaponInventory::DetermineSwapEvent(const bool& Store, const FName& InSocketName)
{
	if (InSocketName == "socket_BackWeapon1")
		return (Store) ? StoreBackWeapon1 : RetrieveBackWeapon1;
	if (InSocketName == "socket_BackWeapon2")
		return (Store) ? StoreBackWeapon2 : RetrieveBackWeapon2;
	if (InSocketName == "socket_ThighWeaponRight")
		return (Store) ? StoreThighRightWeapon : RetrieveThighRightWeapon;
	
	return nullptr;
}

void UWeaponInventory::StoreCurrentWeapon(const FName& AttachSocketName)
{
	if (!CurrentWeapon) { UE_LOG(LogTemp, Error, TEXT("Tried to store an invalid weapon in UWeaponInventory::StoreCurrentWeapon")); return; }
	
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	AttachmentRules.bWeldSimulatedBodies = true;
	CurrentWeapon->AttachToComponent(OwnerCharacter->GetMesh(), AttachmentRules, AttachSocketName);
	CurrentWeapon->CurrentStorageSocket = AttachSocketName;
}
void UWeaponInventory::SwapWeapons()
{
	if (CurrentWeapon && StoredWeapon)
	{
		auto* TempPtr = CurrentWeapon;
		CurrentWeapon = StoredWeapon;
		StoredWeapon = TempPtr;
	}
}
void UWeaponInventory::GrabCurrentWeapon()
{
	if (!CurrentWeapon) { UE_LOG(LogTemp, Error, TEXT("Tried to retrieve an invalid weapon in UWeaponInventory::RetrieveCurrentWeapon")); return; }
	
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	AttachmentRules.bWeldSimulatedBodies = true;
	CurrentWeapon->AttachToComponent(OwnerCharacter->GetMesh(), AttachmentRules, FName(TEXT("hand_rSocket")));
	CurrentWeapon->CurrentStorageSocket = "";
}

#pragma region Grenades

bool UWeaponInventory::PickUpGrenade(AGrenade* Grenade)
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
	return true;
}

void UWeaponInventory::SwapGrenades()
{
	if (RegularGrenades > 0 && PlasmaGrenades > 0)
	{
		CurrentGrenade = (CurrentGrenade == 0) ? 1 : 0; // 0 = Regular, 1 = Plasma
	}
	else
	{
		CurrentGrenade = -1; // No Grenades
	}
}

bool UWeaponInventory::TryGetGrenade(const FVector& SpawnLocation, const FRotator& SpawnRotation, AGrenade*& OutGrenade)
{
	switch (CurrentGrenade) {
	case 0: // Regular
		if (RegularGrenades == 0) return false;
		OutGrenade = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->BulletPoolManager->SpawnGrenade(SpawnLocation, SpawnRotation, CurrentGrenade);
		if (OutGrenade)
		{
			RegularGrenades--;
			return true;
		}
		break;
	case 1: // Plasma
		if (PlasmaGrenades == 0) return false;
		OutGrenade = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->BulletPoolManager->SpawnGrenade(SpawnLocation, SpawnRotation, CurrentGrenade);
		if (OutGrenade)
		{
			PlasmaGrenades--;
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}
bool UWeaponInventory::HasGrenade(const int32& GrenadeType)
{
	switch (GrenadeType)
	{
	case 0: return RegularGrenades > 0;
	case 1: return PlasmaGrenades > 0;
	default: return false;
	};
}
#pragma endregion Grenades

bool UWeaponInventory::ScavageWeapon(AWeapon* Weapon)
{
	if (Weapon->IsSameWeaponType(CurrentWeapon))
	{
		//return FillWeapon(Weapon, PrimaryWeapon);
	}
	else
	{
		//return FillWeapon(Weapon, SecondaryWeapon);
	}
	return false;
}


void UWeaponInventory::TryMeleeWeapon(UActionHandler* InActionHandler)
{
	if (CurrentWeapon && InActionHandler)
	{
		FActionRequest MeleeAction = FActionRequest(CurrentWeapon->MeleeActionEvent);
		MeleeAction.OnCanceled.BindLambda([this]()
		{
			CurrentWeapon->SetMeleeHitBox(false);
		});
		InActionHandler->TriggerActionEvent(MeleeAction);
	}
}

void UWeaponInventory::TryTriggerReloadAction(UActionHandler* InActionHandler)
{
	if (CurrentWeapon && CurrentWeapon->AmmoHandler && CurrentWeapon->AmmoHandler->CanReload() && InActionHandler)
	{
		FActionRequest ReloadAction = FActionRequest(CurrentWeapon->AmmoData->ReloadAction);
		InActionHandler->TriggerActionEvent(ReloadAction);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to reload weapon"));
	}
}
void UWeaponInventory::ReloadCurrentWeapon()
{
	if (CurrentWeapon && CurrentWeapon->AmmoHandler) CurrentWeapon->AmmoHandler->TriggerReload();
}

void UWeaponInventory::DropInventory()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->DropWeapon();
		CurrentWeapon = nullptr;
	} 
	if (StoredWeapon)
	{
		StoredWeapon->DropWeapon();
		StoredWeapon = nullptr;
	}
	if (SpecialWeapon)
	{
		SpecialWeapon->DropWeapon();
		SpecialWeapon = nullptr;
	}
	for (int32 i = 0; i < RegularGrenades; i++)
	{
		const FRotator SpawnRotation = GetOwner()->GetActorRotation();
		const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(FVector(100.0f, 0.0f, 10.0f));
		AGrenade* OutNade = nullptr;
		if (TryGetGrenade(SpawnLocation, SpawnRotation, OutNade))
		{
			OutNade->PickUpComponent->SetEnabled(true);
		}
	}
	for (int32 i = 0; i < PlasmaGrenades; i++)
	{
		
	}
}