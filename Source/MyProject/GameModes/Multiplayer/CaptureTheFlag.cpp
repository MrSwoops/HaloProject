#include "CaptureTheFlag.h"

#include "MyProject/EventSystem/EventDefinitions.h"
#include "MyProject/EventSystem/GlobalEventManager.h"


void ACaptureTheFlag::BeginPlay()
{
	Super::BeginPlay();

	using namespace GlobalEventManager;
	FGlobalEventManager::Subscribe<ACaptureTheFlag, FFlagCaptured>(
		this,
		[this](const FFlagCaptured& Msg)
		{
			OnFlagCaptured(Msg);
		}
		);
	
}

void ACaptureTheFlag::OnFlagCaptured(const GlobalEventManager::FFlagCaptured& Msg)
{
	AddPoints(Msg.Team, 1);
}
