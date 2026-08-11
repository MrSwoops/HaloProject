// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolingBlueprintStatics.h"

#include "BulletPoolManager.h"
#include "MyProject/GameModes/BaseGameMode.h"

AGrenade* UObjectPoolingBlueprintStatics::RetrieveGrenade(UObject* ReferenceObject, const FVector& Location, const FRotator& Rotation, const int32 NadeType)
{
	if (!ReferenceObject) return nullptr;
	AGrenade* OutGrenade = nullptr;

	if (auto* GameMode = Cast<ABaseGameMode>(ReferenceObject->GetWorld()->GetAuthGameMode()))
	{
		OutGrenade = GameMode->BulletPoolManager->SpawnGrenade(Location, Rotation, NadeType);
	}
	
	return OutGrenade;
}
