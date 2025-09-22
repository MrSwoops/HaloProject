// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

class AGameplayCharacter;
class APlayerCharacter;
class UBulletPoolManager;
/**
 * 
 */
UCLASS()
class MYPROJECT_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ABaseGameMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBulletPoolManager* BulletPoolManager;

	
	UPROPERTY()
	float RespawnTime = 5.0f;
	UFUNCTION()
	virtual void RespawnPlayer(APlayerCharacter* Player);
	UFUNCTION()
	virtual void RespawnCharacter(AGameplayCharacter* Character, float RespawnDelay = -1);
	TArray<AActor*> SpawnPoints;

protected:
	virtual void BeginPlay() override;

	virtual void AssignCharacterTags();
	
};
