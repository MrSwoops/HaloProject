// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "Components/MyProjectPickUpComponent.h"
#include "Weapons/Weapon.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawnerPreviewMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SpawnerPreviewMesh");
	SpawnerPreviewMesh->SetSimulatePhysics(false);
	SpawnerPreviewMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnerPreviewMesh->SetVisibility(false);
	InternalSpawnItem();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AActor* ASpawner::SpawnItem()
{
	FActorSpawnParameters SpawnParams;
	return GetWorld()->SpawnActor<AActor>(SpawnerClass, GetActorLocation(), GetActorRotation(), SpawnParams);
}

void AWeaponSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (SpawnerClass && SpawnerPreviewMesh)
	{
		//SpawnerPreviewMesh->SetSkeletalMesh(Cast<AWeapon>(SpawnerClass)->SkeletalMeshComp->GetSkeletalMeshAsset());
	}
}

AActor* AWeaponSpawner::SpawnItem()
{
	SpawnedWeapon = Cast<AWeapon>(Super::SpawnItem());
	if (SpawnedWeapon == nullptr) return nullptr;
	if (SpawnStatic) SpawnedWeapon->SkeletalMeshComp->SetSimulatePhysics(false);
	SpawnedWeapon->PickUpComp->OnInteractDelegate.AddDynamic(this, &AWeaponSpawner::OnWeaponPickUp);
	return SpawnedWeapon;
}

void AWeaponSpawner::OnWeaponPickUp(AGameplayCharacter* Character)
{
	SpawnedWeapon->PickUpComp->OnInteractDelegate.RemoveDynamic(this, &AWeaponSpawner::OnWeaponPickUp);
	SpawnedWeapon = nullptr;
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AWeaponSpawner::InternalSpawnItem, SpawnDelay, false);
}
