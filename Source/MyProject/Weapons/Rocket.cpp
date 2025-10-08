// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket.h"

#include <FMODBlueprintStatics.h>

#include "MyProject/Components/ExplosiveComponent.h"

ARocket::ARocket()
{
	ExplosiveComp = CreateDefaultSubobject<UExplosiveComponent>(TEXT("ExplosiveComponent"));
	
	PropelLoopSoundComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("PropelLoopSoundComponent"));
	PropelLoopSoundComponent->SetupAttachment(GetRootComponent());
	
}


void ARocket::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ExplosiveComp->CreateExplosion();
	Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ARocket::SetActive(bool i)
{
	Super::SetActive(i);
	if (i)
	{
		PropelLoopSoundComponent->Play();
		// UFMODBlueprintStatics::PlayEventAttached(
		// 	,
		// 	GetRootComponent(),
		// 	"",
		// 	FVector::ZeroVector,
		// 	EAttachLocation::Type::SnapToTarget,
		// 	true,
		// 	true
		// );
	}
	else
	{
		PropelLoopSoundComponent->Stop();
	}
}

