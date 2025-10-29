#include "SettingsInterfaceRegistry.h"
#include "Engine/Engine.h"
#include "MyProject/PersistentData/SettingsPersistentData.h"

USettingsInterfaceRegistry* USettingsInterfaceRegistry::Get()
{
	static TWeakObjectPtr<USettingsInterfaceRegistry> Singleton;
	if (!Singleton.IsValid())
	{
		Singleton = NewObject<USettingsInterfaceRegistry>();
		Singleton->AddToRoot(); // Prevent garbage collection
	}
	return Singleton.Get();
}

void USettingsInterfaceRegistry::RegisterObject(UObject* Object)
{
	if (!Object || !Object->GetClass()->ImplementsInterface(USettingsPersistentData::StaticClass()))
	{
		return;
	}

	// Avoid duplicates
	for (const TWeakObjectPtr<UObject>& Obj : RegisteredObjects)
	{
		if (Obj.Get() == Object)
			return;
	}

	RegisteredObjects.Add(Object);
}

void USettingsInterfaceRegistry::UnregisterObject(UObject* Object)
{
	RegisteredObjects.RemoveAll([Object](const TWeakObjectPtr<UObject>& Ptr)
	{
		return !Ptr.IsValid() || Ptr.Get() == Object;
	});
}