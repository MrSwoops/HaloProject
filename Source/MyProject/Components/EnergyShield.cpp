// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyShield.h"

#include <FMODAudioComponent.h>
#include "MyProject/Combat/HurtBox.h"
#include "EnergyShieldShellSKM.h"
#include "MyProject/Characters/GameplayCharacter.h"
#include "MyProject/Characters/Player/PlayerCharacter.h"
#include "MyProject/Weapons/WeaponData/ProjectileData.h"

// Sets default values for this component's properties
UEnergyShield::UEnergyShield()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	RegenAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("RegenAudioComponent"));
	RegenAudioComponent->bAutoActivate = false;

	if (AGameplayCharacter* Owner = Cast<AGameplayCharacter>(GetOwner()))
	{
		Owner->EnergyShield = this;
		if (Owner->EnergyShieldShell) ShieldMesh = Owner->EnergyShieldShell;
	}
}


// Called when the game starts
void UEnergyShield::BeginPlay()
{
	Super::BeginPlay();
	if (AGameplayCharacter* Owner = Cast<AGameplayCharacter>(GetOwner()))
	{
		if (!ShieldMesh && Owner->EnergyShieldShell) ShieldMesh = Owner->EnergyShieldShell;
	}
	//if (ShieldMesh) ShieldMesh->SetShieldVisibility(false);
	if (Player)
	{
		
		RegenAudioComponent = NewObject<UFMODAudioComponent>(Player, UFMODAudioComponent::StaticClass());
		RegenAudioComponent->SetupAttachment(Player->GetRootComponent());
		RegenAudioComponent->SetEvent(RegenSoundEvent);
	}
	CurrentEnergy = MaxEnergy;
}


// Called every frame
void UEnergyShield::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentEnergy += RegenSpeed * DeltaTime;
	if (Player) Player->OnReceiveDamage.Broadcast(CurrentEnergy, MaxEnergy, 0);
	if (CurrentEnergy >= MaxEnergy)
	{
		CurrentEnergy = MaxEnergy;
		StopEnergyRegen();
		if (ShieldMesh) ShieldMesh->SetShieldVisibility(false);
	}
}

void UEnergyShield::StartEnergyRegen()
{
	SetComponentTickEnabled(true);
	if (ShieldMesh) ShieldMesh->SetShieldMaterials(false);
	if (RegenAudioComponent) RegenAudioComponent->Play();
}

void UEnergyShield::StopEnergyRegen()
{
	SetComponentTickEnabled(false);
	if (RegenAudioComponent) RegenAudioComponent->Stop();
}

void UEnergyShield::ResetRegenDelay()
{
	StopEnergyRegen();
	
	GetWorld()->GetTimerManager().SetTimer(RegenDelayTimerHandle, this, &UEnergyShield::StartEnergyRegen, RegenDelay, false);
}

int32 UEnergyShield::TakeDamage(const FDamageMessage& DmgMsg)
{
	if (DmgMsg.Damage <= 0) return 0;
	if (CurrentEnergy <= 0) return DmgMsg.Damage;

	if (DmgMsg.CritBehavior & InstaKHeadShield && DmgMsg.HitPart == EBodyPart::Head)
	{
		CurrentEnergy = 0;
		return 100;
	}
	
	if (ShieldMesh) ShieldMesh->SetShieldVisibility(true);
	ResetRegenDelay();
	int32 RemainingDamage = 0;
	
	if (DmgMsg.Damage > CurrentEnergy)
	{
		RemainingDamage = DmgMsg.Damage - CurrentEnergy;
		CurrentEnergy = 0;
		if (ShieldMesh) ShieldMesh->SetShieldMaterials(true);
	}
	else
	{
		CurrentEnergy -= DmgMsg.Damage;
	}
	return RemainingDamage;
}

void UEnergyShield::DisableShieldComponent()
{
	if (ShieldMesh) ShieldMesh->SetShieldVisibility(false);
	GetWorld()->GetTimerManager().ClearTimer(RegenDelayTimerHandle);
	StopEnergyRegen();
}

void UEnergyShield::EnableShieldComponent()
{
	if (ShieldMesh) ShieldMesh->SetShieldMaterials(false);
	CurrentEnergy = MaxEnergy;
	SetComponentTickEnabled(true);
}

