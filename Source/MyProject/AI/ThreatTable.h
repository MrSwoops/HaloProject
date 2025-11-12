#pragma once

#include "CoreMinimal.h"
#include "ThreatTable.generated.h"

USTRUCT(BlueprintType)
struct FThreatTable
{
public:
	GENERATED_BODY()
	void AddThreat(AActor* Entry, float Threat);
	void AddToTop(AActor* Entry);
	void RemoveThreat(AActor* Entry, float Threat);
	
	void RemoveEntry(AActor* Entry);
	void MarkForRemoval(AActor* Entry, float DelayTime);
	void CancelRemoval(AActor* Entry);
	void UpdateRemovalTimers(float DeltaTime);

	AActor* GetHighestThreatTarget() const;

	bool HasThreats() const { return ThreatTable.Num() > 0; }

private:
	UPROPERTY()
	TMap<AActor*, float> ThreatTable;
	UPROPERTY()
	TMap<AActor*, float> RemovalTimers;
};
