// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeStateInfoWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UGameModeStateInfoWidget::UpdateScore(int CurrentScore, int ScoreToWin, UTextBlock* ScoreText, UProgressBar* ScoreProgress)
{
	ScoreText->SetText(FText::FromString(FString::FromInt(CurrentScore)));
	ScoreProgress->SetPercent(CurrentScore / ScoreToWin);
}

void UGameModeStateInfoWidget::UpdateTime(float Time)
{
	int32 Minutes = FMath::FloorToInt(Time / 60.f);
	int32 Seconds = FMath::FloorToInt(FMath::Fmod(Time, 60.f));

	TimeText->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds)));
}

void UGameModeStateInfoWidget::UpdateGameMode(FText ModeName)
{
	GameModeText->SetText(ModeName);
}
