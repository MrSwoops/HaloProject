// Fill out your copyright notice in the Description page of Project Settings.


#include "ForgeObjectsWidget.h"

#include "MyProject/Forge/Character/ForgeCharacter.h"
#include "MyProject/Forge/Character/ForgePlayerController.h"
#include "MyProject/Forge/ForgeObject/ForgeObject.h"
#include "MyProject/Forge/UI/ForgeModeHudWidget.h"

void UForgeObjectsWidget::InitializeCategories()
{
	InitializeCategory(HumanWeapons, HumanWeaponButtons);
	InitializeCategory(CovWeapons, CovWeaponButtons);
	InitializeCategory(Vehicles, VehiclesButtons);
	InitializeCategory(Spawning, SpawningButtons);
	InitializeCategory(Objectives, ObjectivesButtons);
	InitializeCategory(Props, PropsButtons);
	InitializeCategory(Structures, StructuresButtons);
}
void UForgeObjectsWidget::InitializeCategory(UCommonHierarchicalScrollBox* InScrollBox, const TArray<FForgeObjectButtonInfo>& InInfo)
{
	if (!InScrollBox || !ButtonClass) return;

	InScrollBox->ClearChildren();
	
	// Back Button
	auto* BackButton = CreateWidget<UBaseCommonMenuButton>(InScrollBox, ButtonClass);
	BackButton->SetText(FText::FromString("<"));
	BackButton->OnClicked().AddLambda([this]()
	{
		WidgetSwitcher->SetActiveWidgetIndex(0);
	});
	
	for (auto ButtonInfo : InInfo) // Category Contents
	{
		auto* ContentButton = CreateWidget<UBaseCommonMenuButton>(InScrollBox, ButtonClass);
		ContentButton->SetText(ButtonInfo.ButtonText);
		TSubclassOf<AForgeObject> ObjectClass = ButtonInfo.ObjectToSpawn;
		ContentButton->OnClicked().AddLambda([this, ObjectClass]()
		{
			SpawnForgeObject(ObjectClass);
		});
		InScrollBox->AddChild(ContentButton);
	}
}

void UForgeObjectsWidget::SpawnForgeObject(TSubclassOf<AForgeObject> ObjectClass)
{
	if (!ObjectClass) return;

	auto* ForgeCharacter = Cast<AForgeCharacter>(GetOwningPlayer()->GetPawn());
	if (!ForgeCharacter) return;
	
	const FVector SpawnLocation = ForgeCharacter->GetActorLocation() + (ForgeCharacter->GetActorForwardVector() * 300);
	FActorSpawnParameters SpawnParams;
	auto* ForgeObject = GetWorld()->SpawnActor<AForgeObject>(ObjectClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	if (!ForgeObject) return;

	ForgeCharacter->PickupObject(ForgeObject);
	Cast<AForgePlayerController>(GetOwningPlayer())->GetForgeHudWidget()->SetObjectsWindowEnabled(false);
	 
}
