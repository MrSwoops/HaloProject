#pragma once

#include "CoreMinimal.h"
#include "WeaponUIData.generated.h"

class UWeaponUIWidget;

UCLASS(BlueprintType, Blueprintable)
class MYPROJECT_API UWeaponUIData : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* WeaponImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ReticleImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AmmoImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagAmmoRows = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UWeaponUIWidget> WeaponUIClass;
	
};

UCLASS(BlueprintType, Blueprintable)
class MYPROJECT_API UEnergyWeaponUIData : public UWeaponUIData
{
	GENERATED_BODY()
public:
	
	
};