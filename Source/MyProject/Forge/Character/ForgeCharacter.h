#pragma once

#include "CoreMinimal.h"
#include "ForgeCharacter.generated.h"

class UPhysicsConstraintComponent;
class UInteractorComponent;
class AForgeObject;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class AForgeCharacter : public APawn
{
	GENERATED_BODY()
public:
	AForgeCharacter();
	
	void Move(const FVector2D& Value);
	void StartSprint() { bIsSprinting = true; }
	void StopSprint() { bIsSprinting = false; }
	void Hover(const float& Input);
	void Look(const FInputActionValue& Value);
	void ObjectGrabDrop();
	void DeleteObject();

	void RotateObject(const FVector& AxisInput);
	void ToggleGridSnap() { bGridSnap = !bGridSnap; }
	void ToggleSurfaceSnap() { bSurfaceSnap = !bSurfaceSnap; }

	UPROPERTY(EditDefaultsOnly)
	float MoveSpeed = 23000.0f;
	UPROPERTY(EditDefaultsOnly)
	float SprintModifier = 1.6f;

private:
	bool bIsSprinting = false;
public:
	bool IsSprinting() const { return bIsSprinting; }
	float GetMoveSpeed() const { return (bIsSprinting ? MoveSpeed * SprintModifier : MoveSpeed); }

	UStaticMeshComponent* GetCharacterMesh() const { return Mesh; }
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	UCameraComponent* FirstPersonCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta=(AllowPrivateAccess="true"))
	UInteractorComponent* InteractorComponent;
	UPROPERTY()
	UPhysicsConstraintComponent* GrabConstraint;


public:
	bool HoldingObject() const { return HeldObject != nullptr; }
	void PickupObject(AForgeObject* PickupActor);
	void DropObject();
protected:
	UPROPERTY(EditDefaultsOnly)
	float OrbitDistance = 350.0f;
	UPROPERTY(EditDefaultsOnly)
	float GridSize = 50.0f;
	UPROPERTY(EditDefaultsOnly)
	float RotationSpeed = 100.0f;
	UPROPERTY(EditDefaultsOnly)
	float SurfaceSnapDistance = 100.0f;
	bool bGridSnap = true;
	bool bSurfaceSnap = true;
	void ApplyGridSnap();
	void ApplySurfaceSnap();
	
	UPROPERTY()
	UPrimitiveComponent* HeldObjectRoot;
	UPROPERTY()
	AForgeObject* HeldObject;
	void UpdateObjectCheck();



protected:
	virtual void Tick(float DeltaSeconds) override;
};
