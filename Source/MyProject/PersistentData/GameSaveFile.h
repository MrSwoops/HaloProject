// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSaveFile.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UGameSaveFile : public USaveGame
{
	GENERATED_BODY()

public:

#pragma region SP Stats

public:
	int32 GruntsKilled = 0;
	int32 ElitesKilled = 0;

#pragma endregion

#pragma region MP Stats

	int32 MatchesPlayed = 0;
	int32 MatchesWon = 0;
	int32 PlayersKilled = 0;
	int32 Headshots = 0;
	int32 Deaths = 0;
	float KDRatio() { return (static_cast<float>(PlayersKilled) / static_cast<float>(Deaths)); };

#pragma endregion
	
	
};
