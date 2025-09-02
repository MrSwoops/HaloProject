// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCharacter.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AAICharacter : public AGameplayCharacter
{
	GENERATED_BODY()

public:
	void MoveToRandomPoint();
	
};
