// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/IDamageable.h"
#include "GameplayCharacter.generated.h"

class UAIPerceptionStimuliSourceComponent;
class UEnergyShieldShellSKM;
class UEnergyShield;
class UWeaponInventory;
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

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRespawnCharacter, AGameplayCharacter*, Character);

UCLASS()
class MYPROJECT_API AGameplayCharacter : public ACharacter, public IIDamageable
{
	GENERATED_BODY()
public:
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Blueprintable, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	//USkeletalMeshComponent* CharacterMesh;

public:
	AGameplayCharacter();

#pragma region Ragdoll

	UFUNCTION(BlueprintCallable)
	virtual void SetRagdoll(bool Active);

	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;
	
private:
	FVector MeshLocation;
	FRotator MeshRotation;

#pragma endregion Ragdoll
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void Die();
	UFUNCTION(BlueprintCallable)
	virtual void Respawn(const FVector& Location, const FRotator& Rotation);
	
	bool IsDead = false;
	
#pragma region Weapons
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAIPerceptionStimuliSourceComponent* AIPerceptionStimuli;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWeaponInventory* WeaponInventory;
	
	void PickUpWeapon(AWeapon* Weapon);
	UFUNCTION()
	bool ScavageWeapon(AWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual bool PickUpGrenade(AGrenade* Grenade);

#pragma endregion Weapons


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UEnergyShieldShellSKM* EnergyShieldShell;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UEnergyShield* EnergyShield;
	
	UPROPERTY(BlueprintReadOnly)
	float Health = 0;

	float GetHealthPercent();
	
	UPROPERTY(EditAnywhere)
	float MaxHealth = 45;
	virtual void TakeDamage(IDamageDealer*) override;
	virtual void TakeDamage(const int32&) override;
	virtual void TakeBulletDamage(const FBulletData&, const EHurtboxType&) override;

	UCharacterInteractableComponent* CurrentInteraction = nullptr;
	TArray<UCharacterInteractableComponent*> Interactables;

	virtual void AddInteractable(UCharacterInteractableComponent* Interactable);
	virtual void RemoveInteractable(UCharacterInteractableComponent* Interactable);
	
	UFUNCTION(BlueprintCallable)
	virtual void Move(const FVector2D& Value);
	void Move(const FInputActionValue& Value);
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void Look(const FInputActionValue& Value);
	UPROPERTY(BlueprintReadOnly)
	FRotator DesiredRotation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls")
	float IKRigYawFreedom = 35.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls")
	float RotationSpeed = 5.0f;
	bool bShouldRotateToCamera = false;

	virtual void Crouch();
	virtual void UnCrouch();

	UFUNCTION()
	virtual void TryInteract();

	virtual void SwapWeapons();
	virtual void SwapGrenades();
	virtual void ThrowGrenade();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Reload();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Melee();
	
	
};
