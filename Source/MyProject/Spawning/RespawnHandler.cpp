#include "RespawnHandler.h"

#include "Kismet/GameplayStatics.h"
#include "MyProject/GameplayCharacter.h"

void URespawnHandler::RetrieveSpawnPoints()
{
	// SpawnPoints = TMap<FGameplayTag, ACharacterSpawner>();
	// FName SpawnPointTag = "SpawnPoint";
	// TArray<AActor*> SpawnPointsArray;
	// UGameplayStatics::GetAllActorsWithTag(GetWorld(), SpawnPointTag, SpawnPointsArray);
	// for (auto SpawnPointActor : SpawnPointsArray)
	// {
	// 	Cast<>(SpawnPointActor)
	// }
}

void URespawnHandler::RespawnCharacter(AGameplayCharacter* Character, float RespawnDelay)
{
	// if (RespawnDelay == -1) RespawnDelay = RespawnTime;
	// FTimerHandle TimerHandle;
	// FTimerDelegate TimerDelegate;
	// AActor* SpawnPoint = SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)];
	// const FVector& Location = SpawnPoint->GetActorLocation();
	// const FRotator& Rotation = SpawnPoint->GetActorRotation();
	// TimerDelegate.BindUFunction(Character, FName("Respawn"), Location, Rotation);
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, RespawnDelay, false);
}

void URespawnHandler::DetermineBestSpawnPoint()
{
	
}
