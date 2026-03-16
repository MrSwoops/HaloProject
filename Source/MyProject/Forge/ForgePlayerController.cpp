#include "ForgePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ForgeCharacter.h"
#include "Blueprint/UserWidget.h"
#include "MyProject/Player/PlayerCharacter.h"
#include "UI/ForgeModeHudWidget.h"

void AForgePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UE_LOG(LogTemp, Error, TEXT("SetupInputComponent called"));
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(ForgePlayerControls, 0);
		Subsystem->AddMappingContext(SharedControls, 0);
	}
	else
	{UE_LOG(LogTemp, Error, TEXT("Unable to assign mapping contexts in AForgePlayerController::SetupInputComponent"))}
	SetupForgePlayerInputs();
	SetupSharedInputs();
}

void AForgePlayerController::SetupForgePlayerInputs()
{
	UE_LOG(LogTemp, Error, TEXT("Set Forge Inputs called"));
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Standard Bindings
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AForgePlayerController::HandleMove);
		EnhancedInputComponent->BindAction(HoverAction, ETriggerEvent::Triggered, this, &AForgePlayerController::HandleHover);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AForgePlayerController::HandleLook);

		EnhancedInputComponent->BindAction(ObjectSelectionAction, ETriggerEvent::Triggered, this, &AForgePlayerController::HandleObjectSelection);
		EnhancedInputComponent->BindAction(OpenObjectsWindowAction, ETriggerEvent::Triggered, this, &AForgePlayerController::HandleOpenObjectsWindow);
		EnhancedInputComponent->BindAction(OpenObjectEditWindowAction, ETriggerEvent::Triggered, this, &AForgePlayerController::HandleOpenObjectEditWindow);
		EnhancedInputComponent->BindAction(DeleteObjectAction, ETriggerEvent::Triggered, this, &AForgePlayerController::HandleDeleteObject);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
void AForgePlayerController::SetupSharedInputs()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Standard Bindings
		EnhancedInputComponent->BindAction(SwitchModeAction, ETriggerEvent::Triggered, this, &AForgePlayerController::SwitchMode);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AForgePlayerController::HandlePauseInput);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AForgePlayerController::HandleMove(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Error, TEXT("Forge PC Move"));
	if (ForgeCharacter) ForgeCharacter->Move(Value.Get<FVector2D>());
}
void AForgePlayerController::HandleHover(const FInputActionValue& Value)
{
	if (ForgeCharacter) ForgeCharacter->Hover(Value.Get<float>());
}
void AForgePlayerController::HandleLook(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Error, TEXT("Forge PC Look"));
	if (ForgeCharacter) ForgeCharacter->Look(Value);
}

void AForgePlayerController::HandleObjectSelection(const FInputActionValue& Value)
{

}
void AForgePlayerController::HandleDeleteObject(const FInputActionValue& Value)
{
	
}
void AForgePlayerController::HandleOpenObjectsWindow(const FInputActionValue& Value)
{
	if (ForgeHudWidget)
	{
		SetIgnoreMoveInput(true);
		FInputModeGameAndUI InputMode;
		SetInputMode(InputMode);
		ForgeHudWidget->SetObjectsWindowEnabled(true);
	}
}
void AForgePlayerController::HandleOpenObjectEditWindow(const FInputActionValue& Value)
{
	if (ForgeHudWidget)
	{
		SetIgnoreMoveInput(true);
		FInputModeGameAndUI InputMode;
		SetInputMode(InputMode);
		ForgeHudWidget->SetObjectsWindowEnabled(true);
	}
}


void AForgePlayerController::SwitchMode(const FInputActionValue& Value)
{
	if (IsForgeMode())
	{
		if (!PlayerCharacter) return;
		
		Possess(PlayerCharacter);
		bIsForgeMode = false;
		return;
	}

	if (!ForgeCharacter) return;

	Possess(ForgeCharacter);
	bIsForgeMode = true;
}

void AForgePlayerController::HandlePauseInput(const FInputActionValue& Value)
{
	if (IsForgeMode() && ForgeHudWidget && ForgeHudWidget->IsSubWindowOpen()) // Close Forge Edit Window
	{
		SetIgnoreMoveInput(false);
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		ForgeHudWidget->SetObjectsWindowEnabled(false);
		ForgeHudWidget->SetObjectEditWindowEnabled(false);
		return;
	}

	// pause
	//TogglePause();
}


