#pragma once

#include "CoreMinimal.h"
#include "InteractionSettings.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FInteractionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InteractionSettings")
	bool RemoveInteractionOnInteract = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InteractionSettings")
	FString InteractionText = "Pick Up";
};
