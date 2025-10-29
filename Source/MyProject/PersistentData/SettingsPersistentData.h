// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SettingsPersistentData.generated.h"

class UPlayerSettings;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class USettingsPersistentData : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT_API ISettingsPersistentData
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SaveLoad")
	void SaveData(UPlayerSettings* SaveFile);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SaveLoad")
	void LoadData(UPlayerSettings* SaveFile);
};
