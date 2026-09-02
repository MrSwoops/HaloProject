// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyShieldShellSKM.h"

UEnergyShieldShellSKM::UEnergyShieldShellSKM()
{
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetOwnerNoSee(true);
	SetNotifyRigidBodyCollision(false);
	SetShieldMaterials(false);
}

void UEnergyShieldShellSKM::BeginPlay()
{
	Super::BeginPlay();
	SetShieldMaterials(false);
}



void UEnergyShieldShellSKM::SetShieldMaterials(bool Broken)
{
	if (Broken)
	{
		for (int32 i = GetNumMaterials() - 1; i >= 0; i--)
		{
			if (BrokenShieldMat) SetMaterial(i, BrokenShieldMat);
		}
	}
	else
	{
		for (int32 i = GetNumMaterials() - 1; i >= 0; i--)
		{
			if (ShieldMat) SetMaterial(i, ShieldMat);
		}
	}
}

void UEnergyShieldShellSKM::SetShieldVisibility(bool IsVisible)
{
	SetVisibility(IsVisible);
}

