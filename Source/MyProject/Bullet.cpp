// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/HurtBox.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/IDamageable.h"
#include "Weapons/BulletData.h"

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
	if (OtherComp && OtherActor)
	{
		if (auto* Damageable = Cast<IIDamageable>(OtherActor))
		{
			if (UHurtBox* HurtBox = Cast<UHurtBox>(OtherComp))
			{
				if (BulletData.HasPenetration && HurtBox->HurtboxType != EHurtboxType::Head)
					ReturnToPool();
				else
					ReturnToPool();
				
				Damageable->TakeBulletDamage(BulletData, HurtBox->HurtboxType);
				// switch (HurtBox->HurtboxType)
				// {
				// case Head:
				// 	Damageable->TakeDamage(BulletData::);
				// 	break;
				// case Limb:
				// 	Damageable->TakeDamage(this);
				// 	break;
				// case Torso:
				// 	Damageable->TakeDamage(this);
				// 	break;
				// default:
				// 	Damageable->TakeDamage(this);
				// 	break;
				// };
			}
			else
			{
				Damageable->TakeDamage(this);
				ReturnToPool();
			}
		}
	}
	else
	{
		ReturnToPool();
	}
	if ((OtherActor == this) || (OtherComp == nullptr)) return;
	
	//if (OtherComp->IsSimulatingPhysics()) OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	
	
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

void ABullet::LoadBulletData(FBulletData InBulletData = BulletData::Default)
{
	Damage = InBulletData.Damage;
	Movement->InitialSpeed = InBulletData.Speed;
	Movement->MaxSpeed = InBulletData.Speed;
	this->BulletData = InBulletData;
}


// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
