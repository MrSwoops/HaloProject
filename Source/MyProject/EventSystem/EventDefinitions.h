#pragma once
#include "CoreMinimal.h"

struct FDamageLog;
class AOddBall;
class AFlag;
class AGameplayCharacter;

namespace GlobalEventManager
{
	struct FPlayerKilledMessage
	{
		const FDamageLog& DamageLog;
		AGameplayCharacter* Killer;
		TArray<AGameplayCharacter*> KillAssisters;
		AGameplayCharacter* Victim;
	};

	struct FFlagCaptured
	{
		int32 Team;
		AFlag* CapturedFlag;
	};

	struct FOddBallScored
	{
		int32 Team;
		AOddBall* CapturedFlag;
	};
}
