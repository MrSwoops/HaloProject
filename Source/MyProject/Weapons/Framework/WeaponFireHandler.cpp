#include "WeaponFireHandler.h"

#include "MyProject/PlayerCharacter.h"
#include "MyProject/Weapons/WeaponData/WeaponFireData.h"

void UWeaponFireHandler::Initialize(UWeaponFireData* FireData, UBulletPoolManager* BulletPoolManager)
{
	BulletManager = BulletPoolManager;
	Spread = FireData->Spread;
	FireRate = FireData->FireRate;
	FireSoundEvent = FireData->FireSoundEvent;
	DryFireSoundEvent = FireData->DryFireSoundEvent;
	FireAnimation = FireData->FireAnimation;
}

void UWeaponFireHandler::GetBulletSpawnInfo(FVector& SpawnLocation, FVector& Direction)
{
	
}


void UWeaponFireHandler::GetPlayerBulletSpawnInfo(FVector& SpawnLocation, FVector& Direction)
{
	const APlayerController* PlayerController = Cast<APlayerController>(CharacterOwner->GetController());
	FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	SpawnLocation = CharacterOwner->GetActorLocation() + SpawnRotation.RotateVector(*MuzzleOffset);

	// Cast ray to get more accurate direction
	FVector Start = PlayerController->PlayerCameraManager->GetCameraLocation();
	FVector End = Start + (PlayerController->PlayerCameraManager->GetActorForwardVector() * 2000.f);
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(CharacterOwner);
	CollisionParams.AddIgnoredActor(WeaponOwner);
	DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 5.f, 0, 1.f);
#define ECC_Hurtbox ECC_GameTraceChannel4
	bool bHit = GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		Start,
		End,
		FCollisionObjectQueryParams(ECC_Hurtbox), // Object channel(s) to hit
		CollisionParams
	);
	Direction = (bHit) ? (HitResult.ImpactPoint - SpawnLocation).GetSafeNormal() : SpawnRotation.Vector();
}


void UWeaponFireHandler::FireBullet()
{
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			FVector Direction;
			FVector SpawnLocation;
			if (IsPlayerOwned)
				GetPlayerBulletSpawnInfo(SpawnLocation, Direction);
			else
				GetBulletSpawnInfo(SpawnLocation, Direction);
			
			//if (bHit) DrawDebugLine(GetWorld(), SpawnLocation, HitResult.ImpactPoint, FColor::Green, false, 5.f, 0, 1.f); else DrawDebugLine(GetWorld(), SpawnLocation, SpawnLocation + (Direction * 20000), FColor::Red, false, 5.f, 0, 1.f);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			FVector ForwardVector = Direction;//SpawnRotation.Vector(); // Converts rotation to direction vector
			float ConeHalfAngleRad = FMath::DegreesToRadians(Spread); // Spread is an angle in radians. Convert degrees if needed:
			FVector RandomDirection = FMath::VRandCone(ForwardVector, ConeHalfAngleRad);
			FRotator SpreadRotation = RandomDirection.Rotation(); // Get new rotation from direction
			
			BulletManager->SpawnBullet(SpawnLocation, SpreadRotation, *WeaponType);
			//DrawDebugLine(World, SpawnLocation, SpawnLocation + RandomDirection * 1000.0f, FColor::Red, false, 1.0f, 0, 1.0f);
		}
	}
}


void USingleFireHandler::Initialize(UWeaponFireData* FireData, UBulletPoolManager* BulletPoolManager)
{
	Super::Initialize(FireData, BulletPoolManager);
}

void USingleFireHandler::FirePressed()
{
	if (!OnFireRateCD)
	{
		OnFireRateCD = true;
		Super::FirePressed();
		FireBullet();
		if (IsPlayerOwned && FireAnimation != nullptr)
		{
			if (UAnimInstance* AnimInstance = Cast<APlayerCharacter>(CharacterOwner)->GetMesh1P()->GetAnimInstance()) // Get the animation object for the arms mesh
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { OnFireRateCD = false; }, FireRate, false);
	}
}



void UBurstFireHandler::Initialize(UWeaponFireData* FireData, UBulletPoolManager* BulletPoolManager)
{
	Super::Initialize(FireData, BulletPoolManager);
	if (UBurstWeaponFireData* BurstData = Cast<UBurstWeaponFireData>(FireData))
	{
		BurstRate = BurstData->BurstRate;
		BurstShots = BurstData->BurstShots;
	}
}

void UPelletFireHandler::Initialize(UWeaponFireData* FireData, UBulletPoolManager* BulletPoolManager)
{
	Super::Initialize(FireData, BulletPoolManager);
	if (UPelletWeaponFireData* PelletData = Cast<UPelletWeaponFireData>(FireData))
	{
		PelletsPerShot = PelletData->PelletsPerShots;
	}
}
