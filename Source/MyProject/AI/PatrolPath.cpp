#include "PatrolPath.h"

#include "Components/SplineComponent.h"

APatrolPath::APatrolPath()
{
	SplinePath = CreateDefaultSubobject<USplineComponent>(TEXT("SplinePath"));
	SplinePath->SetupAttachment(RootComponent);
}

TArray<FVector> APatrolPath::GetPathPoints()
{
	TArray<FVector> Points;

	if (SplinePath)
	{
		const int32 NumPoints = SplinePath->GetNumberOfSplinePoints();
		for (int32 i = 0; i < NumPoints; ++i)
		{
			Points.Add(SplinePath->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
		}
	}
	return Points;
}