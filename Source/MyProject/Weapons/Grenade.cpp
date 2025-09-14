// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

#include "../MyProjectPickUpComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyProject/ExplosiveComponent.h"

// Sets default values
AGrenade::AGrenade()
{
	Armed = false;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollider"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);

	RootComponent = CollisionComp;

	PickUpComponent = CreateDefaultSubobject<UMyProjectPickUpComponent>(TEXT("PickUpComponent"));
	PickUpComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->SetActive(false);

	ExplosiveComp = CreateDefaultSubobject<UExplosiveComponent>(TEXT("Explosive"));
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	Armed = false;
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Armed)
	{
		Timer -= DeltaTime;

		if (Timer <= 0)
		{
			Explode();
		}
	}
}

void AGrenade::SetActive(bool i)
{
	if (i)
	{
		ProjectileMovement->SetUpdatedComponent(RootComponent);
	} else
	{
		Armed = false;
		PickUpComponent->SetActive(i);
	}
	ProjectileMovement->Activate(i);
	
	Super::SetActive(i);
}

void AGrenade::Throw()
{
	PickUpComponent->Enabled = false;
	PickUpComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickUpComponent->SetActive(false);
	SetActive(true);
	ProjectileMovement->SetVelocityInLocalSpace(FVector().ForwardVector * ProjectileMovement->InitialSpeed);
	UGameplayStatics::PlaySoundAtLocation(this, ThrowSound, GetActorLocation());
}


void AGrenade::Arm(const float& ArmTime)
{
	PickUpComponent->Enabled = false;
	Timer = ArmTime;
	Armed = true;
}

void AGrenade::Explode()
{
	Armed = false;
	ExplosiveComp->CreateExplosion();
	ReturnToPool();
}

void AGrenade::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//if (Sticky)
	{
		//ProjectileMovement->Deactivate();
		//OtherActor;
	}
}



void AGrenade::FellOutOfWorld(const UDamageType& dmgType)
{
	ReturnToPool();
}




