// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Campaign/CampaignCheckpoint.h"
#include "Campaign/Skulls.h"
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

#pragma region Campaign

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FSkulls SkullData = FSkulls();
	
#pragma region Campaign Flags
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bStartedCampaign = false;
#pragma endregion Campaign Flags
	
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FCampaignCheckpoint CampaignCheckpoint = FCampaignCheckpoint();
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FCampaignCheckpoint PreviousCheckpoint = FCampaignCheckpoint(); // Used as a backup if primary gets corrupted or if there is a death loop in the new one
	
#pragma endregion Campaign

#pragma region SP Stats

public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	int32 GruntsKilled = 0;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	int32 ElitesKilled = 0;

#pragma endregion

#pragma region MP Stats

	UPROPERTY(SaveGame, BlueprintReadWrite)
	int32 MatchesPlayed = 0;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	int32 MatchesWon = 0;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	int32 PlayersKilled = 0;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	int32 Headshots = 0;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	int32 Deaths = 0;
	float KDRatio() { return (static_cast<float>(PlayersKilled) / static_cast<float>(Deaths)); };

#pragma endregion
	
	
};
