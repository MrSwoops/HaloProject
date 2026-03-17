#include "ForgeCharacter.h"

#include "ForgePlayerController.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "MyProject/Forge/ForgeObject/ForgeObject.h"
#include "MyProject/Interactions/InteractorComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

AForgeCharacter::AForgeCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterMesh"));
	SetRootComponent(Mesh);

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(Mesh);
	FirstPersonCamera->bUsePawnControlRotation = true;

	InteractorComponent = CreateDefaultSubobject<UInteractorComponent>(TEXT("InteractorComponent"));
	InteractorComponent->SetupAttachment(FirstPersonCamera);

	GrabConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("GrabConstraint"));
	GrabConstraint->SetupAttachment(RootComponent);
}

void AForgeCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HeldObject)
	{
		FVector ObjectLocation = HeldObject->GetActorLocation();
		FVector PawnLocation = GetActorLocation();

		FVector Offset = PawnLocation - ObjectLocation;
		Offset = Offset.GetSafeNormal() * OrbitDistance;

		SetActorLocation(ObjectLocation + Offset);

		FRotator LookRotation = (ObjectLocation - GetActorLocation()).Rotation();
		Mesh->SetWorldRotation(LookRotation);

		if (bSurfaceSnap) ApplySurfaceSnap();
		if (bGridSnap) ApplyGridSnap();
	}
}

void AForgeCharacter::Move(const FVector2D& Value)
{
	if (Controller == nullptr) return;

	if (HeldObjectRoot)
	{
		FRotator ControlRotation = Controller->GetControlRotation();
		ControlRotation.Pitch = 0;
		ControlRotation.Roll = 0.0f;

		FVector ForwardDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);
		FVector Force = (ForwardDirection * Value.Y * GetMoveSpeed()) + (RightDirection * Value.X * GetMoveSpeed());
		HeldObjectRoot->AddForce(Force, NAME_None, true);
		GrabConstraint->SetWorldLocation(GetActorLocation() + GetActorForwardVector() * OrbitDistance);
	}
	else
	{
		FRotator ControlRotation = Controller->GetControlRotation();
		ControlRotation.Roll = 0.0f;

		FVector ForwardDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);

		Mesh->AddForce(ForwardDirection * Value.Y * GetMoveSpeed());
		Mesh->AddForce(RightDirection * Value.X * GetMoveSpeed());
	}
}

void AForgeCharacter::Hover(const float& Input)
{
	if (HeldObjectRoot) HeldObjectRoot->AddForce(FVector::UpVector * Input * MoveSpeed);
	else Mesh->AddForce(FVector::UpVector * Input * MoveSpeed);
}

void AForgeCharacter::Look(const FInputActionValue& Value)
{
	if (Controller == nullptr) return;

	const FVector2D LookAxis = Value.Get<FVector2D>();
	if (!HeldObject)
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const float NewYaw = ControlRotation.Yaw + LookAxis.X;
		float NewPitch = ControlRotation.Pitch - LookAxis.Y;
		NewPitch = FMath::Clamp(NewPitch, -89.0f, 89.0f);
		FRotator NewRotation = FRotator(NewPitch, NewYaw, 0.0f);
		Controller->SetControlRotation(NewRotation);
		Mesh->SetWorldRotation(NewRotation);
		return;
	}
	const FVector ObjectLocation = HeldObject->GetActorLocation();
	const FVector PawnLocation = GetActorLocation();

	FVector Offset = PawnLocation - ObjectLocation;
	const FRotator Rotation(0, LookAxis.X, 0);
	Offset = Rotation.RotateVector(Offset);
	const FVector NewLocation = ObjectLocation + Offset;

	SetActorLocation(NewLocation);
	const FRotator LookRotation = (ObjectLocation - NewLocation).Rotation();
	SetActorRotation(LookRotation);
}

void AForgeCharacter::UpdateObjectCheck()
{
	if (HeldObject) return;

	//UGameplayStatics::
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
		DropObject();
		Object->Destroy();
	}
	else if (InteractorComponent && InteractorComponent->HasInteractionObject())
	{
		if (auto* ForgeObject = Cast<AForgeObject>(InteractorComponent->GetCurrentInteractionObject()))
		{
			ForgeObject->Destroy();
		}
	}
}

void AForgeCharacter::PickupObject(AForgeObject* PickupActor)
{
	if (HeldObject || !PickupActor || !PickupActor->GetRootComponent()) return;
	HeldObject = PickupActor;
	HeldObjectRoot = Cast<UPrimitiveComponent>(HeldObject->GetRootComponent());

	HeldObjectRoot->SetSimulatePhysics(true);
	HeldObjectRoot->SetEnableGravity(false);
	HeldObjectRoot->SetLinearDamping(2.0f);
	HeldObjectRoot->SetAngularDamping(5.0f);
	
	GrabConstraint->SetConstrainedComponents(Mesh, NAME_None, HeldObjectRoot, NAME_None);
	GrabConstraint->SetDisableCollision(true);
	GrabConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Limited, 0);
	GrabConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Limited, 0);
	GrabConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 0);
	GrabConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0);
	GrabConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0);
	GrabConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0);
	
	if (auto* ForgeController = Cast<AForgePlayerController>(GetController()))
	{
		ForgeController->UpdateInputs(true);
	}
	HeldObject->OnPickUp();
}

void AForgeCharacter::DropObject()
{
	if (!HeldObject) return;

	if (HeldObjectRoot)
	{
		HeldObjectRoot->SetEnableGravity(true);
	}
	GrabConstraint->BreakConstraint();
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