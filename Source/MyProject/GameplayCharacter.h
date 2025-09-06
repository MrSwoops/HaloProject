// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/IDamageable.h"
#include "GameplayCharacter.generated.h"

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


UCLASS()
class MYPROJECT_API AGameplayCharacter : public ACharacter, public IIDamageable
{
	GENERATED_BODY()
public:
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Blueprintable, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	//USkeletalMeshComponent* CharacterMesh;

public:
	AGameplayCharacter();

	UFUNCTION(BlueprintCallable)
	virtual void SetRagdoll(bool Active);

	

	UFUNCTION(BlueprintCallable)
	virtual void Die();

	bool IsDead = false;
	
#pragma region Weapons
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWeaponInventory* WeaponInventory;
	
	void PickUpWeapon(AWeapon* Weapon);
	UFUNCTION()
	bool ScavengeWeapon(AWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual bool PickUpGrenade(AGrenade* Grenade);

#pragma endregion Weapons

	UPROPERTY(BlueprintReadOnly)
	float Health = 0;
	
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100;
	virtual void TakeDamage(IDamageDealer*) override;
	virtual void TakeDamage(const int32&) override;

	UCharacterInteractableComponent* CurrentInteraction = nullptr;
	TArray<UCharacterInteractableComponent*> Interactables;

	virtual void AddInteractable(UCharacterInteractableComponent* Interactable);
	virtual void RemoveInteractable(UCharacterInteractableComponent* Interactable);

	
	
protected:
	virtual void BeginPlay() override;
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	virtual void TryInteract();

	virtual void SwapWeapons();
	virtual void SwapGrenades();
	virtual void ThrowGrenade();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Reload();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Melee();
	
public:
	
	//USkeletalMeshComponent* GetMesh2P() const { return CharacterMesh; }
	
};
