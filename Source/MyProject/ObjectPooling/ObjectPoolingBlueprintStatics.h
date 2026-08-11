// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ObjectPoolingBlueprintStatics.generated.h"

class AGrenade;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UObjectPoolingBlueprintStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category="Object Pools")
	static AGrenade* RetrieveGrenade(UObject* ReferenceObject, const FVector& Location, const FRotator& Rotation, const int32 NadeType);
};
