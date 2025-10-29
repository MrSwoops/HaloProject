// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsPersistenceHandler.h"

// Sets default values
ASettingsPersistenceHandler::ASettingsPersistenceHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASettingsPersistenceHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASettingsPersistenceHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

