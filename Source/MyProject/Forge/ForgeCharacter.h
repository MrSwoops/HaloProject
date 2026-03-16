#pragma once

#include "CoreMinimal.h"
#include "ForgeCharacter.generated.h"

class UCameraComponent;
struct FInputActionValue;

UCLASS()
class AForgeCharacter : public APawn
{
	GENERATED_BODY()
public:
	AForgeCharacter();
	
	void Move(const FVector2D& Value);
	void Hover(const float& Input);
	void Look(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly)
	float MoveSpeed = 200.0f;

	UStaticMeshComponent& GetCharacterMesh() const { return *Mesh; }
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	UCameraComponent* FirstPersonCamera;

	UPROPERTY()
	AActor* HeldObject;
	void UpdateObjectCheck();
};
