#include "Flag.h"

void AFlag::BeginPlay()
{
	Super::BeginPlay();

	FlagPoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagPoleMesh"));
}
