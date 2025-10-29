#include "MultiplayerGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject/PlayerCharacter.h"
#include "MyProject/UI/Multiplayer/GameModeStateInfoWidget.h"

AMultiplayerGameModeBase::AMultiplayerGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMultiplayerGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeGameModeUI();
	StartRound();
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
