// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/VerticalBox.h"
#include "MyProject/UI/InputBindingDisplay/InputDisplayWidget.h"
#include "ForgeModeHudWidget.generated.h"

class UForgeObjectsWidget;
class UForgeObjectEditMenu;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UForgeModeHudWidget : public UCommonUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ForgeModeHudWidget, meta = (BindWidget))
	UForgeObjectEditMenu* ObjectEditWindow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ForgeModeHudWidget, meta = (BindWidget))
	UForgeObjectsWidget* ObjectsWindow;

#pragma region InputDisplay
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ForgeModeHudWidget, meta = (BindWidget))
	UVerticalBox* InputDisplayBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ForgeModeHudWidget, meta = (BindWidget))
	UInputDisplayWidget* DeleteObjectInputDisplay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ForgeModeHudWidget, meta = (BindWidget))
	UInputDisplayWidget* PickupObjectInputDisplay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ForgeModeHudWidget, meta = (BindWidget))
	UInputDisplayWidget* PlaceObjectInputDisplay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ForgeModeHudWidget, meta = (BindWidget))
	UInputDisplayWidget* OpenObjectsWindowInputDisplay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ForgeModeHudWidget, meta = (BindWidget))
	UInputDisplayWidget* OpenEditObjectWindowInputDisplay;

	void UpdateInputDisplay(const bool& HoldingObject = false, const bool& HoveringObject = false);
#pragma endregion InputDisplay
	
	void SetObjectEditWindowEnabled(bool Enabled);
	void SetObjectsWindowEnabled(bool Enabled);
	bool IsSubWindowOpen() const { return bSubWindowOpen; }
private:
	bool bSubWindowOpen = false;

protected:
	virtual void NativeConstruct() override;
};
