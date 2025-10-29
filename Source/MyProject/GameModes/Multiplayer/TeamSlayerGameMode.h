#pragma once
#include "MyProject/GameModes/MultiplayerGameModeBase.h"
#include "TeamSlayerGameMode.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ATeamSlayerGameMode : public AMultiplayerGameModeBase
{
	GENERATED_BODY()
public:
	virtual void HandlePlayerDeath(const GlobalEventManager::FPlayerKilledMessage& Msg) override;
	
};
