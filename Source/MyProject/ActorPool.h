// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPool.generated.h"

class APooledActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UActorPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorPool();

	UPROPERTY(EditAnywhere)
	int32 initialSize;

	UPROPERTY(EditAnywhere)
	int32 maxSize;

	UPROPERTY(EditAnywhere)
	int32 addAmount;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APooledActor> actorClass;

	TArray<APooledActor*> pool;

	//UFUNCTION(BlueprintCallable)
	AActor* SpawnFromTransform(FTransform transform);

	//UFUNCTION(BlueprintCallable)
	AActor* SpawnFromLocationAndRotation(FVector location, FRotator rotation);

private:
	void InitializePool();
	void AddToPool(int count);
	APooledActor* RetrieveFirst();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
