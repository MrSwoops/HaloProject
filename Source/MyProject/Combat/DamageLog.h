#pragma once

#include "CoreMinimal.h"
#include "DamageLogEntry.h"
#include "DamageLog.generated.h"

class AGameplayCharacter;

USTRUCT(BlueprintType)
struct FDamageLog
{
	GENERATED_BODY()
public:

	void AddEntry(FDamageLogEntry Entry);
	void ClearLog() { DamageLog.Empty(); }

	void GetDamageInformation(AGameplayCharacter*& OutKiller, TArray<AGameplayCharacter*>& OutAssists);

private:
	TArray<FDamageLogEntry> DamageLog;

};