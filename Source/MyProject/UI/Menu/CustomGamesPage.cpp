#include "CustomGamesPage.h"
//#include "CommonButtonBase.h"
#include "Kismet/GameplayStatics.h"

UCustomGamesPage::UCustomGamesPage()
{
	
}

void UCustomGamesPage::NativeConstruct()
{
	Super::NativeConstruct();
	StartGameButton->OnClicked().AddUObject(this, &UCustomGamesPage::StartGame);//.AddDynamic(this, &UCustomGamesPage::StartGame);
}

void UCustomGamesPage::StartGame()
{
	UGameplayStatics::OpenLevel(this, FName(MapName), true, TEXT("?game=/Game/GameModes/" + GameModeName + "." + GameModeName + "_C"));
}
