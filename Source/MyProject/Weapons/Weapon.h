// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class AGameplayCharacter;
struct FEnhancedInputActionEventBinding;
class AMyProjectCharacter;
class ACustomGameMode;
class UMyProjectPickUpComponent;
class UWeaponUIWidget;

UENUM(BlueprintType, meta = (Bitflags = "true"))
enum EWeapon : int
{
	TestWeapon,
	M90,
	M6CSOC,
	MA5C,
	BR55,
	M99,
	M9,
	
	
};

UCLASS()
class MYPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	AWeapon();

	void DisableWeapon();
	void EnableWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TEnumAsByte<EWeapon> WeaponModel;
	
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	ACustomGameMode* GameMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UMyProjectPickUpComponent* PickUpComp;


	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AGameplayCharacter* TargetCharacter);

	UPROPERTY(EditDefaultsOnly)
	float FireRate = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	TArray<USoundBase*> FireSounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Fire();
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void FireHeld() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* ReloadAnimation;
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Reload();
	UPROPERTY()
	FTimerHandle ReloadTimerHandle;
protected:
	UFUNCTION()
	virtual void ReloadMag();
	
public:

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

	UFUNCTION()
	virtual void ShootBullet();

	/** The Character holding this weapon*/
	UPROPERTY()
	AGameplayCharacter* Character;

	bool Racked = true;
public:	

	bool IsPlayerOwned = false;

	void UpdateReserveAmmoUI();
};
