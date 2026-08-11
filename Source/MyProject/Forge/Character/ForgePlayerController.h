#pragma once

#include "CoreMinimal.h"
#include "MyProject/Characters/Player/CustomPlayerController.h"
#include "ForgePlayerController.generated.h"

class UForgeModeHudWidget;
class APlayerCharacter;
class AForgeCharacter;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class AForgePlayerController : public ACustomPlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Forge")
	TSubclassOf<APlayerCharacter> PlayerCharacterClass;

protected:
	virtual void BeginPlay() override;
	
#pragma region Possession
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void AcknowledgePossession(APawn* P) override;
	
	virtual void OnPossessPlayer(APlayerCharacter* InPossessedPlayer) override;
	virtual void OnUnPossessPlayer(APlayerCharacter* InPossessedPlayer) override;
	void OnPossessForgeCharacter(AForgeCharacter* PossessedVehicle);
	void OnUnPossessForgeCharacter(AForgeCharacter* PossessedVehicle);

public:
	const bool& IsForgeMode() const { return bIsForgeMode; }
private:
	bool bIsForgeMode;
	UPROPERTY()
	AForgeCharacter* ForgeCharacter;
#pragma endregion Possession

#pragma region Inputs
protected:
	virtual void SetupInputComponent() override;
	
#pragma region ForgePlayerActions
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|ForgePlayer")
	UInputMappingContext* ForgePlayerControls;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|ForgePlayer")
	UInputAction* ForgeMovementAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|ForgePlayer")
	UInputAction* ForgeSprintAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|ForgePlayer")
	UInputAction* ForgeLookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|ForgePlayer")
	UInputAction* ForgeHoverAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|ForgePlayer")
	UInputAction* ForgeObjectSelectionAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|ForgePlayer")
	UInputAction* ForgeDeleteObjectAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|ForgePlayer")
	UInputAction* ForgeOpenObjectsWindowAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|ForgePlayer")
	UInputAction* ForgeOpenObjectEditWindowAction;
#pragma endregion ForgePlayerActions
#pragma region ForgePlayerFunctions
protected:
	void SetupForgePlayerInputs();
	void HandleLook(const FInputActionValue& Value);
	void HandleMove(const FInputActionValue& Value);
	void HandleForgeStartSprint(const FInputActionValue& Value);
	void HandleForgeStopSprint(const FInputActionValue& Value);
	void HandleHover(const FInputActionValue& Value);
	void HandleObjectSelection(const FInputActionValue& Value);
	void HandleDeleteObject(const FInputActionValue& Value);
	void HandleOpenObjectsWindow(const FInputActionValue& Value);
	void HandleOpenObjectEditWindow(const FInputActionValue& Value);
	
#pragma endregion ForgePlayerFunctions

#pragma region SharedActions
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Shared")
	UInputMappingContext* SharedControls;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Shared")
	UInputAction* SwitchModeAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Shared")
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
public:
	UForgeModeHudWidget* GetForgeHudWidget() const { return ForgeHudWidget; }
	void UpdateInputs(const bool& HoldingObject = false, const bool& HoveringObject = false);
#pragma endregion UI
	
};
