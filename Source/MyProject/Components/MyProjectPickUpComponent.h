// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterInteractableComponent.h"
#include "MyProjectPickUpComponent.generated.h"

class AWeapon;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class MYPROJECT_API UMyProjectPickUpComponent : public UCharacterInteractableComponent
{
	GENERATED_BODY()

public:
	
	

	virtual void OnInteract(AGameplayCharacter* Character) override;
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY()
	AWeapon* AttachedWeapon = nullptr; 
	
	UMyProjectPickUpComponent();
protected:

	/** Called when the game starts */
	virtual void BeginPlay() override;

};