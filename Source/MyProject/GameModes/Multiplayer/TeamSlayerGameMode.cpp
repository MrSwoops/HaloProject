#include "TeamSlayerGameMode.h"

#include "MyProject/GameplayCharacter.h"
#include "MyProject/EventSystem/EventDefinitions.h"

void ATeamSlayerGameMode::HandlePlayerDeath(const GlobalEventManager::FPlayerKilledMessage& Msg)
{
	Super::HandlePlayerDeath(Msg);
	if (Msg.Killer == nullptr || Msg.Victim->Team == Msg.Killer->Team)
	{
		AddPoints(Msg.Killer->Team, 1);//AddPoints(Msg.Victim->Team, -1);
	}
	else
	{
		AddPoints(Msg.Killer->Team, 1);
	}
}
