// #include "SettingsInterfaceRegistry.h"
// #include "Engine/World.h"
// #include "GameFramework/Actor.h"
// #include "MyProject/PersistentData/SettingsPersistentData.h"
//
//
// // Automatically register any object implementing the interface
// static void RegisterPersistentDataObject(UObject* Object)
// {
// 	if (Object && Object->GetClass()->ImplementsInterface(USettingsPersistentData::StaticClass()))
// 	{
// 		USettingsInterfaceRegistry::Get()->RegisterObject(Object);
// 	}
// }
//
// // Automatically unregister on destruction
// static void UnregisterPersistentDataObject(UObject* Object)
// {
// 	if (Object && Object->GetClass()->ImplementsInterface(USettingsPersistentData::StaticClass()))
// 	{
// 		USettingsInterfaceRegistry::Get()->UnregisterObject(Object);
// 	}
// }
//
// // Hook lifecycle events globally
// struct FSettingsPersistentDataHook
// {
// 	FSettingsPersistentDataHook()
// 	{
// 		FCoreUObjectDelegates::OnObjectConstructed.AddLambda([](UObject* NewObj)
// 		{
// 			RegisterPersistentDataObject(NewObj);
// 		});
//
// 		FCoreUObjectDelegates::OnObjectConstructed.AddLambda([](const UObject* DestroyedObj)
// 		{
// 			// const-cast since Unregister expects non-const
// 			UnregisterPersistentDataObject(const_cast<UObject*>(DestroyedObj));
// 		});
// 	}
// };
//
// // Instantiate once on module load
// static FSettingsPersistentDataHook GSettingsPersistentDataHook;