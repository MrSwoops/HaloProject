// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MyProject/PersistentData/SettingsPersistentData.h"
#include "SettingsTabsActivatableWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MYPROJECT_API USettingsTabsActivatableWidget : public UCommonActivatableWidget//, public ISettingsPersistentData
{
	GENERATED_BODY()

public:
	//virtual void SaveData_Implementation(UPlayerSettings* SaveFile) override {};
	//virtual void LoadData_Implementation(UPlayerSettings* SaveFile) override {};
};
