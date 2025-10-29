// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProject/PersistentData/SettingsPersistentData.h"
#include "SettingsPersistenceHandler.generated.h"

UCLASS()
class MYPROJECT_API ASettingsPersistenceHandler : public AActor, public ISettingsPersistentData
{
	GENERATED_BODY()

public:
	virtual void SaveData_Implementation(UPlayerSettings* SaveFile) override {};
	virtual void LoadData_Implementation(UPlayerSettings* SaveFile) override {};
	// Sets default values for this actor's properties
	ASettingsPersistenceHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
