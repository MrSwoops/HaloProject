#pragma once
#include "CoreMinimal.h"

class AGameplayCharacter;

namespace GlobalEventManager
{
	struct FPlayerKilledMessage
	{
		AGameplayCharacter* Killer;
		AGameplayCharacter* Victim;
	};

	struct FTestMessage
	{
		FString Message;
	};
}
