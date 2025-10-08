// Fill out your copyright notice in the Description page of Project Settings.


#include "FireDataFactory.h"

#include "MyProject/Weapons/WeaponData/WeaponFireData.h"

UFireDataFactory::UFireDataFactory()
{
	// This factory will create instances of UMyCustomDataAsset
	SupportedClass = UWeaponFireData::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UFireDataFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UWeaponFireData>(Context, UWeaponFireData::StaticClass(), InName, Flags);
}