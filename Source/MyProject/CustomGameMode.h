// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustomGameMode.generated.h"

class UBulletPoolManager;
class UActorPool;
class ABullet;

/**
 * 
 */
UCLASS()
class MYPROJECT_API ACustomGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ACustomGameMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBulletPoolManager* BulletPoolManager;

protected:
	virtual void BeginPlay() override;

private:
};

