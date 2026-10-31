#include "BloodlustAwakening/Units/Implementation/CppRangedUnit.h"

ACppRangedUnit::ACppRangedUnit()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACppRangedUnit::BeginPlay()
{
	Super::BeginPlay();
}

void ACppRangedUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACppRangedUnit::Setup(const FCppUnitSpawnParameters& InSpawnParameters)
{
	Super::Setup(InSpawnParameters);
}
