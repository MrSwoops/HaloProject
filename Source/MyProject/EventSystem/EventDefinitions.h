﻿#pragma once
#include "CoreMinimal.h"

class AOddBall;
class AFlag;
class AGameplayCharacter;

namespace GlobalEventManager
{
	struct FPlayerKilledMessage
	{
		AGameplayCharacter* Killer;
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
