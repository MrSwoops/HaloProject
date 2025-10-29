#include "OddBallGameMode.h"

#include "MyProject/EventSystem/EventDefinitions.h"
#include "MyProject/EventSystem/GlobalEventManager.h"

void AOddBallGameMode::BeginPlay()
{
	Super::BeginPlay();

	using namespace GlobalEventManager;
	FGlobalEventManager::Subscribe<AOddBallGameMode, FOddBallScored>(
		this,
		[this](const FOddBallScored& Msg)
		{
			OddBallScored(Msg);
		}
		);
}

void AOddBallGameMode::OddBallScored(const GlobalEventManager::FOddBallScored& Msg)
{
	
}

