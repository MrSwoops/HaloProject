#include "ForgeCharacter.h"

#include "ForgePlayerController.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "MyProject/Forge/ForgeBuilderGameMode.h"
#include "MyProject/Forge/ForgeObject/ForgeObject.h"
#include "MyProject/Interactions/InteractorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

AForgeCharacter::AForgeCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(SphereComp);
	SphereComp->SetSimulatePhysics(true);
	SphereComp->SetEnableGravity(false);
	SphereComp->SetCollisionProfileName(TEXT("PhysicsActor"));
	SphereComp->BodyInstance.bLockXRotation = true;
	SphereComp->BodyInstance.bLockYRotation = true;
	SphereComp->BodyInstance.bLockZRotation = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterMesh"));
	Mesh->SetupAttachment(SphereComp);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(Mesh);
	FirstPersonCamera->bUsePawnControlRotation = true;

	InteractorComponent = CreateDefaultSubobject<UInteractorComponent>(TEXT("InteractorComponent"));
	InteractorComponent->SetupAttachment(FirstPersonCamera);

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
}

void AForgeCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Mesh)
	{
		FRotator CamRot = FirstPersonCamera->GetComponentRotation();
		FRotator NewRot = Mesh->GetComponentRotation();
		NewRot.Yaw = CamRot.Yaw;
		NewRot.Pitch = CamRot.Pitch;
		Mesh->SetWorldRotation(NewRot);
	}

	if (HeldObject && PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		FVector TargetLocation = FirstPersonCamera->GetComponentLocation() + FirstPersonCamera->GetForwardVector() * OrbitDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, HeldObject->GetActorRotation());
	}
}

void AForgeCharacter::Move(const FVector2D& Value)
{
	if (Controller == nullptr) return;

	FRotator ControlRot = Controller->GetControlRotation();
	if (HeldObject) ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;

	FVector Forward = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::X);
	FVector Right = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::Y);
	FVector DesiredMove = Forward * Value.Y + Right * Value.X;

	SphereComp->AddForce(DesiredMove * GetMoveSpeed());

	// Wall push correction
	// if (!DesiredMove.IsNearlyZero())
	// {
	// 	FHitResult Hit;
	// 	FCollisionQueryParams Params;
	// 	Params.AddIgnoredActor(this);
	// 	if (GetWorld()->SweepSingleByChannel(Hit, GetActorLocation(), GetActorLocation() + DesiredMove * 10.f,
	// 		FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(50.f), Params))
	// 	{
	// 		FVector PushBack = Hit.ImpactNormal * -WallPushDistance;
	// 		Mesh->AddForce(PushBack * GetMoveSpeed());
	// 	}
	// }
}

void AForgeCharacter::Hover(const float& Input)
{
	SphereComp->AddForce(FVector::UpVector * Input * GetMoveSpeed());
}

void AForgeCharacter::Look(const FInputActionValue& Value)
{
	if (!Controller) return;

	const FVector2D LookAxis = Value.Get<FVector2D>();
	if (!HeldObject)
	{
		FRotator Rot = Controller->GetControlRotation();
		Rot.Yaw += LookAxis.X;
		Rot.Pitch = FMath::Clamp(Rot.Pitch - LookAxis.Y, -89.f, 89.f);
		Controller->SetControlRotation(Rot);
		return;
	}
	if (HeldObject)
	{
		FVector ObjectLoc = HeldObject->GetActorLocation();
		FVector CharToObj = SphereComp->GetComponentLocation() - ObjectLoc;

		FRotator YawRot(0, LookAxis.X * 0.5f, 0); // scale rotation speed
		CharToObj = YawRot.RotateVector(CharToObj);

		FVector TargetPos = ObjectLoc + CharToObj;

		// Check walls
		// FHitResult Hit;
		// FCollisionQueryParams Params;
		// Params.AddIgnoredActor(this);
		// Params.AddIgnoredActor(HeldObject);
		// if (GetWorld()->SweepSingleByChannel(Hit, ObjectLoc, TargetPos, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(50.f), Params))
		// {
		// 	TargetPos = Hit.ImpactPoint - Hit.ImpactNormal * WallPushDistance;
		// }

		FVector SpringForce = (TargetPos - SphereComp->GetComponentLocation()) * 1500.f;
		SphereComp->AddForce(SpringForce);

		// Always face the object visually
		FRotator LookAtRot = (ObjectLoc - SphereComp->GetComponentLocation()).Rotation();
		Mesh->SetWorldRotation(FRotator(0.f, LookAtRot.Yaw, 0.f));

		// Physics handle target stays at object
		if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
		{
			FVector ObjectTarget = HeldObject->GetActorLocation();
			PhysicsHandle->SetTargetLocationAndRotation(ObjectTarget, HeldObject->GetActorRotation());
		}
	}
}

