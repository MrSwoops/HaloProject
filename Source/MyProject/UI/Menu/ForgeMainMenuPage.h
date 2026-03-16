// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "MyProject/UI/Common/CommonActivatableWidgetPage.h"
#include "ForgeMainMenuPage.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UForgeMainMenuPage : public UCommonActivatableWidgetPage
{
	GENERATED_BODY()

public:
	FString GameModeName = "TeamSlayer";
	FString MapName = "Foundry";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCommonButtonBase* StartGameButton;

protected:
	UFUNCTION()
	void StartGame();

	virtual void NativeConstruct() override;
};
