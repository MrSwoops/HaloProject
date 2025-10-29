#pragma once
#include "CoreMinimal.h"
#include "MyProject/GameModes/MultiplayerGameModeBase.h"
#include "OddBallGameMode.generated.h"

namespace GlobalEventManager
{
	struct FOddBallScored;
}

UCLASS()
class AOddBallGameMode : public AMultiplayerGameModeBase
{
	GENERATED_BODY()
public:

protected:
	virtual void BeginPlay() override;

	virtual void OddBallScored(const GlobalEventManager::FOddBallScored& Msg);
};
