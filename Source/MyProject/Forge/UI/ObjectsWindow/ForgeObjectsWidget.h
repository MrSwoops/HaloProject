// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CommonActivatableWidgetSwitcher.h"
#include "CommonHierarchicalScrollBox.h"
#include "MyProject/UI/Common/BaseCommonMenuButton.h"
#include "ForgeObjectsWidget.generated.h"

class AForgeObject;

USTRUCT(BlueprintType, Blueprintable)
struct FForgeObjectButtonInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ButtonText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AForgeObject> ObjectToSpawn;
};

UCLASS()
class MYPROJECT_API UForgeObjectsWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	UCommonActivatableWidgetSwitcher* WidgetSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	UCommonHierarchicalScrollBox* CategoryScrollBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	UCommonHierarchicalScrollBox* HumanWeapons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	UCommonHierarchicalScrollBox* CovWeapons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	UCommonHierarchicalScrollBox* Vehicles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	UCommonHierarchicalScrollBox* Spawning;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	UCommonHierarchicalScrollBox* Objectives;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	UCommonHierarchicalScrollBox* Props;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	UCommonHierarchicalScrollBox* Structures;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UBaseCommonMenuButton> ButtonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<FForgeObjectButtonInfo> HumanWeaponButtons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<FForgeObjectButtonInfo> CovWeaponButtons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<FForgeObjectButtonInfo> VehiclesButtons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<FForgeObjectButtonInfo> SpawningButtons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<FForgeObjectButtonInfo> ObjectivesButtons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<FForgeObjectButtonInfo> PropsButtons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<FForgeObjectButtonInfo> StructuresButtons;
	
	void InitializeCategories();
	void InitializeCategory(UCommonHierarchicalScrollBox* InScrollBox, const TArray<FForgeObjectButtonInfo>& InInfo);

	void SpawnForgeObject(TSubclassOf<AForgeObject> ObjectClass);
};
