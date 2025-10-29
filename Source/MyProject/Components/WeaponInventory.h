// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponInventory.generated.h"


class AEquipment;
class AGrenade;
class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UWeaponInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere)
	AWeapon* CurrentWeapon = nullptr;
	UPROPERTY()
	AWeapon* PrimaryWeapon = nullptr;
	UPROPERTY()
	AWeapon* SecondaryWeapon = nullptr;

	UPROPERTY()
	AEquipment* EquipmentSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon> StartingPrimary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon> StartingSecondary;

	void PickUpWeapon(AWeapon* Weapon);
	void PickUpWeapon(UEnhancedInputComponent* EnhancedInputComponent, AWeapon* Weapon);
	UFUNCTION()
	bool ScavageWeapon(AWeapon* Weapon);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 CurrentGrenade = 0;
	int32 RegularGrenades = 2;
	int32 PlasmaGrenades = 2;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool PickUpGrenade(AGrenade* Grenade);
	void SwapGrenades();

	
	bool TryGetGrenade(const FVector& SpawnLocation, const FRotator& SpawnRotation, AGrenade*& OutGrenade);
	bool TryMeleeWeapon();

	void TryReloadWeapon();
	void SwapWeapons();
	void SwapWeapons(UEnhancedInputComponent* EnhancedInputComponent);

	void PickUpEquipment(AEquipment* Equipment);

	void InitializeInventory();

	void DropInventory();
};
