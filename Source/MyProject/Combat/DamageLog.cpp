#include "DamageLog.h"

#include "DamageLogEntry.h"

void FDamageLog::AddEntry(FDamageLogEntry Entry)
{
	DamageLog.Add(Entry);
}

void FDamageLog::GetDamageInformation(AGameplayCharacter*& OutKiller, TArray<AGameplayCharacter*>& OutAssists)
{
	OutKiller = nullptr;
	OutAssists.Reset();

	// Defensive: ensure the log isn’t empty
	if (DamageLog.Num() == 0)
	{
		return;
	}

	// Iterate backwards: newest entries first
	for (int32 i = DamageLog.Num() - 1; i >= 0; --i)
	{
		if (AGameplayCharacter* Attacker = DamageLog[i].Attacker)
		{
			if (OutKiller == nullptr) OutKiller = Attacker;
			else if (Attacker != OutKiller && !OutAssists.Contains(Attacker)) OutAssists.Add(Attacker);
		}
	}
}
