#pragma once

#include "RespawnHandler.generated.h"

class AGameplayCharacter;
class ACharacterSpawner;
struct FGameplayTag;

UCLASS()
class URespawnHandler : public UObject
{
	GENERATED_BODY()
public:

	//TMap<FGameplayTag, ACharacterSpawner> SpawnPoints;
	void RetrieveSpawnPoints();

	void DetermineBestSpawnPoint();
	void RespawnCharacter(AGameplayCharacter* Character, float RespawnDelay);
	 
};