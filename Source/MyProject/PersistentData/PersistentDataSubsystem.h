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
	static const FString SaveFileName;

	UPROPERTY(BlueprintReadOnly)
	UGameSaveFile* CurrentSaveFile;

	void MakeNewSaveFile();

	void SaveData();
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
