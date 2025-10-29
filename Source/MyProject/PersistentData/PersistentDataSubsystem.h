// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PersistentDataSubsystem.generated.h"

class UPlayerSettings;
class UGameSaveFile;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UPersistentDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	FString SaveFileName = "SaveFile";
	UGameSaveFile* CurrentSaveFile;

	void SaveData();

#pragma region Settings

	FString SettingsFileName = "UserSettings";
	UPlayerSettings* CurrentSettingsFile;

	void SaveSettings();
	void LoadSettings();

#pragma endregion Settings
};
