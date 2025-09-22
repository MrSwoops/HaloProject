// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPool.h"
#include "../PooledActor.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	initialSize = 5;
	maxSize = 50;
	addAmount = 5;

	// ...
}

void UActorPool::InitializePool()
{
	
	AddToPool(initialSize);
}

void UActorPool::AddToPool(int count)
{
	FVector const location = FVector(1000.0f, 1000.0f, 1000.0f);
	FRotator const rotation = FRotator(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UWorld* const world = GetWorld();
	for (int i = 0; i < count; i++)
	{
		APooledActor* actor = world->SpawnActor<APooledActor>(actorClass, location, rotation, ActorSpawnParams);

		if (actor == nullptr) {
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("actornull"));
			continue;
		}

		pool.Add(actor);
		actor->ReturnToPool();
	}
}

AActor* UActorPool::SpawnFromTransform(FTransform transform, bool AutoActivate)
{
	APooledActor* actor = RetrieveFirst();
	if (AutoActivate) actor->SetActive(true);
	actor->SetActorTransform(transform);
	return actor;
}

APooledActor* UActorPool::RetrieveFirst() 
{
	for (APooledActor* actor : pool)
	{
		if (actor->pooled)
		{
			actor->pooled = false;
			return actor;
		}
	}
	AddToPool(addAmount);
	return RetrieveFirst();
}

AActor* UActorPool::SpawnFromLocationAndRotation(FVector location, FRotator rotation, bool AutoActivate)
{
	APooledActor* actor = RetrieveFirst();
	actor->SetActorLocationAndRotation(location, rotation);
	if (AutoActivate) actor->SetActive(true);
	return actor;
}

// Called when the game starts
void UActorPool::BeginPlay()
{
	Super::BeginPlay();
	InitializePool();
	// ...

}

