// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CommonTextBlock.h"
#include "GameModeStateInfoWidget.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UGameModeStateInfoWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonTextBlock* TimeText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonTextBlock* GameModeText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonTextBlock* BlueScoreText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* BlueProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonTextBlock* RedScoreText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* RedProgress;

	UFUNCTION()
	void UpdateScores(int32 BlueScore, int32 RedScore, int32 ScoreToWin)
	{ UpdateTeam1Score(BlueScore, ScoreToWin); UpdateTeam2Score(RedScore, ScoreToWin); }
	void UpdateTeam1Score(int32 CurrentScore, int32 ScoreToWin) { UpdateScore(CurrentScore, ScoreToWin, BlueScoreText, BlueProgress); }
	void UpdateTeam2Score(int32 CurrentScore, int32 ScoreToWin) { UpdateScore(CurrentScore, ScoreToWin, RedScoreText, RedProgress); }
private:
	void UpdateScore(int32 CurrentScore, int32 ScoreToWin, UTextBlock* ScoreText, UProgressBar* ScoreProgress);
public:
	UFUNCTION()
	void UpdateTime(float Time);
	UFUNCTION()
	void UpdateGameMode(FText ModeName);
};
