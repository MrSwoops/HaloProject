#include "ForgeBuilderGameMode.h"

#include "EngineUtils.h"
#include "ForgeObject/ForgeObject.h"

void AForgeBuilderGameMode::BeginPlay()
{
	Super::BeginPlay();
	for (TActorIterator<AForgeObject> It(GetWorld()); It; ++It)
	{
		AForgeObject* FoundActor = *It;
		UE_LOG(LogTemp, Warning, TEXT("Found actor: %s"), *FoundActor->GetName());
		
		ForgeObjects.AddUnique(FoundActor);
	}
}

void AForgeBuilderGameMode::SetForgeObjectsPlayMode(bool IsPlayMode)
{
	for (AForgeObject* ForgeObject : ForgeObjects)
	{
		ForgeObject->SetPlayMode(IsPlayMode);
	}
}

void AForgeBuilderGameMode::AddForgeObject(AForgeObject* ForgeObject)
{
	if (!ForgeObject) return;
	ForgeObjects.AddUnique(ForgeObject);
}

void AForgeBuilderGameMode::RemoveForgeObject(AForgeObject* ForgeObject)
{
	if (!ForgeObject) return;
	ForgeObjects.Remove(ForgeObject);
}
