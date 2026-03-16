#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ForgeLevelDataFile.generated.h"

class AMultiplayerGameModeBase;

struct ForgeObjectData
{
	//GENERATED_BODY()
	ForgeObjectData(){}
	ForgeObjectData(const FVector& InPosition, const FRotator& InRotation, const int32& InItemID)
	{
		Position = InPosition;
		Rotation = InRotation;
		ItemID = InItemID;
	}

	FVector Position = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	int32 ItemID = 0;
};

UCLASS()
class UForgeLevelDataFile : public USaveGame
{
	GENERATED_BODY()
public:
	FName CustomLevelName;

	TSubclassOf<AMultiplayerGameModeBase> GameMode;
	FName BaseLevelName; 

	bool bIsInvalid; // Invalid if not enough spawns, not all necessary game mode objs are placed
	//TList<ForgeObjectData> ForgeObjects;
};
