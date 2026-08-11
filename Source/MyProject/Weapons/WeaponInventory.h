// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponInventory.generated.h"


struct FGameplayTag;
class UCharacterActionEvent;
class UActionHandler;
class AGameplayCharacter;
class AEquipment;
class AGrenade;
class AWeapon;

USTRUCT(BlueprintType)
struct FInventoryDefault
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|DefaultOverrides")
	TSubclassOf<AWeapon> StartingPrimary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|DefaultOverrides")
	TSubclassOf<AWeapon> StartingSecondary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|DefaultOverrides")
	int32 StartingFragGrenades;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|DefaultOverrides")
	int32 StartingPlasmaGrenades;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|DefaultOverrides")
	TSubclassOf<AEquipment> StartingEquipment;
};

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AWeapon* SpecialWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AWeapon* CurrentWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AWeapon* StoredWeapon = nullptr;

	UPROPERTY()
	AEquipment* EquipmentSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|DefaultOverrides")
	bool bOverrideDefaultInventory = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|DefaultOverrides", meta = (EditCondition = "bOverrideDefaultInventory", EditConditionHides))
	FInventoryDefault DefaultInventory;

	UPROPERTY()
	AGameplayCharacter* OwnerCharacter;
	
	void InitializeInventory();
	void InitializeInventoryEquipment(FInventoryDefault& Inventory);
private:
	UFUNCTION()
	void InitializeInventoryEquipment(TSubclassOf<AWeapon>& InStartingPrimary, TSubclassOf<AWeapon>& InStartingSecondary, int32& InStartingFragGrenades, int32& InStartingPlasmaGrenades, TSubclassOf<AEquipment>& InStartingEquipment);
		
public:
	UFUNCTION()
	bool ScavageWeapon(AWeapon* Weapon);
	
	void TryMeleeWeapon(UActionHandler* InActionHandler);

	// Reloading
	void TryTriggerReloadAction(UActionHandler* InActionHandler);
	UFUNCTION(BlueprintCallable)
	void ReloadCurrentWeapon();

	// Weapon swapping
	void PickUpWeapon(AWeapon* Weapon);
	
	void TryTriggerSwapWeaponsAction(UActionHandler* InActionHandler);
	UFUNCTION(BlueprintCallable, Category = "AnimNotify")
	void StoreCurrentWeapon(const FName& AttachSocketName = FName(TEXT("socket_BackWeapon1")));
	UFUNCTION(BlueprintCallable, Category = "AnimNotify")
	void SwapWeapons();
	UFUNCTION(BlueprintCallable, Category = "AnimNotify")
	void GrabCurrentWeapon();
	UFUNCTION() 

	UCharacterActionEvent* DetermineSwapEvent(const bool& Store, const FName& InSocketName);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory|SwapActions|BackSlot1")
	UCharacterActionEvent* StoreBackWeapon1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory|SwapActions|BackSlot1")
	UCharacterActionEvent* RetrieveBackWeapon1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory|SwapActions|BackSlot2")
	UCharacterActionEvent* StoreBackWeapon2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory|SwapActions|BackSlot2")
	UCharacterActionEvent* RetrieveBackWeapon2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory|SwapActions|RightThighSlot")
	UCharacterActionEvent* StoreThighRightWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory|SwapActions|RightThighSlot")
	UCharacterActionEvent* RetrieveThighRightWeapon;

	void DropInventory();

#pragma region Equipment
public:
	void PickUpEquipment(AEquipment* Equipment);
#pragma endregion Equipment

#pragma region Grenades
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 CurrentGrenade = 0;
	int32 RegularGrenades = 2;
	int32 PlasmaGrenades = 2;
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool HasGrenade(const int32& GrenadeType);
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool PickUpGrenade(AGrenade* Grenade);
	
	void SwapGrenades();

	bool TryGetGrenade(const FVector& SpawnLocation, const FRotator& SpawnRotation, AGrenade*& OutGrenade);
#pragma endregion Grenades
};
