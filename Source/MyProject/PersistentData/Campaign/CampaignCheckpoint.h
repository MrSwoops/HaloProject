#pragma once

#include "CampaignCheckpoint.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FWeaponCheckpointData
{
	GENERATED_BODY()
public:
};

USTRUCT(Blueprintable, BlueprintType)
struct FCharacterCheckpointData
{
	GENERATED_BODY()
public:
	
};

USTRUCT(Blueprintable, BlueprintType)
struct FCampaignCheckpoint
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelName;
	//MissionProgressPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Checkpoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGuid, FCharacterCheckpointData> SavedCharacters;
};
