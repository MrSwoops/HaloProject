#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MyProject/Forge/ForgeObject/ForgeObjectSettings.h"
#include "ForgeLevelDataFile.generated.h"

class AMultiplayerGameModeBase;

USTRUCT()
struct FForgeObjectData
{
	GENERATED_BODY()
	FForgeObjectData(){}
	FForgeObjectData(const FVector& InPosition, const FRotator& InRotation, const int32& InItemID, const FObjectSettings& InObjectSettings)
	{
		Position = InPosition;
		Rotation = InRotation;
		ItemID = InItemID;
		ObjectSettings = InObjectSettings;
	}
	UPROPERTY(SaveGame)
	FVector Position = FVector::ZeroVector;
	UPROPERTY(SaveGame)
	FRotator Rotation = FRotator::ZeroRotator;
	int32 ItemID = 0;
	FObjectSettings ObjectSettings = FObjectSettings();
};

UCLASS()
class UForgeLevelDataFile : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(SaveGame)
	FName CustomLevelName;

	UPROPERTY(SaveGame)
	TSubclassOf<AMultiplayerGameModeBase> GameMode;
	UPROPERTY(SaveGame)
	FName BaseLevelName; 

	UPROPERTY(SaveGame)
	bool bIsInvalid; // Invalid if not enough spawns, invalid spawns, not all necessary game mode objs are placed

	//UPROPERTY(SaveGame)
	//TArray<FForgeObjectData> ForgeObjects;

	//static void SaveForgeData();
};
