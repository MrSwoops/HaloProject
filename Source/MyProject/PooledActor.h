// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledActor.generated.h"

UCLASS()
class MYPROJECT_API APooledActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APooledActor();

	bool pooled = false;

	virtual void SetActive(bool i);

	void ReturnToPool()
	{
		pooled = true;
		SetActive(false);
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