void AForgeCharacter::ObjectGrabDrop()
{
	if (HeldObject) DropObject();
	else if (InteractorComponent)
	{
		InteractorComponent->OnInteract();
	}
}
void AForgeCharacter::DeleteObject()
{
	if (HeldObject)
	{
		auto* Object = HeldObject;
		if (auto* GM = Cast<AForgeBuilderGameMode>(GetWorld()->GetAuthGameMode())) GM->RemoveForgeObject(Object);
		DropObject();
		Object->Destroy();
	}
	else if (InteractorComponent && InteractorComponent->HasInteractionObject())
	{
		if (auto* ForgeObject = Cast<AForgeObject>(InteractorComponent->GetCurrentInteractionObject()))
		{
			if (auto* GM = Cast<AForgeBuilderGameMode>(GetWorld()->GetAuthGameMode())) GM->RemoveForgeObject(ForgeObject);
			ForgeObject->Destroy();
		}
	}
}

void AForgeCharacter::PickupObject(AForgeObject* PickupActor)
{
	if (HeldObject) { UE_LOG(LogTemp, Error, TEXT("Already holding object in AForgeCharacter::PickupObject")); return; }
	if (!PickupActor) { UE_LOG(LogTemp, Error, TEXT("Invalid pickup actor in AForgeCharacter::PickupObject")); return; }
	
	HeldObject = PickupActor;
	HeldObjectRoot = Cast<UPrimitiveComponent>(HeldObject->GetRootComponent());
	if (!HeldObjectRoot) { HeldObject = nullptr; UE_LOG(LogTemp, Error, TEXT("Invalid heldobjectroot in AForgeCharacter::PickupObject")); return; }

	HeldObjectRoot->SetSimulatePhysics(true);
	HeldObjectRoot->SetEnableGravity(false);
	HeldObjectRoot->SetPhysicsLinearVelocity(FVector::ZeroVector);
	HeldObjectRoot->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	
	FVector OrbitTarget = ComputeOrbitTarget();
	SphereComp->SetWorldLocation(OrbitTarget);
	if (PhysicsHandle)
	{
		FVector GrabLocation = HeldObjectRoot->GetComponentLocation();
		PhysicsHandle->GrabComponentAtLocationWithRotation(HeldObjectRoot, NAME_None, GrabLocation, HeldObjectRoot->GetComponentRotation());
	}
	HeldObject->OnPickUp();
	if (auto* ForgeController = Cast<AForgePlayerController>(GetController())) { ForgeController->UpdateInputs(true); }
}

void AForgeCharacter::DropObject()
{
	if (!HeldObject) { UE_LOG(LogTemp, Error, TEXT("No held object to drop in AForgeCharacter::DropObject")); return; }

	if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}

	if (HeldObjectRoot)
	{
		HeldObjectRoot->SetEnableGravity(true);
	}

	HeldObject->OnDrop();
	if (auto* ForgeController = Cast<AForgePlayerController>(GetController()))
	{
		ForgeController->UpdateInputs(false, true);
	}
	HeldObject = nullptr;
	HeldObjectRoot = nullptr;
}

void AForgeCharacter::ApplyGridSnap()
{
	if (!HeldObject) return;

	FVector Location = HeldObject->GetActorLocation();

	Location.X = FMath::GridSnap(Location.X, GridSize);
	Location.Y = FMath::GridSnap(Location.Y, GridSize);
	Location.Z = FMath::GridSnap(Location.Z, GridSize);

	HeldObject->SetActorLocation(Location);
}

void AForgeCharacter::ApplySurfaceSnap()
{
	if (!HeldObject) return;

	FHitResult Hit;

	FVector Start = HeldObject->GetActorLocation();
	FVector End = Start - FVector(0,0,SurfaceSnapDistance);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(HeldObject);

	if (GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params))
	{
		FVector NewLocation = Hit.ImpactPoint;
		HeldObject->SetActorLocation(NewLocation);

		FRotator AlignRotation =
			FRotationMatrix::MakeFromZ(Hit.ImpactNormal).Rotator();

		HeldObject->SetActorRotation(AlignRotation);
	}
}

void AForgeCharacter::RotateObject(const FVector& AxisInput)
{
	if (!HeldObjectRoot) return;

	const FVector Torque = AxisInput * RotationSpeed * 10000.0f;
	HeldObjectRoot->AddTorqueInRadians(Torque, NAME_None, true);
}

FVector AForgeCharacter::ComputeOrbitTarget()
{
	if (!HeldObject) return SphereComp->GetComponentLocation() + FirstPersonCamera->GetForwardVector() * OrbitDistance;

	FVector ObjectLocation = HeldObject->GetActorLocation();
	FVector ToChar = (SphereComp->GetComponentLocation() - ObjectLocation).GetSafeNormal2D();

	// Snap to 8 orbit points
	float AngleStep = 45.f;
	float CurrentAngle = FMath::RadiansToDegrees(FMath::Atan2(ToChar.Y, ToChar.X));
	float SnappedAngle = FMath::RoundToFloat(CurrentAngle / AngleStep) * AngleStep;
	float Rad = FMath::DegreesToRadians(SnappedAngle);

	FVector OrbitOffset = FVector(FMath::Cos(Rad), FMath::Sin(Rad), 0.f) * OrbitDistance;
	FVector DesiredPos = ObjectLocation + OrbitOffset;

	// Trace for walls
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(HeldObject);

	if (GetWorld()->LineTraceSingleByChannel(Hit, ObjectLocation, DesiredPos, ECC_Visibility, Params))
	{
		DesiredPos = Hit.ImpactPoint - Hit.ImpactNormal * WallPushDistance;
	}

	return DesiredPos;
}