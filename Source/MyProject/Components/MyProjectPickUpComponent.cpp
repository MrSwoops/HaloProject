// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectPickUpComponent.h"

#include <FMODBlueprintStatics.h>

#include "../GameplayCharacter.h"
#include "../Weapons/Weapon.h"
#include "WeaponInventory.h"

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
	if (!Enabled) return;
	if(AGameplayCharacter* Character = Cast<AGameplayCharacter>(OtherActor))
	{
		if (AttachedWeapon) // Has Weapon
		{
			if ((Character->WeaponInventory->PrimaryWeapon && Character->WeaponInventory->PrimaryWeapon->IsSameWeaponType(AttachedWeapon)))
			{
				if (Character->WeaponInventory->PrimaryWeapon->AmmoHandler->LootWeapon(AttachedWeapon->AmmoHandler)) AttachedWeapon->Destroy();
				return;
			}
			if (Character->WeaponInventory->SecondaryWeapon && Character->WeaponInventory->SecondaryWeapon->IsSameWeaponType(AttachedWeapon))
			{
				if (Character->WeaponInventory->SecondaryWeapon->AmmoHandler->LootWeapon(AttachedWeapon->AmmoHandler)) AttachedWeapon->Destroy();
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


