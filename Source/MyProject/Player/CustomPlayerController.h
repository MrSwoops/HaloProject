#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerController.generated.h"

class APlayerCharacter;
class AForgeCharacter;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

#pragma region Possession
protected:
	// virtual void OnPossess(APawn* InPawn) override;
	// virtual void OnUnPossess() override;
	// virtual void AcknowledgePossession(APawn* P) override;
	//
	// void OnPossessPlayer(APlayerCharacter* InPossessedPlayer);
	// void OnUnPossessPlayer(APlayerCharacter* InPossessedPlayer);

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;
#pragma endregion Possession

#pragma region Inputs
protected:
	//virtual void SetupInputComponent() override;
	
#pragma region ForgePlayerActions
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|StandardMovement")
	UInputMappingContext* PlayerControls;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|StandardMovement")
	UInputAction* PlayerMovementAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|StandardMovement")
	UInputAction* PlayerLookAction;
#pragma endregion PlayerActions
#pragma region PlayerFunctions
protected:
	// void SetupForgePlayerInputs();
	// void HandlePlayerLook(const FInputActionValue& Value);
	// void HandlePlayerMove(const FInputActionValue& Value);
	// void HandlePlayerHover(const FInputActionValue& Value);
	// void HandlePlayerInteract(const FInputActionValue& Value);
#pragma endregion PlayerFunctions

#pragma endregion Inputs
	
};
