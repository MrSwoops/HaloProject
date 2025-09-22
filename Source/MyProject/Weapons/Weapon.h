// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class ABaseGameMode;
class AGameplayCharacter;
struct FEnhancedInputActionEventBinding;
class AMyProjectCharacter;
class ACustomGameMode;
class UMyProjectPickUpComponent;
class UWeaponUIWidget;
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

	void DisableWeapon();
	void EnableWeapon();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FGameplayTag WeaponType;
	
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	ABaseGameMode* GameMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UMyProjectPickUpComponent* PickUpComp;


	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void AttachWeapon(AGameplayCharacter* TargetCharacter);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	TArray<USoundBase*> FireSounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	USoundBase* DryFireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	TArray<USoundBase*> ReloadSounds;
	
	UPROPERTY(EditDefaultsOnly)
	float FireRate = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UAnimMontage* FireAnimation;
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Fire();
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void FireHeld() {}

	UFUNCTION()
	virtual void ShootBullet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UAnimMontage* ReloadAnimation;
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Reload();
	UPROPERTY()
	FTimerHandle ReloadTimerHandle;
protected:
	UFUNCTION()
	virtual void ReloadMag();

	//FGameplayTagContainer ReloadTags;
	
public:

	UFUNCTION(BlueprintCallable)
	bool IsSameWeaponType(AWeapon* OtherWeapon);
	bool IsSameWeaponType(FGameplayTag TagToCheck);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animations)
	TArray<UAnimMontage*> FPMeleeAnimations;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Audio")
	TArray<USoundBase*> MeleeSounds;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MeleeLungeDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MeleeDamage = 70.0f;
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Melee();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UWeaponUIWidget> WeaponUIClass;
	UPROPERTY()
	UWeaponUIWidget* WeaponUI;

	UPROPERTY(EditDefaultsOnly)
	float Spread = 5.0f;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxMagSize;
	UPROPERTY(EditDefaultsOnly)
	int32 MaxReserveMags;

	int32 CurrentMagAmmo;
	int32 CurrentReserveAmmo;

	virtual float GetCurrentMagPercent() {return static_cast<float>(CurrentMagAmmo) / static_cast<float>(MaxMagSize);}

	UFUNCTION()
	void DropWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	void BindActions(UEnhancedInputComponent* InpComp);
	void UnbindActions(UEnhancedInputComponent* InpComp);
	TMap<UInputAction*, FEnhancedInputActionEventBinding*> Bindings;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	UPROPERTY()
	AGameplayCharacter* Character;

	bool Racked = true;
public:	

	bool IsPlayerOwned = false;

	void UpdateReserveAmmoUI();
};
