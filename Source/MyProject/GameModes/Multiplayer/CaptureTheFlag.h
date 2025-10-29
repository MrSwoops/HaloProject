#pragma once
#include "CoreMinimal.h"
#include "MyProject/GameModes/MultiplayerGameModeBase.h"
#include "CaptureTheFlag.generated.h"

namespace GlobalEventManager
{
	struct FFlagCaptured;
}

UCLASS()
class ACaptureTheFlag : public AMultiplayerGameModeBase
{
	GENERATED_BODY()
public:


protected:
	virtual void BeginPlay() override;

	void OnFlagCaptured(const GlobalEventManager::FFlagCaptured& Msg);
};
