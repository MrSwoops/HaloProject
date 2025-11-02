#pragma once

#include "CoreMinimal.h"
#include "CampaignDefinitions.generated.h"

UENUM(BlueprintType)
enum class EDifficulty : uint8 // Use enum class for strong typing and uint8 for reflection
{
	Easy UMETA(DisplayName = "Easy"), 
	Normal UMETA(DisplayName = "Normal"),
	Heroic UMETA(DisplayName = "Heroic"),
	Legendary UMETA(DisplayName = "Legendary")
};
