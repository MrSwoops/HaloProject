// Fill out your copyright notice in the Description page of Project Settings.


#include "PersistentDataSubsystem.h"

#include "GameSaveFile.h"
#include "PersistentDataSubscriber.h"
#include "PlayerSettings.h"
#include "SettingsPersistentData.h"
#include "InterfaceRegistry/SettingsInterfaceRegistry.h"
#include "Kismet/GameplayStatics.h"

void UPersistentDataSubsystem::SaveData()
{
	if (!CurrentSaveFile) CurrentSaveFile = Cast<UGameSaveFile>(UGameplayStatics::CreateSaveGameObject(UGameSaveFile::StaticClass()));
	if (auto* World = GetWorld())
	{
		TArray<AActor*> PersistentActors;
		UGameplayStatics::GetAllActorsWithInterface(World, UPersistentDataSubscriber::StaticClass(), PersistentActors);
		for (AActor* Actor : PersistentActors)
		{
			Cast<IPersistentDataSubscriber>(Actor)->SaveData(CurrentSaveFile);
		}
	}
	UGameplayStatics::AsyncSaveGameToSlot(CurrentSaveFile, SaveFileName, 0);
}


void UPersistentDataSubsystem::SaveSettings()
{
	if (CurrentSettingsFile == nullptr) CurrentSettingsFile = Cast<UPlayerSettings>(UGameplayStatics::CreateSaveGameObject(UPlayerSettings::StaticClass()));
	if (auto* World = GetWorld())
	{
		TArray<AActor*> PersistentActors;
		UGameplayStatics::GetAllActorsWithInterface(World, USettingsPersistentData::StaticClass(), PersistentActors);
		for (AActor* Actor : PersistentActors)
		{
			if (Actor) ISettingsPersistentData::Execute_SaveData(Actor, CurrentSettingsFile);
			//Cast<ISettingsPersistentData>(Actor)->SaveData(CurrentSettingsFile);
		}
		// USettingsInterfaceRegistry* Registry = USettingsInterfaceRegistry::Get();
		// for (const TWeakObjectPtr<UObject>& ObjPtr : Registry->GetAll())
		// {
		// 	if (UObject* Obj = ObjPtr.Get())
		// 	{
		// 		ISettingsPersistentData::Execute_SaveData(Obj, CurrentSettingsFile);
		// 	}
		// }
	}
	UGameplayStatics::AsyncSaveGameToSlot(CurrentSettingsFile, SettingsFileName, 0);
}

void UPersistentDataSubsystem::LoadSettings()
{
	if (CurrentSettingsFile == nullptr) CurrentSettingsFile = Cast<UPlayerSettings>(UGameplayStatics::LoadGameFromSlot(SettingsFileName, 0));
	if (CurrentSettingsFile == nullptr) CurrentSettingsFile = Cast<UPlayerSettings>(UGameplayStatics::CreateSaveGameObject(UPlayerSettings::StaticClass()));
	
	if (auto* World = GetWorld())
	{
		TArray<AActor*> PersistentActors;
		UGameplayStatics::GetAllActorsWithInterface(World, USettingsPersistentData::StaticClass(), PersistentActors);
		for (AActor* Actor : PersistentActors)
		{
			ISettingsPersistentData::Execute_LoadData(Actor, CurrentSettingsFile);
			//Cast<ISettingsPersistentData>(Actor)->LoadData(CurrentSettingsFile);
		}
		// USettingsInterfaceRegistry* Registry = USettingsInterfaceRegistry::Get();
		// for (const TWeakObjectPtr<UObject>& ObjPtr : Registry->GetAll())
		// {
		// 	if (UObject* Obj = ObjPtr.Get())
		// 	{
		// 		ISettingsPersistentData::Execute_LoadData(Obj, CurrentSettingsFile);
		// 	}
		// }
	}
}
