#include "WeaponProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../WeaponData/ProjectileData.h"
#include "MyProject/Characters/GameplayCharacter.h"
#include "MyProject/Combat/DamageMessage.h"
#include "MyProject/Combat/HurtBox.h"
#include "MyProject/Combat/Interfaces/Damageable.h"
#include "MyProject/Combat/Interfaces/IDamageable.h"


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
		if (OtherActor->Implements<UDamageable>())
		{
			FDamageMessage DmgMsg = FDamageMessage();
			DmgMsg.Damage = Execute_GetDamage(this);
			DmgMsg.Attacker = Shooter;
			DmgMsg.HitDirection = GetActorForwardVector();
			DmgMsg.CritBehavior = ProjectileData->CritHitBehavior;
			
			if (UHurtBox* HurtBox = Cast<UHurtBox>(OtherComp))
			{
				DmgMsg.HitPart = HurtBox->HurtboxType;
			}
			IDamageable::Execute_TakeDamage(OtherActor, DmgMsg);
		}
	}
	if (ProjectileData->HasPenetration)
	{
		
	}
	//else
	ReturnToPool();
	//if (OtherComp->IsSimulatingPhysics()) OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
}

void AWeaponProjectile::FellOutOfWorld(const UDamageType& dmgType)
{
	ReturnToPool();
}

float AWeaponProjectile::GetDamage_Implementation()
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
