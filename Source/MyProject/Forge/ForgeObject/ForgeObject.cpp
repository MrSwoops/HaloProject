#include "ForgeObject.h"

#include "MyProject/Forge/Character/ForgeCharacter.h"

AForgeObject::AForgeObject()
{
	ForgePreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>("ForgePreviewMesh");
	SetRootComponent(ForgePreviewMesh);

	GameplayObject = CreateDefaultSubobject<UChildActorComponent>("GameplayObject");
	GameplayObject->SetupAttachment(ForgePreviewMesh);
}

void AForgeObject::BeginPlay()
{
	Super::BeginPlay();
	SetPlayMode(false);
}

void AForgeObject::OnInteract_Implementation(AForgeCharacter* InCharacter)
{
	if (!InCharacter) { UE_LOG(LogTemp, Error, TEXT("Invalid InteractingCharacter in AItem::OnInteract")); return; }
	if (InCharacter->HoldingObject()) { UE_LOG(LogTemp, Error, TEXT("Invalid Inventory in AItem::OnInteract")); return; }
	InCharacter->PickupObject(this);
}

void AForgeObject::OnPickUp()
{
	SetIsInteractable_Implementation(false);
}

void AForgeObject::OnDrop()
{
	SetIsInteractable_Implementation(true);
}

void AForgeObject::SetPlayMode(const bool& IsPlayMode)
{
	if (IsPlayMode)
	{
		ForgePreviewMesh->SetVisibility(false);
		GameplayObject->SetVisibility(true);
		return;
	}
	
	GameplayObject->SetVisibility(false);
	ForgePreviewMesh->SetVisibility(true);
}
