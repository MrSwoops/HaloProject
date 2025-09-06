// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"

#include "ExplosiveComponent.h"
#include "Interfaces/DamageDealer.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ExplosiveComponent = CreateDefaultSubobject<UExplosiveComponent>(TEXT("Explosive"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosiveBarrel::TakeDamage(IDamageDealer* DD)
{
	TakeDamage(DD->GetDamage());
}

void AExplosiveBarrel::TakeDamage(const int32& Damage)
{
	Health -= Damage;
	if (Health <= 0)
	{
		ExplosiveComponent->CreateExplosion();
		Mesh->SetSimulatePhysics(false);
		Mesh->SetVisibility(false);
		Mesh->SetCollisionProfileName("NoCollision");
	}
}

void AExplosiveBarrel::Reset()
{
	Health = MaxHealth;
	Mesh->SetCollisionProfileName("PhysicsActor");
	Mesh->SetVisibility(true);
	Mesh->SetSimulatePhysics(true);
}
