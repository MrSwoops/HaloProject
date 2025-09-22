// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCharacter.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTakeDamage, const float&, CurrentShields, const float&, MaxShields, const int32&, Damagetaken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateInteractions, bool, Active, const FString&, Text);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FUpdateGrenadesUI, const int32&, CurrentGrenadeSelection, const int32&, RegularGrenadesCount, const int32&, PlasmaGrenadesCount);

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
	virtual void Die() override;

	virtual void SetRagdoll(bool Active) override;

#pragma region UIUpdates
public:
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnTakeDamage OnReceiveDamage;
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FUpdateInteractions UpdateInteractions;
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FUpdateGrenadesUI UpdateGrenadesUI;

#pragma endregion UIUpdates
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	//virtual void SwapWeapons() override;
	virtual void ThrowGrenade() override;
	virtual void SwapGrenades() override;
	virtual bool PickUpGrenade(AGrenade* Grenade) override;

	virtual void TakeDamage(const int32& Damage) override;

	virtual void AddInteractable(UCharacterInteractableComponent* Interactable) override;
	virtual void RemoveInteractable(UCharacterInteractableComponent* Interactable) override;
	
	UFUNCTION()
	void SetCameraPersp(const bool& FirstPerson);
	UFUNCTION()
	void SwapCam();
	
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

#pragma region Inputs/Controls
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Movement Actions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Camera Actions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwapCamAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SwapWeaponsAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SwapGrenadesAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ThrowGrenadeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MeleeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* UseAbilityAction;

protected:
	virtual void NotifyControllerChanged() override;
	
#pragma endregion Inputs/Controls

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
