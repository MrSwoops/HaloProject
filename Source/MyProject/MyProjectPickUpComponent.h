// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterInteractableComponent.h"
#include "MyProjectPickUpComponent.generated.h"

class AWeapon;
// Declaration of the delegate that will be called when someone picks this up
// The character picking this up is the parameter sent with the notification
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, AMyProjectCharacter*, PickUpCharacter);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, AMyProjectCharacter*, PickUpCharacter);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class MYPROJECT_API UMyProjectPickUpComponent : public UCharacterInteractableComponent
{
	GENERATED_BODY()

public:
	
	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnPickUp OnPickUp;

	virtual void OnInteract(AMyProjectCharacter* Character) override;
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY()
	AWeapon* AttachedWeapon = nullptr; 
	
	UMyProjectPickUpComponent();
protected:

	/** Called when the game starts */
	virtual void BeginPlay() override;

};