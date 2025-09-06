// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "HurtBox.h"
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
	
	if (GEngine)
	{
		FString T = Hit.GetActor()->GetName() + ": " + Hit.GetComponent()->GetName() +  "Hit. Bone Hit: " + Hit.BoneName.ToString();
		GEngine->AddOnScreenDebugMessage(
			-1,                  // Key: Unique ID for the message (-1 for a new message)
			5.0f,                // TimeToDisplay: Duration in seconds to display the message
			FColor::Yellow,      // DisplayColor: Color of the text
			T // DebugMessage: The string to display
		);
	}
	ReturnToPool();
	if (OtherComp && OtherActor)
	{
		if (auto* Damageable = Cast<IIDamageable>(OtherActor))
		{
			if (UHurtBox* HurtBox = Cast<UHurtBox>(OtherComp))
			{
				switch (HurtBox->HurtboxType)
				{
				case Head:
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(
							-1,                  // Key: Unique ID for the message (-1 for a new message)
							5.0f,                // TimeToDisplay: Duration in seconds to display the message
							FColor::Yellow,      // DisplayColor: Color of the text
							TEXT("Head") // DebugMessage: The string to display
						);
					}
					Damageable->TakeDamage(Damage * 2);
					break;
				case Limb:
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(
							-1,                  // Key: Unique ID for the message (-1 for a new message)
							5.0f,                // TimeToDisplay: Duration in seconds to display the message
							FColor::Yellow,      // DisplayColor: Color of the text
							TEXT("Limb") // DebugMessage: The string to display
						);
					}
					Damageable->TakeDamage(this);
					break;
				case Torso:
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(
							-1,                  // Key: Unique ID for the message (-1 for a new message)
							5.0f,                // TimeToDisplay: Duration in seconds to display the message
							FColor::Yellow,      // DisplayColor: Color of the text
							TEXT("Torso") // DebugMessage: The string to display
						);
					}
					Damageable->TakeDamage(this);
					break;
				default:
					Damageable->TakeDamage(this);
					break;
				};
			}
			else
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1,                  // Key: Unique ID for the message (-1 for a new message)
						5.0f,                // TimeToDisplay: Duration in seconds to display the message
						FColor::Yellow,      // DisplayColor: Color of the text
						TEXT("No Hurtbox") // DebugMessage: The string to display
					);
				}
				Damageable->TakeDamage(this);
			}
		}
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



// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
