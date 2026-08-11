// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/Characters/GameplayCharacter.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTakeDamage, const float&, CurrentShields, const float&, MaxShields, const int32&, Damagetaken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateInteractions, bool, Active, const FString&, Text);


/**
 * 
 */
UCLASS()
class MYPROJECT_API APlayerCharacter : public AGameplayCharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();

	virtual void BeginPlay() override;

	virtual void Respawn(const FVector& Location, const FRotator& Rotation) override;;
	virtual void Die_Implementation() override;

	virtual void SetRagdoll(bool Active) override;

#pragma region UIUpdates
public:
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnTakeDamage OnReceiveDamage;
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FUpdateInteractions UpdateInteractions;
	

#pragma endregion UIUpdates
	
	//virtual void SwapWeapons() override;
	virtual void SwapGrenades() override;
	virtual bool PickUpGrenade(AGrenade* Grenade) override;

	virtual void TakeDamage_Implementation(const FDamageMessage& DmgMsg) override;

	virtual void AddInteractable(UCharacterInteractableComponent* Interactable) override;
	virtual void RemoveInteractable(UCharacterInteractableComponent* Interactable) override;
	
	UFUNCTION()
	void SetCameraPersp(const bool& FirstPerson);
	UFUNCTION()
	void SwapCam();
	
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

public:
	
	/** Cameras */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCameraComponent;

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return FirstPersonMesh; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
};
