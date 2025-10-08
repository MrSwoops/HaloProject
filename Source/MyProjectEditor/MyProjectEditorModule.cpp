// #include "MyProjectEditorModule.h"
// #include "AssetToolsModule.h"
// #include "IAssetTools.h"
// #include "MyProject/Weapons/WeaponData/WeaponFireData.h"
// #include "AssetTypeActions_WeaponFireData.h"
//
// #define LOCTEXT_NAMESPACE "FMyProjectEditorModule"
//
// void FMyProjectEditorModule::StartupModule()
// {
// 	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
//
// 	// Register a custom category for all weapon-related data assets
// 	EAssetTypeCategories::Type WeaponDataCategory = AssetTools.RegisterAdvancedAssetCategory(
// 		FName(TEXT("WeaponDataAssets")),
// 		LOCTEXT("WeaponDataAssetsCategory", "Weapon Data Assets")
// 	);
//
// 	// Register UWeaponFireData (and its subclasses) in this category
// 	WeaponFireDataAssetActions = MakeShared<FAssetTypeActions_WeaponFireData>(WeaponDataCategory);
// 	AssetTools.RegisterAssetTypeActions(WeaponFireDataAssetActions.ToSharedRef());
// }
//
// void FMyProjectEditorModule::ShutdownModule()
// {
// 	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
// 	{
// 		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
// 		if (WeaponFireDataAssetActions.IsValid())
// 		{
// 			AssetTools.UnregisterAssetTypeActions(WeaponFireDataAssetActions.ToSharedRef());
// 		}
// 	}
// }
//
// #undef LOCTEXT_NAMESPACE
//
// IMPLEMENT_MODULE(FMyProjectEditorModule, MyProjectEditor)