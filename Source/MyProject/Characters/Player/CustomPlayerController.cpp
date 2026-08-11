#include "CustomPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.h"
#include "UI/PlayerHudWidget.h"

#pragma region InputSetup
void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerControls, 0);
	}
	else
	{UE_LOG(LogTemp, Error, TEXT("Unable to assign mapping contexts in AForgePlayerController::SetupInputComponent"))}
	SetupPlayerInputs();
}
void ACustomPlayerController::SetupPlayerInputs()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::HandlePlayerMove);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ACustomPlayerController::HandlePlayerCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACustomPlayerController::HandlePlayerUnCrouch);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACustomPlayerController::HandlePlayerJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACustomPlayerController::HandlePlayerStopJumping);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::HandlePlayerLook);
		EnhancedInputComponent->BindAction(SwapCamAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::HandlePlayerSwapCam);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::HandlePlayerInteract);

		EnhancedInputComponent->BindAction(SwapGrenadesAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::HandlePlayerSwapNades);
		EnhancedInputComponent->BindAction(ThrowGrenadeAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::HandlePlayerThrowNade);

		EnhancedInputComponent->BindAction(SwapWeaponsAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::HandlePlayerSwapWeapons);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ACustomPlayerController::HandlePlayerFirePressed);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ACustomPlayerController::HandlePlayerFireReleased);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::HandlePlayerReload);
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::HandlePlayerMelee);
	}
	else UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
}
#pragma endregion InputSetup

#pragma region Possession
void ACustomPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ACustomPlayerController::AcknowledgePossession(APawn* P)
{
	if (!IsLocalController()) return;

	if (PlayerCharacter && !Cast<APlayerCharacter>(P))
	{
		OnUnPossessPlayer(PlayerCharacter);
	}
	if (APlayerCharacter* InPlayer = Cast<APlayerCharacter>(P)) OnPossessPlayer(InPlayer);
}

void ACustomPlayerController::OnPossessPlayer(APlayerCharacter* InPossessedPlayer)
{
	if (!InPossessedPlayer) { UE_LOG(LogTemp, Error, TEXT("Tried to call OnPossessPlayer with invalid character in ACustomPlayerController::OnPossessPlayer")); return; }
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerControls, 0);
	}
	if (!PlayerCharacter) // First time set up
	{
		PlayerCharacter = InPossessedPlayer;
		InitializePlayerHud(InPossessedPlayer);
	}
	SetPlayerHudEnabled(true);

	SetIgnoreMoveInput(false);
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
}

void ACustomPlayerController::OnUnPossessPlayer(APlayerCharacter* InPossessedPlayer)
{
	if (!InPossessedPlayer) { UE_LOG(LogTemp, Error, TEXT("Tried to call OnUnPossessPlayer with invalid character in ACustomPlayerController::OnUnPossessPlayer")); return; }
	SetPlayerHudEnabled(false);
}
#pragma endregion Possession

#pragma region UI
void ACustomPlayerController::InitializePlayerHud(APlayerCharacter* InPlayerCharacter)
{
	if (PlayerHudWidget || !PlayerHudClass || !InPlayerCharacter) return;
	
	PlayerHudWidget = CreateWidget<UPlayerHudWidget>(this, PlayerHudClass);
	PlayerHudWidget->InitializeConnections(InPlayerCharacter);
	PlayerHudWidget->AddToViewport();
}
void ACustomPlayerController::SetPlayerHudEnabled(const bool& SetEnabled)
{
	if (!PlayerHudWidget) return;
	PlayerHudWidget->SetVisibility(SetEnabled ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

#pragma endregion UI

#pragma region PlayerFunctions
void ACustomPlayerController::HandlePlayerMove(const FInputActionValue& Value)
{
	if (PlayerCharacter) PlayerCharacter->Move(Value);// if (PlayerCharacter) PlayerCharacter->;
}
void ACustomPlayerController::HandlePlayerJump(const FInputActionValue& Value)
{
	if (PlayerCharacter) PlayerCharacter->Jump();
}
void ACustomPlayerController::HandlePlayerStopJumping(const FInputActionValue& Value)
{
	if (PlayerCharacter) PlayerCharacter->StopJumping();
}
void ACustomPlayerController::HandlePlayerCrouch(const FInputActionValue& Value)
{
	if (PlayerCharacter) PlayerCharacter->Crouch();
}
void ACustomPlayerController::HandlePlayerUnCrouch(const FInputActionValue& Value)
{
	if (PlayerCharacter) PlayerCharacter->UnCrouch();
}
void ACustomPlayerController::HandlePlayerLook(const FInputActionValue& Value)
{
	if (PlayerCharacter) PlayerCharacter->Look(Value);
}
void ACustomPlayerController::HandlePlayerSwapCam(const FInputActionValue& Value)
{
	if (PlayerCharacter) PlayerCharacter->SwapCam();
}
void ACustomPlayerController::HandlePlayerInteract(const FInputActionValue& Value)
{
	if (PlayerCharacter) PlayerCharacter->TryInteract();
}
void ACustomPlayerController::HandlePlayerSwapNades(const FInputActionValue& Value)
{
	if (PlayerCharacter) PlayerCharacter->SwapGrenades();
}
void ACustomPlayerController::HandlePlayerThrowNade(const FInputActionValue& Value)
{
	if (PlayerCharacter) PlayerCharacter->TriggerGrenadeThrow();
}
void ACustomPlayerController::HandlePlayerSwapWeapons(const FInputActionValue& Value)
{
	if (PlayerCharacter) PlayerCharacter->SwapWeapons();
}
void ACustomPlayerController::HandlePlayerFirePressed(const FInputActionValue& Value)
{
	if (PlayerCharacter) PlayerCharacter->FirePressed();
}
void ACustomPlayerController::HandlePlayerFireReleased(const FInputActionValue& Value)
{
	if (PlayerCharacter) PlayerCharacter->FireReleased();
}
void ACustomPlayerController::HandlePlayerReload(const FInputActionValue& Value)
{
	if (PlayerCharacter) PlayerCharacter->TriggerReloadAction();
}
void ACustomPlayerController::HandlePlayerMelee(const FInputActionValue& Value)
{
	if (PlayerCharacter) PlayerCharacter->Melee();
}
void ACustomPlayerController::HandlePlayerUseAbility(const FInputActionValue& Value)
{
	//if (PlayerCharacter) PlayerCharacter->;
}
#pragma endregion PlayerFunctions