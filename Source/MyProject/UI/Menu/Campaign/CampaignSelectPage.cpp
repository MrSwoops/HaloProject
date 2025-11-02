#include "CampaignSelectPage.h"

#include "Kismet/GameplayStatics.h"

UCampaignSelectPage::UCampaignSelectPage()
{
	
}

void UCampaignSelectPage::NativeConstruct()
{
	Super::NativeConstruct();
	StartGameButton->OnClicked().AddUObject(this, &UCampaignSelectPage::StartGame);//.AddDynamic(this, &UCustomGamesPage::StartGame);
}

void UCampaignSelectPage::StartGame()
{
	UGameplayStatics::OpenLevel(this, FName(LevelName));
}