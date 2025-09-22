// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"

#include "NavigationData.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject/AICharacter.h"
#include "MyProject/PlayerCharacter.h"
#include "MyProject/Components/BulletPoolManager.h"

ABaseGameMode::ABaseGameMode() 
{
	BulletPoolManager = CreateDefaultSubobject<UBulletPoolManager>(TEXT("BulletPoolManager"));
}

void ABaseGameMode::BeginPlay()
{
	//TArray<AActor*> SpawnPoints;
	FName TagToFind = "SpawnPoint";
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TagToFind, SpawnPoints);
	AssignCharacterTags();
}

void ABaseGameMode::AssignCharacterTags()
{
	
}


void ABaseGameMode::RespawnPlayer(APlayerCharacter* Player)
{
	RespawnCharacter(Player);
}


void ABaseGameMode::RespawnCharacter(AGameplayCharacter* Character, float RespawnDelay)
{
	if (RespawnDelay == -1) RespawnDelay = RespawnTime;
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	AActor* SpawnPoint = SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)];
	const FVector& Location = SpawnPoint->GetActorLocation();
	const FRotator& Rotation = SpawnPoint->GetActorRotation();
	TimerDelegate.BindUFunction(Character, FName("Respawn"), Location, Rotation);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, RespawnDelay, false);
}
