// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyProjectWeaponComponent.generated.h"

class UMyProjectPickUpComponent;
class UWeaponUIWidget;
class AMyProjectCharacter;
class ACustomGameMode;
class UUserWidget;
class WidgetT;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UMyProjectWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:

	UPROPERTY()
	ACustomGameMode* GameMode;

	UPROPERTY(EditDefaultsOnly)
	float FireRate = 0.1f;

	

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** Sets default values for this component's properties */
	UMyProjectWeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool AttachWeapon(AMyProjectCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Fire();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Reload();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Melee();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UWeaponUIWidget> WeaponUIClass;
	UPROPERTY()
	UWeaponUIWidget* WeaponUI;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxMagSize;
	UPROPERTY(EditDefaultsOnly)
	int32 MaxReserveMags;

	int32 CurrentMagAmmo;
	int32 CurrentReserveAmmo;

	UPROPERTY(BlueprintReadOnly)
	UMyProjectPickUpComponent* PickUpComponent;
	
protected:
	virtual void BeginPlay() override;

	/** The Character holding this weapon*/
	AMyProjectCharacter* Character;

private:
};
