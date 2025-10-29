// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"

#include "NavigationData.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject/AICharacter.h"
#include "MyProject/PlayerCharacter.h"
#include "MyProject/Components/BulletPoolManager.h"
#include "MyProject/EventSystem/EventDefinitions.h"
#include "MyProject/EventSystem/GlobalEventManager.h"

ABaseGameMode::ABaseGameMode() 
{
	BulletPoolManager = CreateDefaultSubobject<UBulletPoolManager>(TEXT("BulletPoolManager"));
}

void ABaseGameMode::BeginPlay()
{
	//TArray<AActor*> SpawnPoints;
	FName SpawnPointTag = "SpawnPoint";
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), SpawnPointTag, SpawnPoints);
	AssignCharacterTags();

	GlobalEventManager::FGlobalEventManager::Subscribe<ABaseGameMode, GlobalEventManager::FPlayerKilledMessage>(
	this,
	[this](const GlobalEventManager::FPlayerKilledMessage& Msg)
	{
		HandlePlayerDeath(Msg);
	},
	false,  // onlyTriggerIfActive
	false);   // triggerOnce
}

void ABaseGameMode::HandlePlayerDeath(const GlobalEventManager::FPlayerKilledMessage& Msg)
{
	//(Msg.Killer->Team == Msg.Victim->Team) ? " betrayed " : " killed "
	FString S = Msg.Killer->CharacterName + " killed " + Msg.Victim->CharacterName;
	FText T = FText::FromString(S);//(TEXT("%s received kill event: %s killed %s"), *GetName(), Msg.KillerName, Msg.VictimName));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,                  // Key: Unique ID for the message (-1 for a new message)
			5.0f,                // TimeToDisplay: Duration in seconds to display the message
			FColor::Yellow,      // DisplayColor: Color of the text
			S // DebugMessage: The string to display
		);
	}
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
