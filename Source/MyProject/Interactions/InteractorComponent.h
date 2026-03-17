#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractorComponent.generated.h"


class AForgeCharacter;
class UInteractionUI;
class UInteractableComponent;
class IInteractableInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UInteractorComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractorComponent();

	void InitializeUI(APlayerController* InController);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UInteractionUI> InteractionUIClass;
	UPROPERTY()
	UInteractionUI* InteractionUI;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactions")
	float InteractionCheckFirstPersonDistance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactions")
	float InteractionCheckThirdPersonDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool PerformDebugging = false;

public:
	UFUNCTION()
	void OnInteract();
	UFUNCTION(Server, Reliable)
	void Server_RequestInteraction(UObject* InteractionObject);
	UPROPERTY()
	AForgeCharacter* CharacterOwner;

	bool HasInteractionObject() const { return CurrentInteractionObject != nullptr; }
	UObject* GetCurrentInteractionObject() const { return CurrentInteractionObject; }
private:
	void PerformInteractableCheck();
	void OnNewInteractable(UObject* NewInteractable);
	void OnLostInteractable();

	UPROPERTY()
	UObject* CurrentInteractionObject;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
