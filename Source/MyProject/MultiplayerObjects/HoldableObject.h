#pragma once
#include "CoreMinimal.h"
#include "HoldableObject.generated.h"

UCLASS()
class AHoldableObject : public AActor
{
	GENERATED_BODY()
public:
	int32 Team;
};
