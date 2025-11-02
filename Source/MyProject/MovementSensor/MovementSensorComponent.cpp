#include "MovementSensorComponent.h"

#include "MovementSensorWidget.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "MyProject/GameplayCharacter.h"
#include "MyProject/PlayerCharacter.h"

UMovementSensorComponent::UMovementSensorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
#define ECC_HurtBox ECC_GameTraceChannel3
	ObjectTypesToDetect.Add(UEngineTypes::ConvertToObjectType(ECC_HurtBox));
}

void UMovementSensorComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(
		ScanTimerHandle, 
		this, 
		&UMovementSensorComponent::ScanForMovement, 
		ScanRate, 
		true
	);
	// if (auto* PlayerOwner = Cast<APlayerCharacter>(GetOwner()))
	// {
	// 	//UMovementSensorWidget
	// 	SensorWidget = CreateWidget<UMovementSensorWidget>(Cast<APlayerController>(PlayerOwner->Controller), SensorWidgetClass);
	// 	if (SensorWidget)
	// 	{
	// 		SensorWidget->AddToViewport();
	// 		SensorWidget->MovementSensor = this;
	// 	}
	// 	//PlayerOwner->
	// }
	if (APlayerCharacter* PlayerOwner = Cast<APlayerCharacter>(GetOwner()))
	{
		if (APlayerController* PC = Cast<APlayerController>(PlayerOwner->GetController()))
		{
			if (SensorWidgetClass)
			{
				SensorWidget = CreateWidget<UMovementSensorWidget>(PC, SensorWidgetClass);
				if (SensorWidget)
				{
					SensorWidget->AddToViewport();
					SensorWidget->MovementSensor = this;
				}
			}
		}
	}
}

void UMovementSensorComponent::ScanForMovement()
{
	DetectedActors.Empty();
	FVector OwnerLoc = GetOwner()->GetActorLocation();
	//
	// // Find all pawns (you can narrow this to specific teams/classes)
	// TArray<AActor*> Pawns;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), Pawns);
	//
	// for (AActor* Pawn : Pawns)
	// {
	// 	if (Pawn == GetOwner()) continue;
	//
	// 	float Dist = FVector::Dist(OwnerLoc, Pawn->GetActorLocation());
	// 	if (Dist <= SensorRadius)
	// 	{
	// 		// Optionally, check for movement by comparing velocity
	// 		FVector Velocity = Pawn->GetVelocity();
	// 		if (!Velocity.IsNearlyZero())
	// 		{
	// 			FDetectedActorInfo Info;
	// 			Info.Actor = Pawn;
	// 			Info.Location = Pawn->GetActorLocation();
	// 			DetectedActors.Add(Info);
	// 		}
	// 	}
	// }
	
	// TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	// ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	// ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TSubclassOf<AActor> ClassFilter = AGameplayCharacter::StaticClass();
	TArray<AActor*> ActorsToIgnore;// { GetOwner() };
	//ActorsToIgnore.Add(GetOwner());
	TArray<AActor*> OverlappedActors;
	
	UKismetSystemLibrary::SphereOverlapActors(
		this,
		OwnerLoc,
		ScanRadius,
		ObjectTypesToDetect,
		ClassFilter,
		ActorsToIgnore,
		OverlappedActors
	);
	
	for (AActor* Actor : OverlappedActors)
	{
		FVector Velocity = Actor->GetVelocity();
		if (Velocity.Length() > MinSpeedDetection)
		{
			FDetectedActorInfo Info;
			Info.Actor = Cast<AGameplayCharacter>(Actor);
			Info.Location = Actor->GetActorLocation();
			DetectedActors.Add(Info);
		}
	}
	if (SensorWidget) SensorWidget->Repaint();
}