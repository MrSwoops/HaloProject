// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameMode.h"
#include "MyProject/AICharacter.h"
#include "TrainingGameMode.generated.h"

class AAICharacter;
/**
 * 
 */
UCLASS()
class MYPROJECT_API ATrainingGameMode : public ABaseGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<AAICharacter*> Dummies;
	TSubclassOf<AAICharacter> DummyClass = AAICharacter::StaticClass();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DummyRespawnTime = 0.5f;

	virtual void BeginPlay() override;

	virtual void RespawnCharacter(AGameplayCharacter* Character, float RespawnDelay) override;

protected:

	virtual void AssignCharacterTags() override;
	
};
