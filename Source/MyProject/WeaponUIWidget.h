// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "WeaponUIWidget.generated.h"

class UImage;
class UListView;
class UPanelWidget;

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWeaponUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* WeaponImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* BulletImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UPanelWidget* BulletPanel;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ReserveAmmo;

	UFUNCTION()
	void SetReserveText(const int32& Ammo)
	{
		ReserveAmmo->SetText(FText::FromString(FString::FromInt(Ammo)));
	}

	UFUNCTION()
	void SetMagAmmo(const int32& MagSize);

	UFUNCTION()
	void UpdateAmmoUI(const int32& CurrentMagAmmo);

	void InitializeWeaponUI(const int32& CurrentMagAmmo, const int32& MaxMagSize, const int32& CurrentReserveAmmo);

protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<UImage*> MagAmmoArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 BulletRows;

};
