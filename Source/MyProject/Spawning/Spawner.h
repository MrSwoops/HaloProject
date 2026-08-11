// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class AGameplayCharacter;
class AWeapon;

UCLASS()
class MYPROJECT_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	USkeletalMeshComponent* SpawnerPreviewMesh;

	virtual AActor* SpawnItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TSubclassOf<AActor> SpawnerClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float SpawnDelay;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void OnConstruction(const FTransform& Transform) override;

	FTimerHandle SpawnTimerHandle;
	void InternalSpawnItem(){ SpawnItem(); }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

UCLASS()
class MYPROJECT_API AWeaponSpawner : public ASpawner
{
	GENERATED_BODY()
public:
	virtual AActor* SpawnItem() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	bool SpawnStatic;

	AWeapon* SpawnedWeapon;
	UFUNCTION()
	void OnWeaponPickUp(AGameplayCharacter* Character);
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
};

UCLASS()
class MYPROJECT_API AVehicleSpawner : public ASpawner
{
	GENERATED_BODY()
public:
	
};