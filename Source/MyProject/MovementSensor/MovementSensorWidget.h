// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MovementSensorWidget.generated.h"

class UImage;
class UMovementSensorComponent;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UMovementSensorWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UImage* BackgroundImage;
	
	UPROPERTY(BlueprintReadWrite, Category="Radar")
	UMovementSensorComponent* MovementSensor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Radar")
	float RadarRadius = 200.f; // On-screen circle radius

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Radar")
	UTexture2D* IndicatorImage;

	UPROPERTY()
	FSlateBrush DotBrush;

	void Repaint();

protected:
	virtual void NativeConstruct() override;
	
	virtual int32 NativePaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled
	) const override;

};