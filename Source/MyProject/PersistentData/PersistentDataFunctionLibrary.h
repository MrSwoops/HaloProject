// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PersistentDataFunctionLibrary.generated.h"

class UGameSaveFile;
class UPlayerSettingsFile;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UPersistentDataFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	// UFUNCTION(BlueprintCallable, Category="Persistent Data|User Settings")
	// static UGameSaveFile* GetGameSaveFile(UObject* ReferenceObject);
	
	UFUNCTION(BlueprintCallable, Category="Persistent Data")
	static void SaveGame(UObject* ReferenceObject);

	UFUNCTION(BlueprintCallable, Category="Persistent Data")
	static void LoadGame(UObject* ReferenceObject);

	// User Settings
	
	UFUNCTION(BlueprintCallable, Category="Persistent Data|User Settings")
	static UPlayerSettingsFile* GetUserSettingsFile(UObject* ReferenceObject);
};
