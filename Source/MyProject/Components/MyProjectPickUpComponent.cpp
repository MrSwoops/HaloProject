// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectPickUpComponent.h"

#include <FMODBlueprintStatics.h>

#include "../Weapons/Weapon.h"
#include "MyProject/Characters/GameplayCharacter.h"
#include "MyProject/Weapons/WeaponInventory.h"

UMyProjectPickUpComponent::UMyProjectPickUpComponent()
{
	
}

void UMyProjectPickUpComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UMyProjectPickUpComponent::OnInteract(AGameplayCharacter* Character)
{
	Character->RemoveInteractable(this);
	OnInteractDelegate.Broadcast(Character);
}

void UMyProjectPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bEnabled) return;
	if(AGameplayCharacter* Character = Cast<AGameplayCharacter>(OtherActor))
	{
		if (AttachedWeapon) // Has Weapon
		{
			if ((Character->WeaponInventory->CurrentWeapon && Character->WeaponInventory->CurrentWeapon->IsSameWeaponType(AttachedWeapon)))
			{
				if (Character->WeaponInventory->CurrentWeapon->AmmoHandler->LootWeapon(AttachedWeapon->AmmoHandler)) AttachedWeapon->Destroy();
				return;
			}
			if (Character->WeaponInventory->StoredWeapon && Character->WeaponInventory->StoredWeapon->IsSameWeaponType(AttachedWeapon))
			{
				if (Character->WeaponInventory->StoredWeapon->AmmoHandler->LootWeapon(AttachedWeapon->AmmoHandler)) AttachedWeapon->Destroy();
				return;
			}
		}
		
		if (InteractOnAreaEnter)
		{
			OnInteract(Character);
		}
		else
		{
			Character->AddInteractable(this);
		}
	}
}


