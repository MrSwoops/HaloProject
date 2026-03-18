#include "ForgePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ForgeCharacter.h"
#include "Blueprint/UserWidget.h"
#include "MyProject/Forge/ForgeBuilderGameMode.h"
#include "MyProject/Forge/UI/ForgeModeHudWidget.h"
#include "MyProject/Player/PlayerCharacter.h"


void AForgePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (PlayerCharacterClass && !PlayerCharacter)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FVector NewSpawnLocation = FVector::ZeroVector;
		auto* NewPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(PlayerCharacterClass, NewSpawnLocation, FRotator::ZeroRotator, SpawnParams);
		if (!NewPlayerCharacter) return;
		Possess(NewPlayerCharacter);
		Possess(ForgeCharacter);
	}
}


#pragma region InputSetup
void AForgePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
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
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Standard Bindings
		EnhancedInputComponent->BindAction(ForgeMovementAction, ETriggerEvent::Triggered, this, &AForgePlayerController::HandleMove);
		EnhancedInputComponent->BindAction(ForgeSprintAction, ETriggerEvent::Started, this, &AForgePlayerController::HandleForgeStartSprint);
		EnhancedInputComponent->BindAction(ForgeSprintAction, ETriggerEvent::Completed, this, &AForgePlayerController::HandleForgeStopSprint);
		EnhancedInputComponent->BindAction(ForgeHoverAction, ETriggerEvent::Triggered, this, &AForgePlayerController::HandleHover);
		EnhancedInputComponent->BindAction(ForgeLookAction, ETriggerEvent::Triggered, this, &AForgePlayerController::HandleLook);

		EnhancedInputComponent->BindAction(ForgeObjectSelectionAction, ETriggerEvent::Triggered, this, &AForgePlayerController::HandleObjectSelection);
		EnhancedInputComponent->BindAction(ForgeOpenObjectsWindowAction, ETriggerEvent::Triggered, this, &AForgePlayerController::HandleOpenObjectsWindow);
		EnhancedInputComponent->BindAction(ForgeOpenObjectEditWindowAction, ETriggerEvent::Triggered, this, &AForgePlayerController::HandleOpenObjectEditWindow);
		EnhancedInputComponent->BindAction(ForgeDeleteObjectAction, ETriggerEvent::Triggered, this, &AForgePlayerController::HandleDeleteObject);
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
#pragma endregion InputSetup
#pragma region Possession
void AForgePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AForgePlayerController::AcknowledgePossession(APawn* P)
{
	Super::Super::AcknowledgePossession(P);
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
	Super::OnPossessPlayer(InPossessedPlayer);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(ForgePlayerControls);
		Subsystem->AddMappingContext(PlayerControls, 0);
	}
	if (ForgeCharacter) PlayerCharacter->SetActorLocation(ForgeCharacter->GetActorLocation());
	bIsForgeMode = false;
}
void AForgePlayerController::OnPossessForgeCharacter(AForgeCharacter* InForgeCharacter)
{
	if (!InForgeCharacter) { UE_LOG(LogTemp, Error, TEXT("Tried to call OnPossessVehicle with invalid vehicle in ACustomPlayerController::OnPossessVehicle")); return; }
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(PlayerControls);
		Subsystem->AddMappingContext(ForgePlayerControls, 0);
	}
	if (!ForgeCharacter) // First time set up
	{
		ForgeCharacter = InForgeCharacter;
		InitializeForgeHud();
	}
	SetForgeHudEnabled(true);
	SetIgnoreMoveInput(false);
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
	if (PlayerCharacter) ForgeCharacter->SetActorLocation(PlayerCharacter->GetActorLocation());
	bIsForgeMode = true;
}
void AForgePlayerController::OnUnPossessForgeCharacter(AForgeCharacter* InPossessedVehicle)
{
	if (!InPossessedVehicle) { UE_LOG(LogTemp, Error, TEXT("Tried to call OnUnPossessVehicle with invalid vehicle in ACustomPlayerController::OnUnPossessVehicle")); return; }
	SetForgeHudEnabled(false);
}
void AForgePlayerController::OnUnPossessPlayer(APlayerCharacter* InPossessedPlayer)
{
	if (!InPossessedPlayer) { UE_LOG(LogTemp, Error, TEXT("Tried to call OnUnPossessPlayer with invalid character in ACustomPlayerController::OnUnPossessPlayer")); return; }
	Super::OnUnPossessPlayer(InPossessedPlayer);
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
void AForgePlayerController::UpdateInputs(const bool& HoldingObject, const bool& HoveringObject)
{
	if (ForgeHudWidget) ForgeHudWidget->UpdateInputDisplay(HoldingObject, HoveringObject);
}

#pragma endregion UI

#pragma region ForgePlayerFunctions
void AForgePlayerController::HandleMove(const FInputActionValue& Value)
{
	if (ForgeCharacter) ForgeCharacter->Move(Value.Get<FVector2D>());
}
void AForgePlayerController::HandleForgeStartSprint(const FInputActionValue& Value)
{
	if (ForgeCharacter) ForgeCharacter->StartSprint();
}
void AForgePlayerController::HandleForgeStopSprint(const FInputActionValue& Value)
{
	if (ForgeCharacter) ForgeCharacter->StopSprint();
}
void AForgePlayerController::HandleHover(const FInputActionValue& Value)
{
	if (ForgeCharacter) ForgeCharacter->Hover(Value.Get<float>());
}
void AForgePlayerController::HandleLook(const FInputActionValue& Value)
{
	if (ForgeCharacter) ForgeCharacter->Look(Value);
}

void AForgePlayerController::HandleObjectSelection(const FInputActionValue& Value)
{
	if (ForgeCharacter) ForgeCharacter->ObjectGrabDrop();
}
void AForgePlayerController::HandleDeleteObject(const FInputActionValue& Value)
{
	if (ForgeCharacter) ForgeCharacter->DeleteObject();
}
void AForgePlayerController::HandleOpenObjectsWindow(const FInputActionValue& Value)
{
	if (!ForgeCharacter || ForgeCharacter->HoldingObject()) return;
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
	if (!ForgeCharacter || !ForgeCharacter->HoldingObject()) return;
	if (ForgeHudWidget)
	{
		SetIgnoreMoveInput(true);
		FInputModeGameAndUI InputMode;
		SetInputMode(InputMode);
		ForgeHudWidget->SetObjectEditWindowEnabled(true);
	}
}
#pragma endregion ForgePlayerFunctions
#pragma region SharedFunctions
void AForgePlayerController::SwitchMode(const FInputActionValue& Value)
{
	if (IsForgeMode())
	{
		if (!PlayerCharacter) return;
		
		Possess(PlayerCharacter);
		bIsForgeMode = false;
		Cast<AForgeBuilderGameMode>(GetWorld()->GetAuthGameMode())->SetForgeObjectsPlayMode(true);
		return;
	}

	if (!ForgeCharacter) return;

	Possess(ForgeCharacter);
	bIsForgeMode = true;
	Cast<AForgeBuilderGameMode>(GetWorld()->GetAuthGameMode())->SetForgeObjectsPlayMode(false);
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
#pragma endregion SharedFunctions