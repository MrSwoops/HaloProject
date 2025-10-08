// #pragma once
//
// #include "AssetTypeActions_Base.h"
// #include "MyProject/Weapons/WeaponData/WeaponFireData.h"
//
// class FAssetTypeActions_WeaponFireData : public FAssetTypeActions_Base
// {
// public:
// 	FAssetTypeActions_WeaponFireData(EAssetTypeCategories::Type InCategory)
// 		: MyCategory(InCategory)
// 	{}
//
// 	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "WeaponFireData", "Weapon Fire Data"); }
// 	virtual FColor GetTypeColor() const override { return FColor(255, 140, 0); }
// 	virtual UClass* GetSupportedClass() const override { return UWeaponFireData::StaticClass(); }
// 	virtual uint32 GetCategories() override { return MyCategory; }
//
// private:
// 	EAssetTypeCategories::Type MyCategory;
// };
