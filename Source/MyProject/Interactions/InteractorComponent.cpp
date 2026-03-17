#include "InteractorComponent.h"

#include "InteractableInterface.h"
#include "InteractionSettings.h"
#include "MyProject/Forge/Character/ForgeCharacter.h"
#include "UI/InteractionUI.h"

UInteractorComponent::UInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!CharacterOwner) CharacterOwner = Cast<AForgeCharacter>(GetOwner());
}

void UInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PerformInteractableCheck();
}

void UInteractorComponent::PerformInteractableCheck()
{
	if (!CharacterOwner) return;
	auto* CheckComp = CharacterOwner->GetCharacterMesh();
	if (!CheckComp) return;
	
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	const FVector StartLocation = CheckComp->GetComponentLocation();
	const FVector ForwardVector = CheckComp->GetForwardVector();
	FVector EndLocation = StartLocation + (ForwardVector * InteractionCheckFirstPersonDistance);//(CharacterOwner->FirstPersonCameraComponent->IsActive() ? InteractionCheckFirstPersonDistance : InteractionCheckThirdPersonDistance));

	//GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);
	const bool Hit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_GameTraceChannel1, CollisionParams);

	if (PerformDebugging)
	{
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, Hit ? FColor::Green : FColor::Red, false, 0.3f, 0, 1.0f);
		//if (Hit) { UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName()); }
		//else { UE_LOG(LogTemp, Warning, TEXT("No hit")); }
	}

	if (Hit)
	{
		if (HitResult.GetComponent() && HitResult.GetComponent()->Implements<UInteractableInterface>())
		{
			if (CurrentInteractionObject != HitResult.GetComponent())
			{
				OnNewInteractable(HitResult.GetComponent());
			}
			return;
		}
		if (HitResult.GetActor() && HitResult.GetActor()->Implements<UInteractableInterface>())
		{
			if (CurrentInteractionObject != HitResult.GetActor())
			{
				OnNewInteractable(HitResult.GetActor());
			}
			return;
		}
	}
	if (CurrentInteractionObject) OnLostInteractable();
}

void UInteractorComponent::OnNewInteractable(UObject* NewInteractable)
{
	if (!IInteractableInterface::Execute_IsInteractable(NewInteractable)) return;
	
	CurrentInteractionObject = NewInteractable;
	FInteractionSettings Settings;
	IInteractableInterface::Execute_GetInteractionSettings(NewInteractable, Settings);
	if (InteractionUI)
	{
		InteractionUI->SetVisibility(ESlateVisibility::Visible);
		InteractionUI->UpdateInteractionText(Settings.InteractionText);
	}
}

void UInteractorComponent::OnLostInteractable()
{
	CurrentInteractionObject = nullptr;
	if (InteractionUI)
	{
		InteractionUI->SetVisibility(ESlateVisibility::Hidden);
		InteractionUI->ClearInteractionText();
	}
}

void UInteractorComponent::OnInteract()
{
	if (!CurrentInteractionObject) return;
	if (!CharacterOwner) { UE_LOG(LogTemp, Error, TEXT("No CharacterOwner assigned to UCharacterInteractionComponent in UCharacterInteractionComponent::OnInteract")); return; }

	if (!CharacterOwner->HasAuthority()) // Client. RPC
	{
		Server_RequestInteraction(CurrentInteractionObject);
		return;
	}
	
	// Server. Execute.
	IInteractableInterface::Execute_OnInteract(CurrentInteractionObject, CharacterOwner);

	FInteractionSettings Settings;
	IInteractableInterface::Execute_GetInteractionSettings(CurrentInteractionObject, Settings);
	if (Settings.RemoveInteractionOnInteract)
	{
		OnLostInteractable();
	}
}
void UInteractorComponent::Server_RequestInteraction_Implementation(UObject* InteractionObject)
{
	//OnInteract();
	if (!InteractionObject) return;
	if (!CharacterOwner) { UE_LOG(LogTemp, Error, TEXT("No CharacterOwner assigned to UCharacterInteractionComponent in UCharacterInteractionComponent::Server_RequestInteraction_Implementation")); return; }

	if (!InteractionObject->Implements<UInteractableInterface>()) return;

	IInteractableInterface::Execute_OnInteract(InteractionObject, CharacterOwner);
	
	FInteractionSettings Settings;
	IInteractableInterface::Execute_GetInteractionSettings(InteractionObject, Settings);
	if (Settings.RemoveInteractionOnInteract)
	{
		OnLostInteractable();
	}
}

void UInteractorComponent::InitializeUI(APlayerController* InController)
{
	if (!InController) { UE_LOG(LogTemp, Warning, TEXT("Invalid controller given in UCharacterInteractionComponent::InitializeHUD")); return; }
	
	if (!InteractionUI) InteractionUI = Cast<UInteractionUI>(CreateWidget(InController, InteractionUIClass));
	if (!InteractionUI){ UE_LOG(LogTemp, Warning, TEXT("Unable to create vehicle hud in UCharacterInteractionComponent::InitializeHUD")); return; }
	InteractionUI->AddToViewport();
	InteractionUI->SetVisibility(ESlateVisibility::Hidden);
}
