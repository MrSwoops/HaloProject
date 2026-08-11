// Fill out your copyright notice in the Description page of Project Settings.


#include "PersistentDataSubsystem.h"

#include "GameSaveFile.h"
#include "PersistentDataSubscriber.h"
#include "Kismet/GameplayStatics.h"

const FString UPersistentDataSubsystem::SaveFileName = "SaveFile";

void UPersistentDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CurrentSaveFile = Cast<UGameSaveFile>(UGameplayStatics::LoadGameFromSlot(SaveFileName, 0));

	if (!CurrentSaveFile)
	{
		MakeNewSaveFile();
	}
}

void UPersistentDataSubsystem::MakeNewSaveFile()
{
	CurrentSaveFile = Cast<UGameSaveFile>(UGameplayStatics::CreateSaveGameObject(UGameSaveFile::StaticClass()));
	UGameplayStatics::AsyncSaveGameToSlot(CurrentSaveFile, SaveFileName, 0);
}

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
