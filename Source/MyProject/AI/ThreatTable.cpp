#include "ThreatTable.h"

void FThreatTable::AddThreat(AActor* Entry, float Threat)
{
	if (!Entry) return;

	if (float* ExistingThreat = ThreatTable.Find(Entry)) *ExistingThreat += Threat;
	else ThreatTable.Add(Entry, Threat);
}

void FThreatTable::RemoveThreat(AActor* Entry, float Threat)
{
	if (!Entry) return;

	if (float* ExistingThreat = ThreatTable.Find(Entry)) *ExistingThreat -= Threat;
}

void FThreatTable::RemoveEntry(AActor* Entry)
{
	if (!Entry) return;

	ThreatTable.Remove(Entry);
}

AActor* FThreatTable::GetHighestThreatTarget() const
{
	AActor* HighestActor = nullptr;
	float HighestThreat = -FLT_MAX;

	for (const auto& Pair : ThreatTable)
	{
		if (Pair.Value > HighestThreat)
		{
			HighestThreat = Pair.Value;
			HighestActor = Pair.Key;
		}
	}

	return HighestActor;
}

void FThreatTable::MarkForRemoval(AActor* Entry, float DelayTime)
{
	if (!Entry) return;

	if (!RemovalTimers.Find(Entry)) RemovalTimers.Add(Entry, DelayTime);
}

void FThreatTable::CancelRemoval(AActor* Entry)
{
	if (!Entry) return;

	RemovalTimers.Remove(Entry);
}

void FThreatTable::UpdateRemovalTimers(float DeltaTime)
{
	TArray<AActor*> ActorsToRemove;
	for (auto& Pair : RemovalTimers)
	{
		Pair.Value -= DeltaTime;
		if (Pair.Value <= 0.0f)
		{
			ActorsToRemove.Add(Pair.Key);
		}
	}

	for (AActor* Actor : ActorsToRemove)
	{
		RemoveEntry(Actor);
	}
}