#pragma region Possession
void AForgePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetIgnoreMoveInput(false);
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	bShowMouseCursor = false;
}

void AForgePlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	if (!IsLocalController()) return;

	if (ForgeCharacter && !Cast<AForgeCharacter>(P))
	{
		OnUnPossessForgeCharacter(ForgeCharacter);
	}
	else if (PlayerCharacter && !Cast<APlayerCharacter>(P))
	{
		OnUnPossessPlayer(PlayerCharacter);
	}
	if (APlayerCharacter* InPlayer = Cast<APlayerCharacter>(P)) OnPossessPlayer(InPlayer);
	else if (AForgeCharacter* InForgeCharacter = Cast<AForgeCharacter>(P)) OnPossessForgeCharacter(InForgeCharacter);
}
void AForgePlayerController::OnPossessPlayer(APlayerCharacter* InPossessedPlayer)
{
	if (!InPossessedPlayer) { UE_LOG(LogTemp, Error, TEXT("Tried to call OnPossessPlayer with invalid character in ACustomPlayerController::OnPossessPlayer")); return; }
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// Subsystem->AddMappingContext(StandardControls, 0);
		Subsystem->RemoveMappingContext(ForgePlayerControls);
	}
	if (!PlayerCharacter) // First time set up
	{
		PlayerCharacter = InPossessedPlayer;
		// InitializePlayerHud();
	}
	// ShowPlayerHud();
	if (ForgeCharacter) PlayerCharacter->SetActorLocation(ForgeCharacter->GetActorLocation());
	bIsForgeMode = false;
}
void AForgePlayerController::OnPossessForgeCharacter(AForgeCharacter* InForgeCharacter)
{
	UE_LOG(LogTemp, Error, TEXT("PossessedForgeCharacter"));
	if (!InForgeCharacter) { UE_LOG(LogTemp, Error, TEXT("Tried to call OnPossessVehicle with invalid vehicle in ACustomPlayerController::OnPossessVehicle")); return; }
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// Subsystem->RemoveMappingContext(StandardControls);
		Subsystem->AddMappingContext(ForgePlayerControls, 0);
	}
	if (!ForgeCharacter) // First time set up
	{
		ForgeCharacter = InForgeCharacter;
		InitializeForgeHud();
	}
	SetForgeHudEnabled(true);
	if (PlayerCharacter) ForgeCharacter->SetActorLocation(PlayerCharacter->GetActorLocation());
	bIsForgeMode = true;
}

void AForgePlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	UE_LOG(LogTemp, Error, TEXT("Unpossess"));
}
void AForgePlayerController::OnUnPossessForgeCharacter(AForgeCharacter* InPossessedVehicle)
{
	UE_LOG(LogTemp, Error, TEXT("UnpossessForgeCharacter"));
	if (!InPossessedVehicle) { UE_LOG(LogTemp, Error, TEXT("Tried to call OnUnPossessVehicle with invalid vehicle in ACustomPlayerController::OnUnPossessVehicle")); return; }
	SetForgeHudEnabled(false);
}
void AForgePlayerController::OnUnPossessPlayer(APlayerCharacter* InPossessedPlayer)
{
	if (!InPossessedPlayer) { UE_LOG(LogTemp, Error, TEXT("Tried to call OnUnPossessPlayer with invalid character in ACustomPlayerController::OnUnPossessPlayer")); return; }
	// HidePlayerHud();
}
#pragma endregion Possession

#pragma region UI

void AForgePlayerController::InitializeForgeHud()
{
	if (ForgeHudWidget || !ForgeHudClass) return;
	
	ForgeHudWidget = CreateWidget<UForgeModeHudWidget>(this, ForgeHudClass);
	ForgeHudWidget->AddToViewport();
}

void AForgePlayerController::SetForgeHudEnabled(const bool& SetEnabled)
{
	if (!ForgeHudWidget) return;
	
	ForgeHudWidget->SetVisibility(SetEnabled ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

#pragma endregion UI