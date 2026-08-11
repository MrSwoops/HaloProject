#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

struct FDamageMessage;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT_API IDamageable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DamageableInterface")
	void TakeDamage(const FDamageMessage& DmgMsg);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DamageableInterface")
	void Die();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DamageableInterface")
	bool IsDead() const;
};
