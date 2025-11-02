// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

class UWeaponInventory;

namespace GlobalEventManager
{
	struct FPlayerKilledMessage;
}

class URespawnHandler;
class AGameplayCharacter;
class APlayerCharacter;
class UBulletPoolManager;
/**
 * 
 */
//DECLARE_DYNAMIC_MULTICAST_DELEGATE
UCLASS()
class MYPROJECT_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ABaseGameMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBulletPoolManager* BulletPoolManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URespawnHandler* RespawnHandler;
	
	UPROPERTY()
	float RespawnTime = 5.0f;
	UFUNCTION()
	virtual void RespawnPlayer(APlayerCharacter* Player);
	UFUNCTION()
	virtual void RespawnCharacter(AGameplayCharacter* Character, float RespawnDelay = -1);
	TArray<AActor*> SpawnPoints;

	virtual void InitializeCharacterInventory(UWeaponInventory* Inv);

protected:
	virtual void BeginPlay() override;

	virtual void AssignCharacterTags();

	virtual void HandlePlayerDeath(const GlobalEventManager::FPlayerKilledMessage& Msg);
	
};
