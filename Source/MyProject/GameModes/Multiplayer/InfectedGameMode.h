#pragma once
#include "CoreMinimal.h"
#include "MyProject/GameModes/MultiplayerGameModeBase.h"
#include "InfectedGameMode.generated.h"

UCLASS(Blueprintable)
class AInfectedGameMode : public AMultiplayerGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FInventoryDefault DefaultInfectedInventory;
};
