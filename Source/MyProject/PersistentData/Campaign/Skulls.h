#pragma once

#include "Skulls.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FSkulls
{
	GENERATED_BODY()
public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bUnlockedIWHBYD = false; // Rare dialogue is more common
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bIWHBYDActive = false;
	
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bUnlockedGruntBirthdayParty = false; // Grunt headshots cause a celebration
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bGruntBirthdayPartyActive = false;
	
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bUnlockedIron = false; // Dying in coop sets you back to your checkpoint. Dying in solo restarts the level
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bIronActive = false;
	
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bUnlockedMythic = false; // Enemies have more health
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bMythicActive = false;
	
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bUnlockedTilt = false; // Resistances and weaknesses are increased
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bTiltActive = false;
	
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bUnlockedSputnik = false; // Mass of all objects is decreased
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bSputnikActive = false;
	
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bUnlockedFog = false; // Motion tracker is disabled
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bFogActive = false;
	
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bUnlockedCowbell = false; // Acceleration from explosions is increased
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bCowbellActive = false;
	
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bUnlockedBlind = false; // Hud and weapon do not display on screen
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bBlindActive = false;
	
};
