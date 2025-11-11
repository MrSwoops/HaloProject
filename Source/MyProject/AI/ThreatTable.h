#pragma once

#include "CoreMinimal.h"
#include "ThreatTable.generated.h"

USTRUCT(BlueprintType)
struct FThreatTable
{
public:
	GENERATED_BODY()
// 	UFUNCTION()
// 	void AddThreat(AActor* Target, float Threat);
// 	void RemoveThreat();
// 	
//
// private:
// 	TMap<AActor*, float> ThreatTable;

	void AddThreat(AActor* Entry, float Threat);
	void RemoveThreat(AActor* Entry, float Threat);
	
	void RemoveEntry(AActor* Entry);
	void MarkForRemoval(AActor* Entry, float DelayTime);
	void CancelRemoval(AActor* Entry);
	void UpdateRemovalTimers(float DeltaTime);

	AActor* GetHighestThreatTarget() const;

	bool HasThreats() const { return ThreatTable.Num() > 0; }

private:
	TMap<AActor*, float> ThreatTable;
	TMap<AActor*, float> RemovalTimers;
};
