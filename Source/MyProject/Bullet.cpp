// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/IDamageable.h"

ABullet::ABullet() 
{
	HitCollider = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollider"));
	HitCollider->InitSphereRadius(5.0f);
	HitCollider->BodyInstance.SetCollisionProfileName("Projectile");
	HitCollider->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	RootComponent = HitCollider;
	
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	Movement->UpdatedComponent = HitCollider;
	Movement->InitialSpeed = 3000.f;
	Movement->MaxSpeed = 3000.f;
	Movement->bRotationFollowsVelocity = true;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	// ...
}


void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ReturnToPool();
	if ((OtherActor == nullptr) || (OtherActor == this) || (OtherComp == nullptr)) return;
	
	if (OtherComp->IsSimulatingPhysics()) OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	
	if (auto* Damageable = Cast<IIDamageable>(OtherActor))
	{	
		Damageable->TakeDamage(this);
	}
}

void ABullet::FellOutOfWorld(const UDamageType& dmgType)
{
	ReturnToPool();
}


void ABullet::SetActive(bool i)
{
	Super::SetActive(i);
	if (i)
	{
		Movement->Activate(true);
		Movement->SetUpdatedComponent(RootComponent);
		Movement->SetVelocityInLocalSpace(FVector().ForwardVector * Movement->InitialSpeed);
		//Movement->BeginPlay();
	}
	else
	{
		//Movement->Deactivate();
	}
}

const float& ABullet::GetDamage()
{
	return Damage;
}



// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
