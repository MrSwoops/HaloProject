// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSettings.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UPlayerSettings : public USaveGame
{
	GENERATED_BODY()

public:

#pragma region Audio

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	int32 MasterVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	int32 MusicVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	int32 SFXVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	int32 VoiceVolume;

#pragma endregion Audio

#pragma region Graphics

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	bool Fullscreen;

#pragma endregion Graphics
	
};
