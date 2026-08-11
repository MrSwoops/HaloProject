// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Framework/WeaponAmmoHandler.h"
#include "Framework/WeaponFireHandler.h"
#include "GameFramework/Actor.h"
#include "MyProject/GameModes/BaseGameMode.h"
#include "Weapon.generated.h"

class UCharacterActionEvent;
enum class EWeaponStorageSlot : uint8;
class UWeaponFireData;
class UWeaponFireHandler;
class UWeaponAmmoHandler;
class UWeaponAmmoData;
class ABaseGameMode;
class AGameplayCharacter;
struct FEnhancedInputActionEventBinding;
class AMyProjectCharacter;
class ACustomGameMode;
class UMyProjectPickUpComponent;
class UWeaponUIWidget;
class UFMODEvent;
struct FGameplayTag;

namespace WeaponTags
{
	static const FGameplayTag M90    = FGameplayTag::RequestGameplayTag(FName("Weapon.UNSC.M90"));
	static const FGameplayTag M99    = FGameplayTag::RequestGameplayTag(FName("Weapon.UNSC.M99"));
	static const FGameplayTag M9     = FGameplayTag::RequestGameplayTag(FName("Weapon.UNSC.M9"));
	static const FGameplayTag SP4NKR = FGameplayTag::RequestGameplayTag(FName("Weapon.UNSC.SP4NKR"));
	static const FGameplayTag BR55 = FGameplayTag::RequestGameplayTag(FName("Weapon.UNSC.BR55"));
	static const FGameplayTag SRS = FGameplayTag::RequestGameplayTag(FName("Weapon.UNSC.SRS_99D"));
	static const FGameplayTag PlasPistol = FGameplayTag::RequestGameplayTag(FName("Weapon.Cov.PlasmaPistol"));
}

UCLASS()
class MYPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	AWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FGameplayTag WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	EWeaponStorageSlot StorageSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName PreferredStorageSocket = "socket_BackWeapon1";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName BackupStorageSocket = "socket_ThighWeaponRight";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName CurrentStorageSocket = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Actions")
	UCharacterActionEvent* MeleeActionEvent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Actions")
	UCharacterActionEvent* ReloadActionEvent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UMyProjectPickUpComponent* PickUpComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* GunMuzzle;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;


	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void OnPickup(AGameplayCharacter* TargetCharacter);

	void SetPlayerOwned(const bool& bPlayerOwned);
	UFUNCTION()
	void DropWeapon();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	UFMODEvent* ScavageSoundEvent;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Reload();
	
public:

	UFUNCTION(BlueprintCallable)
	bool IsSameWeaponType(AWeapon* OtherWeapon);
	bool IsSameWeaponType(FGameplayTag TagToCheck);
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void SetMeleeHitBox(const bool& bActive);

	UPROPERTY()
	UWeaponUIWidget* WeaponUI;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
public:
	UPROPERTY()
	AGameplayCharacter* Character;

	bool IsPlayerOwned = false;

	////////////////////////////////////////// New Weapon Stuff //////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void FirePressed();
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void FireReleased();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	UWeaponFireData* FireData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	UWeaponAmmoData* AmmoData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	UProjectileData* ProjectileData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	UWeaponUIData* UIData;

	UPROPERTY(BlueprintReadOnly)
	UWeaponFireHandler* FireHandler;
	UPROPERTY(BlueprintReadOnly)
	UWeaponAmmoHandler* AmmoHandler;

	void InitializeWeapon();
	
};
