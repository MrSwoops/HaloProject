#pragma once

#include "CoreMinimal.h"
#include "MyProject/Weapons/Weapon.h"
#include "DamageLogEntry.generated.h"

class AGameplayCharacter;
class AWeapon;

UENUM(BlueprintType)
enum EDamageLogType : uint8
{
	Weapon,
	Environment
};

USTRUCT(BlueprintType)
struct FDamageLogEntry
{
	GENERATED_BODY()
public:
	FDamageLogEntry()
		: LogType(Weapon), Damage(0)
	{}
	FDamageLogEntry(EDamageLogType InLogType, float InDamage, AGameplayCharacter* InAttacker = nullptr, TSubclassOf<AWeapon> InWeaponClass = AWeapon::StaticClass())
		: LogType(InLogType), Damage(InDamage), Attacker(InAttacker), WeaponClass(InWeaponClass)
	{}

	EDamageLogType LogType;
	float Damage;

	UPROPERTY()
	AGameplayCharacter* Attacker = nullptr;
	TSubclassOf<AWeapon> WeaponClass;
};