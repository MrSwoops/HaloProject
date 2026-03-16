// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionSettings.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

class ACustomCharacter;
class UInteractableComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractableInterface")
	//void OnInteract(ACustomCharacter* InCharacter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractableInterface")
	void GetInteractionSettings(FInteractionSettings& OutSettings);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractableInterface")
	bool IsInteractable();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractableInterface")
	void SetIsInteractable(bool IsActive);
};
