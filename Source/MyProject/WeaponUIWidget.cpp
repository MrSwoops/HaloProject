// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponUIWidget.h"

#include "Components/UniformGridSlot.h"
#include "Components/PanelWidget.h"

void UWeaponUIWidget::SetMagAmmo(const int32& MagSize)
{
	int32 Added = 0;
	for (int32 y = 0; y < BulletRows; y++)
	{
		if (Added >= MagSize) break;
		for (int32 x = 0; x < MagSize / BulletRows; x++)
		{
			//if (Added >= MagSize) break;
			UImage* i = NewObject<UImage>();
			i->SetBrushFromTexture(BulletImage, true);

			FSlateBrush MyBrush = i->GetBrush();
			MyBrush.SetImageSize(FVector2D(20, 20));
			i->SetBrush(MyBrush);
				
			//i->SetDesiredSizeOverride(FVector2D(20, 20));
			BulletPanel->AddChild(i);
			Cast<UUniformGridSlot>(i->Slot)->SetRow(y);
			Cast<UUniformGridSlot>(i->Slot)->SetColumn(x);
			MagAmmoArray.Add(i);

			Added++;
			if (Added >= MagSize) break;
		}
	}	
}

void UWeaponUIWidget::UpdateAmmoUI(const int32& CurrentMagAmmo)
{
	const int32 Index = MagAmmoArray.Num() - CurrentMagAmmo;
	FLinearColor CurrentColor = MagAmmoArray[0]->GetColorAndOpacity();
	FLinearColor NewColor = FLinearColor(CurrentColor.R, CurrentColor.G, CurrentColor.B, 0.3f);
	for (int32 i = 0; i < MagAmmoArray.Num(); i++)
	{
		if (i >= Index)
		{
			NewColor = FLinearColor(CurrentColor.R, CurrentColor.G, CurrentColor.B, 1.0f);
		}
		MagAmmoArray[i]->SetColorAndOpacity(NewColor);
	}
}

void UWeaponUIWidget::InitializeWeaponUI(const int32& CurrentMagAmmo, const int32& MaxMagSize, const int32& CurrentReserveAmmo)
{
	AddToPlayerScreen();
	SetReserveText(CurrentReserveAmmo);
	SetMagAmmo(MaxMagSize);
	UpdateAmmoUI(CurrentMagAmmo);
}


