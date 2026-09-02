// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "WeaponUIWidget.generated.h"

class UWeaponUIData;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UPanelWidget* BulletPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ReserveAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* WeaponReticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* BulletImage;

	UFUNCTION()
	void SetReserveText(const int32& Ammo)
	{
		ReserveAmmo->SetText(FText::FromString(FString::FromInt(Ammo)));
	}
	void SetReserveText(const float& Ammo)
	{
		ReserveAmmo->SetText(FText::FromString(FString::FromInt(FMath::CeilToInt(Ammo))));
	}

	UFUNCTION()
	virtual void SetMagAmmo(const int32& MagSize);

	UFUNCTION()
	virtual void UpdateAmmoUI(const int32& CurrentMagAmmo);
	virtual void InitializeWeaponUI(const int32& CurrentMagAmmo, const int32& MaxMagSize, const int32& CurrentReserveAmmo, UWeaponUIData* UIData);
	
	virtual void UpdateAmmoUI(const float& CurrentMagAmmo) {}
	virtual void InitializeWeaponUI(const float& CurrentMagAmmo, const float& CurrentReserveAmmo, UWeaponUIData* UIData){}

protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<UImage*> MagAmmoArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 BulletRows;

};
