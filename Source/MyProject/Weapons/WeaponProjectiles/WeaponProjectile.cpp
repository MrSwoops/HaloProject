#include "WeaponProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MyProject/Components/HurtBox.h"
#include "MyProject/Interfaces/IDamageable.h"
#include "../WeaponData/ProjectileData.h"


AWeaponProjectile::AWeaponProjectile() 
{
	HitCollider = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollider"));
	HitCollider->InitSphereRadius(5.0f);
	HitCollider->BodyInstance.SetCollisionProfileName("Projectile");
	HitCollider->OnComponentHit.AddDynamic(this, &AWeaponProjectile::OnHit);

	RootComponent = HitCollider;
	
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	Movement->UpdatedComponent = HitCollider;
	Movement->InitialSpeed = 3000.f;
	Movement->MaxSpeed = 3000.f;
	Movement->bRotationFollowsVelocity = true;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSystem"));
	NiagaraComponent->SetupAttachment(RootComponent);
}

void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	// ...
}


void AWeaponProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherComp && OtherActor)
	{
		//if ((OtherActor == this) || (OtherComp == nullptr)) return;
		if (auto* Damageable = Cast<IIDamageable>(OtherActor))
		{
			if (UHurtBox* HurtBox = Cast<UHurtBox>(OtherComp))
			{
				Damageable->TakeProjectileDamage(this, HurtBox->HurtboxType);
				
				if (ProjectileData->HasPenetration && HurtBox->HurtboxType != EHurtboxType::Head)
					ReturnToPool();
				else
					ReturnToPool();
				
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
		else
		{
			ReturnToPool();
		}
	}
	else
	{
		ReturnToPool();
	}
	
	//if (OtherComp->IsSimulatingPhysics()) OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	
	
}

void AWeaponProjectile::FellOutOfWorld(const UDamageType& dmgType)
{
	ReturnToPool();
}

const float  AWeaponProjectile::GetDamage()
{
	return (ProjectileData) ? ProjectileData->Damage : 0;
}

void AWeaponProjectile::EnableBulletTrail()
{
	if (BulletTrailSystem && NiagaraComponent)
	{
		NiagaraComponent->SetAsset(BulletTrailSystem); 
		NiagaraComponent->Activate(); 
	}
}

void AWeaponProjectile::DisableBulletTrail()
{
	if (NiagaraComponent)
	{
		NiagaraComponent->Deactivate();
	}
}


void AWeaponProjectile::SetActive(bool i)
{
	Super::SetActive(i);
	if (i)
	{
		Movement->Activate(true);
		Movement->SetUpdatedComponent(RootComponent);
		Movement->SetVelocityInLocalSpace(FVector().ForwardVector * Movement->InitialSpeed);
		EnableBulletTrail();
	}
	else
	{
		DisableBulletTrail();
		Shooter = nullptr;
	}
}

void AWeaponProjectile::LoadProjectileData(UProjectileData* InBulletData)
{
	ProjectileData = InBulletData;
	Movement->InitialSpeed = InBulletData->Speed;
	Movement->MaxSpeed = InBulletData->Speed;
}


// Called every frame
void AWeaponProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
