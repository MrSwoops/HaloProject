#pragma once

#include "CoreMinimal.h"
#include "ForgeObjectSettings.generated.h"

USTRUCT(Blueprintable)
struct FSpawnerObjectSettings
{
	GENERATED_BODY()

	UPROPERTY()
	int32 SpawnTimer;
};

USTRUCT(Blueprintable)
struct FWeaponObjectSettings
{
	GENERATED_BODY()

	UPROPERTY()
	int32 SpareClips;
};

USTRUCT(Blueprintable)
struct FObjectSettings
{
	GENERATED_BODY()

	UPROPERTY()
	bool bUseSpawnerSettings;
	UPROPERTY(meta = (EditCondition="bUseSpawnerSettings", EditConditionHides))
	FSpawnerObjectSettings SpawnerSettings;
	UPROPERTY()
	bool bUseWeaponSettings;
	UPROPERTY(meta = (EditCondition="bUseSpawnerSettings", EditConditionHides))
	FWeaponObjectSettings WeaponSettings;
};

