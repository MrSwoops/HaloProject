// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterInteractableComponent.h"

#include "GameplayCharacter.h"

// Sets default values for this component's properties
UCharacterInteractableComponent::UCharacterInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SphereRadius = 32.f;
}


// Called when the game starts
void UCharacterInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UCharacterInteractableComponent::OnSphereBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UCharacterInteractableComponent::OnSphereEndOverlap);
	// ...
	
}

void UCharacterInteractableComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	if (!Enabled) return;
	if(AGameplayCharacter* Character = Cast<AGameplayCharacter>(OtherActor))
	{
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

void UCharacterInteractableComponent::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!Enabled) return;
	if(AGameplayCharacter* Character = Cast<AGameplayCharacter>(OtherActor))
	{
		Character->RemoveInteractable(this);
	}
}

void UCharacterInteractableComponent::OnInteract(AGameplayCharacter* Character)
{
	
}

