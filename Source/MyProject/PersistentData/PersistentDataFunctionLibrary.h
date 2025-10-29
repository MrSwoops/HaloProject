// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PersistentDataFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UPersistentDataFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category="Persistent Data")
	static void SaveGame(UObject* ReferenceObject);

	UFUNCTION(BlueprintCallable, Category="Persistent Data")
	static void LoadGame(UObject* ReferenceObject);

	UFUNCTION(BlueprintCallable, Category="Persistent Data")
	static void SaveSettings(UObject* ReferenceObject);

	UFUNCTION(BlueprintCallable, Category="Persistent Data")
	static void LoadSettings(UObject* ReferenceObject);
};
