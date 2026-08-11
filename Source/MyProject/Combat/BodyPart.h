#pragma once

#include "CoreMinimal.h"
#include "BodyPart.generated.h"

UENUM(BlueprintType)
enum class EBodyPart : uint8
{
	Undefined,
	Head,
	Torso,
	UpperArm,
	LowerArm,
	UpperLeg,
	LowerLeg,
};