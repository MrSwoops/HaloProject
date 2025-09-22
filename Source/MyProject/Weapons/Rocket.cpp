// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket.h"

#include "MyProject/Components/ExplosiveComponent.h"

ARocket::ARocket()
{
	ExplosiveComp = CreateDefaultSubobject<UExplosiveComponent>(TEXT("ExplosiveComponent"));
}


void ARocket::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ExplosiveComp->CreateExplosion();
	Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}
