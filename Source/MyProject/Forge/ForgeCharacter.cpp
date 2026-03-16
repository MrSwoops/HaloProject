#include "ForgeCharacter.h"

#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

AForgeCharacter::AForgeCharacter()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterMesh"));
	SetRootComponent(Mesh);

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(Mesh);
	FirstPersonCamera->bUsePawnControlRotation = true;

}

void AForgeCharacter::Move(const FVector2D& Value)
{
	UE_LOG(LogTemp, Error, TEXT("Forge C Move"));
	if (Controller == nullptr) return;

	FRotator ControlRotation = Controller->GetControlRotation();
	//ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	FVector ForwardDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);  // Forward (X)
	FVector RightDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);    // Right (Y)

	//if (auto* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance())) AnimInstance->OnMovement(Value);
	Mesh->AddForce(ForwardDirection * Value.Y * MoveSpeed);
	Mesh->AddForce(RightDirection * Value.X * MoveSpeed);
	//AddMovementInput(ForwardDirection, Value.Y);
	//AddMovementInput(RightDirection, Value.X);
}

void AForgeCharacter::Hover(const float& Input)
{
	Mesh->AddForce(FVector::UpVector * Input * MoveSpeed);
	//AddMovementInput(FVector::UpVector, Input);
}

void AForgeCharacter::Look(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Error, TEXT("Forge C look"));
	if (Controller == nullptr) return;
	
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// Create Look rotation
	FRotator ControlRotation = Controller->GetControlRotation();
	float NewYaw = ControlRotation.Yaw + LookAxisVector.X;
	float NewPitch = ControlRotation.Pitch - LookAxisVector.Y;
	NewPitch = FMath::Clamp(NewPitch, -89.0f, 89.0f);

	Controller->SetControlRotation(FRotator(NewPitch, NewYaw, 0.0f));

}

void AForgeCharacter::UpdateObjectCheck()
{
	if (HeldObject) return;

	//UGameplayStatics::
}
