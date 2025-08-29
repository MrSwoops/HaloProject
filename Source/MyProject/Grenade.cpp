// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

#include "MyProjectPickUpComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyProjectCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Vehicle));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
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
		Arm(FuseTime);
		ProjectileMovement->Activate(true);
		ProjectileMovement->SetUpdatedComponent(RootComponent);
		ProjectileMovement->SetVelocityInLocalSpace(FVector().ForwardVector * ProjectileMovement->InitialSpeed);
	} else
	{
		Armed = false;
	}
	Super::SetActive(i);
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
	if (ExplosionSound) UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

	TArray<AActor*> IgnoredActors;
	TArray<AActor*> OutActors;
	
	bool bHit = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		ExplosionRadius,
		ObjectTypes,
		nullptr,
		IgnoredActors,
		OutActors
	);

	if (bHit)
	{
		for (AActor* Actor : OutActors)
		{
			OnExplode(Cast<UPrimitiveComponent>(GetRootComponent()), Actor,
				Cast<UPrimitiveComponent>(Actor->GetRootComponent()));
		}
	}
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

void AGrenade::OnExplode(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	const float& Distance = FVector::Dist(GetActorLocation(), OtherActor->GetActorLocation());
	const float& DistanceRatio = (ExplosionRadius - Distance) / ExplosionRadius;
	if (AMyProjectCharacter* Character = Cast<AMyProjectCharacter>(OtherActor))
	{
		Character->TakeDamage(GetDamage() * DistanceRatio);
		FVector Velocity = OtherActor->GetActorLocation() - GetActorLocation();
		Velocity.Z += 0.5f;
		Velocity.Normalize();
		Character->GetCharacterMovement()->Launch(Velocity * 1500.0f * DistanceRatio);
	}
	else if (AGrenade* Grenade = Cast<AGrenade>(OtherActor))
	{
		Grenade->Arm(FuseTime / 2);
	}
	
	if (OtherComp->IsSimulatingPhysics())
	{
		FVector Velocity = OtherActor->GetActorLocation() - GetActorLocation();
		Velocity.Z += 0.5f;
		Velocity.Normalize();
		OtherComp->AddImpulseAtLocation(Velocity * 75000.0f * DistanceRatio, GetActorLocation());
	} 
}

void AGrenade::FellOutOfWorld(const UDamageType& dmgType)
{
	ReturnToPool();
}




