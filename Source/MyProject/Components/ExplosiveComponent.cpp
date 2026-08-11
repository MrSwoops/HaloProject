// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveComponent.h"

#include <FMODBlueprintStatics.h>

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Weapons/Grenade.h"
#include "../Weapons/Weapon.h"
#include "MyProject/Characters/GameplayCharacter.h"
#include "MyProject/Combat/DamageMessage.h"

// Sets default values for this component's properties
UExplosiveComponent::UExplosiveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ExplodableObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ExplodableObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ExplodableObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Vehicle));
	ExplodableObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
}


// Called when the game starts
void UExplosiveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UExplosiveComponent::CreateExplosion()
{
	FVector ExplosionLocation = GetOwner()->GetActorLocation();
	FFMODEventInstance EventInstance = UFMODBlueprintStatics::PlayEventAtLocation(
		GetWorld(),
		ExplosionSoundEvent,
		GetOwner()->GetActorTransform(),
		true
	);

	TArray<AActor*> IgnoredActors;
	if (GetOwner()) IgnoredActors.Add(GetOwner());
	TArray<AActor*> OutActors;

	auto* WorldPtr = GetWorld();
	if (!WorldPtr) { UE_LOG(LogTemp, Warning, TEXT("Invalid world found when calling UExplosiveComponent::CreateExplosion")); return; }
	bool bHit = UKismetSystemLibrary::SphereOverlapActors(
		WorldPtr,
		ExplosionLocation,
		ExplosionRadius,
		ExplodableObjectTypes,
		nullptr,
		IgnoredActors,
		OutActors
	);

	if (bHit)
	{
		for (AActor* Actor : OutActors)
		{
			ExplodeActor(Actor, Cast<UPrimitiveComponent>(Actor->GetRootComponent()), ExplosionLocation);
		}
	}
}

void UExplosiveComponent::ExplodeActor(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FVector& ExplosionLocation)
{
	const float& Distance = FVector::Dist(ExplosionLocation, OtherActor->GetActorLocation());
	const float& DistanceRatio = (ExplosionRadius - Distance) / ExplosionRadius;
	FVector Velocity = OtherActor->GetActorLocation() - ExplosionLocation;
	Velocity.Z += 0.5f;
	Velocity.Normalize();
	FDamageMessage DmgMsg = FDamageMessage();
	DmgMsg.Damage = CoreDamage * DistanceRatio;
	DmgMsg.HitDirection = Velocity;
	DmgMsg.HitForce = (BaseExplosionForce / 10.0f) * DistanceRatio;
	
	if (OtherActor->Implements<UDamageable>())
	{
		IDamageable::Execute_TakeDamage(OtherActor, DmgMsg);
	}
	else if (AGrenade* Grenade = Cast<AGrenade>(OtherActor))
	{
		Grenade->Arm(0.2f);
	}
	else if (AWeapon* Weapon = Cast<AWeapon>(OtherActor))
	{
		if (!Weapon->Owner)
		{
			Weapon->SkeletalMeshComp->SetEnableGravity(true);
			Weapon->SkeletalMeshComp->SetSimulatePhysics(true);
		}
	}
	
	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(Velocity * BaseExplosionForce * DistanceRatio, ExplosionLocation);
	} 
}
