#pragma once
#include "CoreMinimal.h"
#include "MyProject/GameModes/BaseGameMode.h"
#include "ForgeBuilderGameMode.generated.h"

class AForgeObject;

UCLASS()
class AForgeBuilderGameMode : public ABaseGameMode
{
	GENERATED_BODY()
public:
	void BeginPlay() override;
	void SetForgeObjectsPlayMode(bool IsPlayMode); // Set objects to play or forge mode.
	void AddForgeObject(AForgeObject* ForgeObject);
	void RemoveForgeObject(AForgeObject* ForgeObject);
private:
	UPROPERTY()
	TArray<AForgeObject*> ForgeObjects;
};
