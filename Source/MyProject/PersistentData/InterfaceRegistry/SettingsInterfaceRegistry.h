#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SettingsInterfaceRegistry.generated.h"

/**
 * Central registry for all objects implementing USettingsPersistentData.
 */
UCLASS()
class MYPROJECT_API USettingsInterfaceRegistry : public UObject
{
	GENERATED_BODY()

public:
	// Returns the global singleton instance (creates if needed)
	static USettingsInterfaceRegistry* Get();

	// Register / unregister interface implementers
	void RegisterObject(UObject* Object);
	void UnregisterObject(UObject* Object);

	// Returns all live interface implementers
	const TArray<TWeakObjectPtr<UObject>>& GetAll() const { return RegisteredObjects; }

private:
	// All registered interface objects (weak to avoid dangling references)
	UPROPERTY()
	TArray<TWeakObjectPtr<UObject>> RegisteredObjects;
};