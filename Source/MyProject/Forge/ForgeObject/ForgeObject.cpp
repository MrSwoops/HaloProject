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
	if (!InCharacter) { UE_LOG(LogTemp, Error, TEXT("Invalid InteractingCharacter in AForgeObject::OnInteract")); return; }
	if (InCharacter->HoldingObject()) { UE_LOG(LogTemp, Error, TEXT("Already holding an object in AForgeObject::OnInteract")); return; }
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
		if (!GameplayObject->GetChildActor())
		{
			GameplayObject->CreateChildActor();
		}
		AActor* Child = GameplayObject->GetChildActor();
		if (Child)
		{
			Child->SetActorTransform(ForgePreviewMesh->GetComponentTransform());

			Child->SetActorHiddenInGame(false);
			Child->SetActorEnableCollision(true);
			Child->SetActorTickEnabled(true);
		}

		ForgePreviewMesh->SetVisibility(false);
		ForgePreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ForgePreviewMesh->SetComponentTickEnabled(false);
		return;
	}
	
	AActor* Child = GameplayObject->GetChildActor();
	if (Child)
	{
		// Disable gameplay actor
		Child->SetActorHiddenInGame(true);
		Child->SetActorEnableCollision(false);
		Child->SetActorTickEnabled(false);
	}

	// Enable preview mesh
	ForgePreviewMesh->SetVisibility(true);
	ForgePreviewMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ForgePreviewMesh->SetComponentTickEnabled(true);
}
