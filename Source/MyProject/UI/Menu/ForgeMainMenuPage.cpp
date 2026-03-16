// Fill out your copyright notice in the Description page of Project Settings.


#include "ForgeMainMenuPage.h"

#include "Kismet/GameplayStatics.h"

void UForgeMainMenuPage::NativeConstruct()
{
	Super::NativeConstruct();
	StartGameButton->OnClicked().AddUObject(this, &UForgeMainMenuPage::StartGame);//.AddDynamic(this, &UCustomGamesPage::StartGame);
}

void UForgeMainMenuPage::StartGame()
{
	UGameplayStatics::OpenLevel(this, FName(MapName), true, TEXT("?game=/Game/Forge/GM_ForgeBuilder.GM_ForgeBuilder_C"));
}