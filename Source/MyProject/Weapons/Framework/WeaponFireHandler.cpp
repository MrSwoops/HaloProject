#include "WeaponFireHandler.h"

#include "FMODBlueprintStatics.h"
#include "WeaponAmmoHandler.h"
#include "MyProject/PlayerCharacter.h"
#include "MyProject/Components/BulletPoolManager.h"
#include "MyProject/Weapons/WeaponData/WeaponAmmoData.h"
#include "MyProject/Weapons/WeaponData/WeaponFireData.h"

void UWeaponFireHandler::Initialize(UWeaponFireData* FireData, UWeaponAmmoHandler* InAmmoHandler, UProjectileData* InProjectileData, UBulletPoolManager* BulletPoolManager)
{
	BulletManager = BulletPoolManager;
	Spread = FireData->Spread;
	FireRate = FireData->FireRate;
	FireSoundEvent = FireData->FireSoundEvent;
	DryFireSoundEvent = FireData->DryFireSoundEvent;
	FireAnimation = FireData->FireAnimation;

	AmmoHandler = InAmmoHandler;
	
	if (InProjectileData)
		ProjectileData = InProjectileData;
}

bool UWeaponFireHandler::IsReadyToFire()
{
	return !OnFireRateCD && ((AmmoHandler) ? AmmoHandler->CanShoot() : true );
}

void UWeaponFireHandler::PlayFireAnimation()
{
	if (IsPlayerOwned && FireAnimation != nullptr)
	{
		if (UAnimInstance* AnimInstance = Cast<APlayerCharacter>(CharacterOwner)->GetMesh1P()->GetAnimInstance()) // Get the animation object for the arms mesh
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
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
			
			BulletManager->SpawnBullet(SpawnLocation, SpreadRotation, ProjectileData);
			UFMODBlueprintStatics::PlayEventAtLocation(
				GetWorld(), // Or a relevant UObject* from your current world context
				FireSoundEvent,
				WeaponOwner->GetActorTransform(),
				true // bAutoPlay: true to start playing immediately
			);
			PlayFireAnimation();
			if (AmmoHandler != nullptr) AmmoHandler->OnShot();
			//DrawDebugLine(World, SpawnLocation, SpawnLocation + RandomDirection * 1000.0f, FColor::Red, false, 1.0f, 0, 1.0f);
		}
	}
}


void USingleFireHandler::Initialize(UWeaponFireData* FireData, UWeaponAmmoHandler* InAmmoHandler, UProjectileData* InProjectileData, UBulletPoolManager* BulletPoolManager)
{
	Super::Initialize(FireData, InAmmoHandler, InProjectileData, BulletPoolManager);
}

void USingleFireHandler::FirePressed()
{
	if (IsReadyToFire())
	{
		OnFireRateCD = true;
		Super::FirePressed();
		FireBullet();
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { OnFireRateCD = false; }, FireRate, false);
	}
}



void UBurstFireHandler::Initialize(UWeaponFireData* FireData, UWeaponAmmoHandler* InAmmoHandler, UProjectileData* InProjectileData, UBulletPoolManager* BulletPoolManager)
{
	Super::Initialize(FireData, InAmmoHandler, InProjectileData, BulletPoolManager);
	if (UBurstWeaponFireData* BurstData = Cast<UBurstWeaponFireData>(FireData))
	{
		BurstRate = BurstData->BurstRate;
		BurstShots = BurstData->BurstShots;
	}
}

void UPelletFireHandler::Initialize(UWeaponFireData* FireData, UWeaponAmmoHandler* InAmmoHandler, UProjectileData* InProjectileData, UBulletPoolManager* BulletPoolManager)
{
	Super::Initialize(FireData, InAmmoHandler, InProjectileData, BulletPoolManager);
	if (UPelletWeaponFireData* PelletData = Cast<UPelletWeaponFireData>(FireData))
	{
		PelletsPerShot = PelletData->PelletsPerShots;
	}
}

void UPelletFireHandler::FirePressed()
{
	if (IsReadyToFire())
	{
		OnFireRateCD = true;
		Super::FirePressed();
		FireBullet();
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { OnFireRateCD = false; }, FireRate, false);
	}
}

void UPelletFireHandler::FireBullet()
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
		for (int32 i = 0; i < PelletsPerShot; i++)
		{
			FVector RandomDirection = FMath::VRandCone(ForwardVector, ConeHalfAngleRad);
			FRotator SpreadRotation = RandomDirection.Rotation(); // Get new rotation from direction
			
			BulletManager->SpawnBullet(SpawnLocation, SpreadRotation, ProjectileData);
			//DrawDebugLine(World, SpawnLocation, SpawnLocation + RandomDirection * 1000.0f, FColor::Red, false, 1.0f, 0, 1.0f);
		}
		if (AmmoHandler != nullptr) AmmoHandler->OnShot();
		PlayFireAnimation();
	}
}

void UChargeFireHandler::Initialize(UWeaponFireData* FireData, UWeaponAmmoHandler* InAmmoHandler, UProjectileData* InProjectileData, UBulletPoolManager* BulletPoolManager)
{
	Super::Initialize(FireData, InAmmoHandler, InProjectileData, BulletPoolManager);
	if (auto* ChargeData = Cast<UChargeWeaponFireData>(FireData))
	{
		MaxCharge = ChargeData->MaxCharge;
		ChargeSpeed = ChargeData->ChargeSpeed;
		OnlyFireOnFullCharge = ChargeData->OnlyFireOnFullCharge;
		AutoFireOnChargeCompletion = ChargeData->AutoFireOnChargeCompletion;
		ChargeSoundEvent = ChargeData->ChargeSoundEvent;
	}
}

void UChargeFireHandler::FirePressed()
{
	Super::FirePressed();
}

void UChargeFireHandler::FireHeld(float DeltaTime)
{
	if (IsReadyToFire())
	{
		if (CurrentCharge < MaxCharge)
		{
			CurrentCharge += ChargeSpeed * DeltaTime;
		}
		else
		{
			if (AutoFireOnChargeCompletion)
			{
				CurrentCharge = 0;
				OnFireRateCD = true;
				FireBullet();
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { OnFireRateCD = false; }, FireRate, false);
			}
		}
	}
}

void UChargeFireHandler::FireReleased()
{
	Super::FireReleased();
	if (!IsReadyToFire()) return;
	if (OnlyFireOnFullCharge && (CurrentCharge < MaxCharge)) return;

	CurrentCharge = 0;
	OnFireRateCD = true;
	FireBullet();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { OnFireRateCD = false; }, FireRate, false);
}
