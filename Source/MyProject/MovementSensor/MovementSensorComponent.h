#pragma once
#include "CoreMinimal.h"
#include "MovementSensorComponent.generated.h"

class AGameplayCharacter;
class UMovementSensorWidget;

USTRUCT(BlueprintType)
struct FDetectedActorInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	AGameplayCharacter* Actor = nullptr;

	UPROPERTY(BlueprintReadOnly)
	FVector Location;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UMovementSensorComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UMovementSensorComponent();

protected:
	virtual void BeginPlay() override;

	// Timer handle for periodic checks
	FTimerHandle ScanTimerHandle;

	void ScanForMovement();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> SensorWidgetClass;

	UMovementSensorWidget* SensorWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sensor")
	float ScanRate = 0.1f; // seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sensor")
	float ScanRadius = 2500.f; // 25 meters = 2500 cm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sensor")
	float MinSpeedDetection = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sensor")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToDetect;
	
	UPROPERTY(BlueprintReadOnly, Category="Sensor")
	TArray<FDetectedActorInfo> DetectedActors;
	UFUNCTION(BlueprintCallable, Category="Sensor")
	const TArray<FDetectedActorInfo>& GetDetectedActors() const { return DetectedActors; }
};
