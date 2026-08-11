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

UENUM(meta = (Bitflags))
enum class ESkulls : uint32 // Use enum class for strong typing and uint8 for reflection
{
	None UMETA(DisplayName = "None"),
	IWHBYD UMETA(DisplayName = "IWHBYD"), // Rare dialogue is more common
	GruntBirthdayParty UMETA(DisplayName = "Grunt Birthday Party"), // Grunt headshots cause a celebration
	Iron UMETA(DisplayName = "Iron"), // Dying in coop sets you back to your checkpoint. Dying in solo restarts the level
	Mythic UMETA(DisplayName = "Mythic"), // Enemies have more health
	Tilt UMETA(DisplayName = "Tilt"), // Resistances and weaknesses are increased
	Sputnik UMETA(DisplayName = "Sputnik"), // Mass of all objects is decreased
	Fog UMETA(DisplayName = "Fog"), // Motion tracker is disabled
	Cowbell UMETA(DisplayName = "Cowbell"), // Acceleration from explosions is increased
	Blind UMETA(DisplayName = "Blind") // Hud and weapon do not display on screen
};