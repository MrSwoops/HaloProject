#pragma once

#include "CoreMinimal.h"
#include "ForgePlayerController.generated.h"

class UForgeModeHudWidget;
class APlayerCharacter;
class AForgeCharacter;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class AForgePlayerController : public APlayerController
{
	GENERATED_BODY()
public:

#pragma region Possession
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void AcknowledgePossession(APawn* P) override;
	
	void OnPossessPlayer(APlayerCharacter* InPossessedPlayer);
	void OnPossessForgeCharacter(AForgeCharacter* PossessedVehicle);
	void OnUnPossessPlayer(APlayerCharacter* InPossessedPlayer);
	void OnUnPossessForgeCharacter(AForgeCharacter* PossessedVehicle);

public:
	const bool& IsForgeMode() const { return bIsForgeMode; }
private:
	bool bIsForgeMode;
	UPROPERTY()
	AForgeCharacter* ForgeCharacter;
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;
#pragma endregion Possession

#pragma region Inputs
protected:
	virtual void SetupInputComponent() override;
	
#pragma region ForgePlayerActions
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|StandardMovement")
	UInputMappingContext* ForgePlayerControls;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|StandardMovement")
	UInputAction* MovementAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|StandardMovement")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|StandardMovement")
	UInputAction* HoverAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|StandardMovement")
	UInputAction* ObjectSelectionAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|StandardMovement")
	UInputAction* DeleteObjectAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|StandardMovement")
	UInputAction* OpenObjectsWindowAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|StandardMovement")
	UInputAction* OpenObjectEditWindowAction;
#pragma endregion ForgePlayerActions
#pragma region ForgePlayerFunctions
protected:
	void SetupForgePlayerInputs();
	void HandleLook(const FInputActionValue& Value);
	void HandleMove(const FInputActionValue& Value);
	void HandleHover(const FInputActionValue& Value);
	void HandleObjectSelection(const FInputActionValue& Value);
	void HandleDeleteObject(const FInputActionValue& Value);
	void HandleOpenObjectsWindow(const FInputActionValue& Value);
	void HandleOpenObjectEditWindow(const FInputActionValue& Value);
	
#pragma endregion ForgePlayerFunctions

#pragma region SharedActions
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|StandardMovement")
	UInputMappingContext* SharedControls;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|StandardMovement")
	UInputAction* SwitchModeAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|StandardMovement")
	UInputAction* PauseAction;
#pragma endregion SharedActions
#pragma region SharedFunctions
protected:
	void SetupSharedInputs();
	void SwitchMode(const FInputActionValue& Value);
	void HandlePauseInput(const FInputActionValue& Value);
	//void HandleEscapeInput(const FInputActionValue& Value);
#pragma endregion SharedFunctions

#pragma endregion Inputs

#pragma region UI
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UForgeModeHudWidget> ForgeHudClass;
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UForgeModeHudWidget* ForgeHudWidget;
	void InitializeForgeHud();
	void SetForgeHudEnabled(const bool& SetEnabled);
#pragma endregion UI
	
};
