// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "MyProject/Characters/GameplayCharacter.h"
#include "MyProject/Characters/Player/PlayerCharacter.h"
#include "MyProject/EventSystem/EventDefinitions.h"
#include "MyProject/EventSystem/GlobalEventManager.h"
#include "MyProject/ObjectPooling/BulletPoolManager.h"

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
	FString KillLogMessage;
	if (!Msg.Killer) // Suicide
	{
		KillLogMessage = Msg.Victim->CharacterName + " committed suicide";
	}
	else if (Msg.Killer->Team == Msg.Victim->Team) // Betrayal
	{
		KillLogMessage = Msg.Killer->CharacterName + " betrayed " + Msg.Victim->CharacterName;
	}
	else // Kill
	{
		KillLogMessage = Msg.Killer->CharacterName + " killed " + Msg.Victim->CharacterName;
	}
	
	FText KillLogText = FText::FromString(KillLogMessage);//(TEXT("%s received kill event: %s killed %s"), *GetName(), Msg.KillerName, Msg.VictimName));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,                  // Key: Unique ID for the message (-1 for a new message)
			5.0f,                // TimeToDisplay: Duration in seconds to display the message
			FColor::Yellow,      // DisplayColor: Color of the text
			KillLogMessage // DebugMessage: The string to display
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

void ABaseGameMode::InitializeCharacterInventory(UWeaponInventory* Inv)
{
	
}
