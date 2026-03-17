// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTextBlock.h"
#include "CommonUserWidget.h"
#include "Components/ProgressBar.h"
#include "PlayerHudWidget.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UPlayerHudWidget : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	void InitializeConnections(APlayerCharacter* InPlayerCharacter);

	UFUNCTION()
	void UpdateHealthBar(const float& CurrentShields, const float& MaxShields, const int32& DamageTaken);
	UFUNCTION()
	void UpdateInteractionText(bool IsActive, const FString& InString);
	UFUNCTION()
	void UpdateGrenadeUI(const int32& CurrentGrenadeSelection, const int32& RegularGrenadesCount, const int32& PlasmaGrenadesCount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	UCommonTextBlock* InteractText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	UCommonTextBlock* GrenadesText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	UCommonTextBlock* PGrenadesText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	UProgressBar* HealthProgress;
};
