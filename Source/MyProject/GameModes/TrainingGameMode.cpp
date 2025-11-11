// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainingGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "MyProject/AI/AICharacter.h"

void ATrainingGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), DummyClass, Actors);
	for (auto Actor : Actors)
	{
		if (auto* AIChar = Cast<AAICharacter>(Actor))
			Dummies.Add(AIChar);
	}
}

void ATrainingGameMode::AssignCharacterTags()
{
	
}

void ATrainingGameMode::RespawnCharacter(AGameplayCharacter* Character, float RespawnDelay)
{
	if (Cast<AAICharacter>(Character))
		Super::RespawnCharacter(Character, DummyRespawnTime);
	else
		Super::RespawnCharacter(Character, RespawnDelay);
}

void ATrainingGameMode::InitializeCharacterInventory(UWeaponInventory* Inv)
{
	Inv->InitializeInventoryEquipment(PlayerInventory);
}
