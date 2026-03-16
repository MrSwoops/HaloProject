#pragma once

#include "CoreMinimal.h"
#include "ActionInputDisplayMapData.generated.h"

USTRUCT(BlueprintType)
struct FInputPromptData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseIcon = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayText;
};

UCLASS(BlueprintType)
class UActionInputDisplayMapData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FKey, FInputPromptData> KeyMappings;

	const FInputPromptData* GetInputPromptData(const FKey& InKey);
};
