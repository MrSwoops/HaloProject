// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/IDamageable.h"
#include "Logging/LogMacros.h"
#include "MyProjectCharacter.generated.h"

class USpringArmComponent;
class AWeapon;
class AGrenade;
class UMyProjectWeaponComponent;
class WeaponInventory;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class IInteractable;
class UCharacterInteractableComponent;
struct FInputActionValue;



UCLASS(config=Game)
class AMyProjectCharacter : public ACharacter, public IIDamageable
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh2P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCameraComponent;
	
#pragma region Inputs/Controls
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
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
	class UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MeleeAction;
	
#pragma endregion Inputs/Controls
	
public:
	AMyProjectCharacter();

#pragma region UIUpdates
public:
	// UPROPERTY(BlueprintAssignable, Category = "Interaction")
	// FOnTakeDamage OnReceiveDamage;
	//
	// UPROPERTY(BlueprintAssignable, Category = "Interaction")
	// FUpdateInteractions UpdateInteractions;
	// UPROPERTY(BlueprintAssignable, Category = "Interaction")
	// FUpdateGrenadesUI UpdateGrenadesUI;

#pragma endregion UIUpdates


#pragma region Weapons
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AWeapon* CurrentWeapon = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AWeapon* PrimaryWeapon = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AWeapon* SecondaryWeapon = nullptr;

	void PickUpWeapon(AWeapon* Weapon);
	UFUNCTION()
	bool ScavengeWeapon(AWeapon* Weapon);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 CurrentGrenade = 0;
	int32 RegularGrenades = 2;
	int32 PlasmaGrenades = 2;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool PickUpGrenade(AGrenade* Grenade);

#pragma endregion Weapons

	UPROPERTY(BlueprintReadOnly)
	float Health = 0;
	
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100;
	virtual void TakeDamage(IDamageDealer*) override;
	virtual void TakeDamage(const int32&) override;

	UCharacterInteractableComponent* CurrentInteraction = nullptr;
	TArray<UCharacterInteractableComponent*> Interactables;

	void AddInteractable(UCharacterInteractableComponent* Interactable);
	void RemoveInteractable(UCharacterInteractableComponent* Interactable);

	UFUNCTION()
	void SetCameraPersp(const bool& FirstPerson);
	UFUNCTION()
	void SwapCam();
	
protected:
	virtual void BeginPlay() override;
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void TryInteract();
	
	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

private:
	void SwapWeapons();
	void SwapGrenades();
	void ThrowGrenade();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Reload();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Melee();
	
public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	USkeletalMeshComponent* GetMesh2P() const { return Mesh2P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

