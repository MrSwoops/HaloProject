#include "BTD_TargetHasLOSDestination.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "MyProject/AI/AICharacter.h"
#include "MyProject/AI/AICharacterController.h"

FString UBTD_TargetHasLOSDestination::GetStaticDescription() const
{
	FString Desc;

	Desc += FString::Printf(TEXT("Target: %s\n"), *InTargetKey.SelectedKeyName.ToString());
	Desc += FString::Printf(TEXT("Destination: %s\n"), *InDestinationKey.SelectedKeyName.ToString());

	return Desc;
}

bool UBTD_TargetHasLOSDestination::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAICharacterController* AICon = Cast<AAICharacterController>(OwnerComp.GetAIOwner());
	if (!AICon) return false;
	
	AAICharacter* AICharacter = Cast<AAICharacter>(AICon->GetPawn());
	if (!AICharacter) return false;

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetKey.SelectedKeyName));
	if (!TargetActor) return false;

	FVector TargetDestination = OwnerComp.GetBlackboardComponent()->GetValueAsVector(InDestinationKey.SelectedKeyName);
	
	FVector LineTraceStartLocation;
	if (auto* GC = Cast<AGameplayCharacter>(TargetActor))
	{
		LineTraceStartLocation = GC->VisionPoint->GetComponentLocation();
	}
	else LineTraceStartLocation = TargetActor->GetActorLocation();

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(TargetActor);
	CollisionParams.AddIgnoredActor(AICharacter);
	FCollisionObjectQueryParams CollisionObjectParams;
// #define ECC_Hurtbox ECC_GameTraceChannel4
// 	CollisionObjectParams.AddObjectTypesToQuery(ECC_Hurtbox);
	CollisionObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
	
	bool bHit = GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		LineTraceStartLocation,
		TargetDestination,
		CollisionObjectParams,
		CollisionParams
	);
	//DrawDebugLine(GetWorld(), Start, End, (!bHit) ? FColor::Green : FColor::Yellow, false, 5.f, 0, 1.f);
	return !bHit;
}