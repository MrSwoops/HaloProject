#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerController.generated.h"

class UPlayerHudWidget;
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
	virtual void OnPossess(APawn* InPawn) override;
	virtual void AcknowledgePossession(APawn* P) override;
	
	virtual void OnPossessPlayer(APlayerCharacter* InPossessedPlayer);
	virtual void OnUnPossessPlayer(APlayerCharacter* InPossessedPlayer);

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;
#pragma endregion Possession

#pragma region Inputs
protected:
	virtual void SetupInputComponent() override;
	
#pragma region PlayerActions
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Player", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* PlayerControls;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Player", meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Player", meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Player", meta=(AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Player", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Player", meta = (AllowPrivateAccess = "true"))
	UInputAction* SwapCamAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Player", meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Player", meta=(AllowPrivateAccess = "true"))
	UInputAction* SwapGrenadesAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Player", meta=(AllowPrivateAccess = "true"))
	UInputAction* ThrowGrenadeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Player", meta=(AllowPrivateAccess = "true"))
	UInputAction* SwapWeaponsAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Player", meta=(AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Player", meta=(AllowPrivateAccess = "true"))
	UInputAction* MeleeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Player", meta=(AllowPrivateAccess = "true"))
	UInputAction* UseAbilityAction;
#pragma endregion PlayerActions
#pragma region PlayerFunctions
protected:
	void SetupPlayerInputs();
	void HandlePlayerMove(const FInputActionValue& Value);
	void HandlePlayerLook(const FInputActionValue& Value);
	void HandlePlayerJump(const FInputActionValue& Value);
	void HandlePlayerStopJumping(const FInputActionValue& Value);
	void HandlePlayerCrouch(const FInputActionValue& Value);
	void HandlePlayerUnCrouch(const FInputActionValue& Value);
	void HandlePlayerSwapCam(const FInputActionValue& Value);
	
	void HandlePlayerInteract(const FInputActionValue& Value);

	void HandlePlayerSwapNades(const FInputActionValue& Value);
	void HandlePlayerThrowNade(const FInputActionValue& Value);

	void HandlePlayerSwapWeapons(const FInputActionValue& Value);
	void HandlePlayerReload(const FInputActionValue& Value);
	void HandlePlayerMelee(const FInputActionValue& Value);
	void HandlePlayerUseAbility(const FInputActionValue& Value);
#pragma endregion PlayerFunctions

#pragma endregion Inputs

#pragma region UI
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UPlayerHudWidget> PlayerHudClass;
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UPlayerHudWidget* PlayerHudWidget;
	void InitializePlayerHud(APlayerCharacter* InPlayerCharacter);
	void SetPlayerHudEnabled(const bool& SetEnabled);
#pragma endregion UI
};
