#pragma once
#include "CoreMinimal.h"
#include "HoldableObject.h"
#include "Flag.generated.h"

UCLASS()
class AFlag : public AHoldableObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* FlagPoleMesh;

protected:
	virtual void BeginPlay() override;
};
