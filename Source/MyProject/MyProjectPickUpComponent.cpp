// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectPickUpComponent.h"
#include "MyProjectCharacter.h"
#include "Weapon.h"

UMyProjectPickUpComponent::UMyProjectPickUpComponent()
{
	
}

void UMyProjectPickUpComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMyProjectPickUpComponent::OnInteract(AMyProjectCharacter* Character)
{
	Character->RemoveInteractable(this);
	OnPickUp.Broadcast(Character);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UMyProjectPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Enabled) return;
	if(AMyProjectCharacter* Character = Cast<AMyProjectCharacter>(OtherActor))
	{
		if (AttachedWeapon && ((Character->PrimaryWeapon && Character->PrimaryWeapon->WeaponModel == AttachedWeapon->WeaponModel) ||
			(Character->SecondaryWeapon && Character->SecondaryWeapon->WeaponModel == AttachedWeapon->WeaponModel))) // Has Weapon
		{
			if (Character->ScavengeWeapon(AttachedWeapon))
			{
				AttachedWeapon->Destroy();
			}
		}
		else if (InteractOnAreaEnter)
		{
			OnInteract(Character);
		}
		else
		{
			Character->AddInteractable(this);
		}
	}
}


