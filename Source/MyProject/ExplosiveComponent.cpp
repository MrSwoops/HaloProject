// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveComponent.h"

#include "GameplayCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/Grenade.h"

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
	if (ExplosionSounds.Num() > 0)
	{
		int32 RandomSoundIndex = FMath::RandRange(0, ExplosionSounds.Num() - 1);
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSounds[RandomSoundIndex], ExplosionLocation);
	} 

	TArray<AActor*> IgnoredActors;
	TArray<AActor*> OutActors;
	
	bool bHit = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
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
	if (AGameplayCharacter* Character = Cast<AGameplayCharacter>(OtherActor))
	{
		Character->TakeDamage(CoreDamage * DistanceRatio);
		FVector Velocity = OtherActor->GetActorLocation() - ExplosionLocation;
		Velocity.Z += 0.5f;
		Velocity.Normalize();
		Character->GetCharacterMovement()->Launch(Velocity * 1500.0f * DistanceRatio);
	}
	else if (AGrenade* Grenade = Cast<AGrenade>(OtherActor))
	{
		Grenade->Arm(Grenade->FuseTime / 2);
	}
	
	if (OtherComp->IsSimulatingPhysics())
	{
		FVector Velocity = OtherActor->GetActorLocation() - ExplosionLocation;
		Velocity.Z += 0.5f;
		Velocity.Normalize();
		OtherComp->AddImpulseAtLocation(Velocity * 75000.0f * DistanceRatio, ExplosionLocation);
	} 
}
