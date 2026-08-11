// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyProject/Combat/DamageLog.h"
#include "MyProject/Combat/Interfaces/Damageable.h"
#include "MyProject/Combat/Interfaces/IDamageable.h"
#include "GameplayCharacter.generated.h"

class UActionHandler;
class UProjectileData;
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FUpdateGrenadesUI, const int32&, CurrentGrenadeSelection, const int32&, RegularGrenadesCount, const int32&, PlasmaGrenadesCount);

UCLASS()
class MYPROJECT_API AGameplayCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()
public:
	AGameplayCharacter();

#pragma region Ragdoll
public:
	UFUNCTION(BlueprintCallable)
	virtual void SetRagdoll(bool Active);
protected:
	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;
private:
	FVector MeshLocation;
	FRotator MeshRotation;
#pragma endregion Ragdoll
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void Die_Implementation();
	UFUNCTION(BlueprintCallable)
	virtual void Respawn(const FVector& Location, const FRotator& Rotation);

	UPROPERTY(EditDefaultsOnly)
	FString CharacterName;
	UPROPERTY(EditDefaultsOnly)
	int32 Team;
	
	bool bIsDead = false;

	FDamageLog DamageLog = FDamageLog();

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FUpdateGrenadesUI UpdateGrenadesUI;
	
#pragma region Character Actions

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UActionHandler* ActionHandler;
	UFUNCTION()
	virtual void OnActionEndCleanup();

#pragma endregion Character Actions

#pragma region Combat Registration
public:
	UFUNCTION(BlueprintCallable)
	virtual void EnterCombat(); 
	UFUNCTION(BlueprintCallable)
	virtual void StartExitCombatTimer(bool OverwriteCurrent = true);
	UFUNCTION(BlueprintCallable)
	virtual void ExitCombat() {IsInCombat = false;}
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsInCombat = false;
protected:
	float ExitCombatTime = 10.0f;
	FTimerHandle ExitCombatTimer;

public:
	UFUNCTION(BlueprintCallable)
	virtual void OnStartTakingFire(const FDamageMessage& DmgMsg);
	UFUNCTION(BlueprintCallable)
	virtual void OnStopTakingFire();
	UPROPERTY(BlueprintReadOnly)
	bool IsUnderFire;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool UnderFireTime;
private:
	FTimerHandle UnderFireTimer;
#pragma endregion Combat Registration

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAIPerceptionStimuliSourceComponent* AIPerceptionStimuli;
	
#pragma region Weapons
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWeaponInventory* WeaponInventory;
	
	void PickUpWeapon(AWeapon* Weapon);
	UFUNCTION()
	bool ScavageWeapon(AWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual bool PickUpGrenade(AGrenade* Grenade);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCharacterActionEvent* GrenadeThrowAction;

#pragma endregion Weapons

	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UEnergyShieldShellSKM* EnergyShieldShell;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UEnergyShield* EnergyShield;
	
	UPROPERTY(BlueprintReadOnly)
	float Health = 0;

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent();
	
	UPROPERTY(EditAnywhere)
	float MaxHealth = 45;
	virtual void TakeDamage_Implementation(const FDamageMessage& DmgMsg) override;
	virtual bool IsDead_Implementation() const override { return bIsDead; }

	UPROPERTY()
	UCharacterInteractableComponent* CurrentInteraction = nullptr;
	UPROPERTY()
	TArray<UCharacterInteractableComponent*> Interactables;

	virtual void AddInteractable(UCharacterInteractableComponent* Interactable);
	virtual void RemoveInteractable(UCharacterInteractableComponent* Interactable);
	
	UFUNCTION(BlueprintCallable)
	virtual void Move(const FVector2D& Value);
	void Move(const FInputActionValue& Value);
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void Look(const FInputActionValue& Value);
	UPROPERTY(BlueprintReadOnly)
	FRotator DesiredRotation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls")
	float IKRigYawFreedom = 35.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls")
	float RotationSpeed = 5.0f;
	bool bShouldRotateToCamera = false;
	void UpdateLookRotation(const float& DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	virtual void CharacterCrouch();
	UFUNCTION(BlueprintCallable)
	virtual void CharacterUnCrouch();

	UFUNCTION()
	virtual void TryInteract();

	virtual void SwapWeapons();
	virtual void FirePressed();
	virtual void FireReleased();

	
	virtual void SwapGrenades();
	UFUNCTION(BlueprintCallable)
	virtual void PrepGrenade();
	UFUNCTION(BlueprintCallable)
	virtual void ReleaseGrenade();
	UPROPERTY()
	AGrenade* GrenadeToThrow;
	void TriggerGrenadeThrow();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void TriggerReloadAction();

	UFUNCTION(BlueprintCallable, Category="AnimNotify")
	void ReloadCurrentWeapon();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Melee();

	UPROPERTY()
	USceneComponent* VisionPoint;

	UFUNCTION(BlueprintCallable, Category = "Character Materials")
	void SetCharacterColor(const FLinearColor& NewColor, int32 MaterialIndex = -1);

private:
	void AssignDynamicMaterials();
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterialInstances;
	

};
