#pragma once

#include "CoreMinimal.h"
#include "ForgeObjectSettings.h"
#include "MyProject/Interactions/InteractableInterface.h"
#include "ForgeObject.generated.h"

class AForgeCharacter;

UCLASS()
class AForgeObject : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
public:
	AForgeObject();
	
	virtual void OnDrop();
	virtual void OnPickUp();

	void SetPlayMode(const bool& IsPlayMode);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* ForgePreviewMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta=(AllowPrivateAccess="true"))
	UChildActorComponent* GameplayObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character", meta=(AllowPrivateAccess="true"))
	FObjectSettings ObjectSettings;

protected:
	virtual void BeginPlay() override;
	

#pragma region Interactable Interface
public:
	UFUNCTION()
	virtual void OnInteract_Implementation(AForgeCharacter* InCharacter) override;
	virtual void GetInteractionSettings_Implementation(FInteractionSettings& OutSettings) override { OutSettings = InteractionSettings; }
	virtual bool IsInteractable_Implementation() override { return bIsInteractable; }
	virtual void SetIsInteractable_Implementation(bool IsActive) override { bIsInteractable = IsActive;}
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Settings")
	FInteractionSettings InteractionSettings;
private:
	bool bIsInteractable = true;
#pragma endregion Interactable Interface
};
