#include "MultiplayerGameModeBase.h"

#include "ToolBuilderUtil.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject/Characters/GameplayCharacter.h"
#include "MyProject/UI/Multiplayer/GameModeStateInfoWidget.h"

TArray<FString> AMultiplayerGameModeBase::BotNames = {"Freddy", "NoobSlayer :)", "Holly", "Kees", "Hamster Beef"};

FString AMultiplayerGameModeBase::GetRandomBotName()
{
	return "BOT " + BotNames[rand() % BotNames.Num()];
}


AMultiplayerGameModeBase::AMultiplayerGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMultiplayerGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	AssignTeams();
	InitializeGameModeUI();
	StartRound();
}

void AMultiplayerGameModeBase::AssignTeams()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameplayCharacter::StaticClass(), FoundActors);
	bool IsTeam1 = true;
	
	for (AActor* Actor : FoundActors)
	{
		AGameplayCharacter* Character = Cast<AGameplayCharacter>(Actor);
		if (!Character) continue;

		Character->Team = (IsTeam1) ? 1 : 0;
		Character->SetCharacterColor((IsTeam1) ? FColor::Red : FColor::Blue);
		IsTeam1 = !IsTeam1;
	}
}


void AMultiplayerGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsRoundOngoing)
	{
		UpdateMatchTimer(DeltaSeconds);
	}
}


void AMultiplayerGameModeBase::StartRound()
{
	MatchTimer = MatchTime;
	BlueTeamScore = 0;
	RedTeamScore = 0;
	OnUpdateScoreDelegate.Broadcast(BlueTeamScore, RedTeamScore, ScoreToWin);
	IsRoundOngoing = true;
}

void AMultiplayerGameModeBase::EndRound()
{
	IsRoundOngoing = false;
}

void AMultiplayerGameModeBase::InitializeCharacterInventory(UWeaponInventory* Inv)
{
	Inv->InitializeInventoryEquipment(DefaultPlayerInventory);
}


void AMultiplayerGameModeBase::InitializeGameModeUI()
{
	if (GameModeUIClass == nullptr) return;
	UGameModeStateInfoWidget* Widget = CreateWidget<UGameModeStateInfoWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), GameModeUIClass);
	Widget->AddToPlayerScreen();
	
	OnUpdateScoreDelegate.AddDynamic(Widget, &UGameModeStateInfoWidget::UpdateScores);
	UpdateMatchTimerDelegate.AddDynamic(Widget, &UGameModeStateInfoWidget::UpdateTime);
}


void AMultiplayerGameModeBase::UpdateMatchTimer(float DeltaTime)
{
	if (MatchTimer <= 0)
	{
		EndRound();
		return;
	}
	
	MatchTimer -= DeltaTime;
	UpdateMatchTimerDelegate.Broadcast(MatchTimer);
}

void AMultiplayerGameModeBase::AddPoints(int32 Team, int32 Points)
{
	if (Team > 0) RedTeamScore += Points;
	else BlueTeamScore += Points;
	OnUpdateScoreDelegate.Broadcast(BlueTeamScore, RedTeamScore, ScoreToWin);
}
