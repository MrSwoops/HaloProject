// Fill out your copyright notice in the Description page of Project Settings.


#include "PersistentDataFunctionLibrary.h"

#include "PersistentDataSubsystem.h"
#include "Settings/PlayerSettingsFile.h"

void UPersistentDataFunctionLibrary::SaveGame(UObject* ReferenceObject)
{
	// UWorld* World = GEngine->GetWorldFromContextObject(ReferenceObject, EGetWorldErrorMode::ReturnNull);
	// if (!World) return;
	// World->GetGameInstance()->GetSubsystem<UPersistentDataSubsystem>()->;
}

void UPersistentDataFunctionLibrary::LoadGame(UObject* ReferenceObject)
{
	// UWorld* World = GEngine->GetWorldFromContextObject(ReferenceObject, EGetWorldErrorMode::ReturnNull);
	// if (!World) return;
	// World->GetGameInstance()->GetSubsystem<UPersistentDataSubsystem>()->;
}

UPlayerSettingsFile* UPersistentDataFunctionLibrary::GetUserSettingsFile(UObject* ReferenceObject)
{
	return UPlayerSettingsFile::GetPlayerSettings();
}
