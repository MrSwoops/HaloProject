#pragma once

#include "CoreMinimal.h"
#include "BodyPart.h"
#include "MyProject/Weapons/WeaponData/ProjectileData.h"
#include "DamageMessage.generated.h"

USTRUCT(Blueprintable)
struct FDamageMessage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageMessage")
	AActor* Attacker = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageMessage")
	float Damage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageMessage")
	EBodyPart HitPart = EBodyPart::Undefined;
	//UPROPERTY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageMessage")
	FVector HitDirection = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageMessage")
	float HitForce = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<CritHitBehavior> CritBehavior = TEnumAsByte<CritHitBehavior>::EnumType::Default;
};
