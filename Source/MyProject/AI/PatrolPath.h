#pragma once

#include "PatrolPath.generated.h"

class USplineComponent;

UCLASS(BlueprintType, Blueprintable)
class APatrolPath : public AActor
{
	GENERATED_BODY()
public:
	APatrolPath();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USplineComponent* SplinePath;

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetPathPoints();
};