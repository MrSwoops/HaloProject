#pragma once
#include "BaseGameMode.h"
#include "MultiplayerGameModeBase.generated.h"

class UGameModeStateInfoWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateMatchTimer, float, MatchTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateScore, int32, BlueScore, int32, RedScore, int32, TargetScore);

UCLASS(Blueprintable, BlueprintType)
class AMultiplayerGameModeBase : public ABaseGameMode
{
	GENERATED_BODY()
public:
	AMultiplayerGameModeBase();
	
	virtual void StartRound();
	virtual void EndRound();
	bool IsRoundOngoing = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ScoreToWin;
	int32 BlueTeamScore = 0;
	int32 RedTeamScore = 0;
	virtual void AddPoints(int32 Team, int32 Points);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MatchTime;
protected:
	float MatchTimer;
	void UpdateMatchTimer(float DeltaTime);

public:
	void InitializeGameModeUI();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameModeStateInfoWidget> GameModeUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnUpdateScore OnUpdateScoreDelegate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnUpdateMatchTimer UpdateMatchTimerDelegate;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	//virtual void TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
};
