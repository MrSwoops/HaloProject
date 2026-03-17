// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "BaseCommonMenuButton.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBaseCommonMenuButton : public UCommonButtonBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	UCommonTextBlock* ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText Text;

	void SetText(const FText& NewText);
protected:
	virtual void NativePreConstruct() override;
};
