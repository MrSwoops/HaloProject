// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameMode.h"

#include "ObjectPooling/BulletPoolManager.h"

ACustomGameMode::ACustomGameMode() 
{
	BulletPoolManager = CreateDefaultSubobject<UBulletPoolManager>(TEXT("Bullet Poob"));
}

void ACustomGameMode::BeginPlay()
{
	
}

