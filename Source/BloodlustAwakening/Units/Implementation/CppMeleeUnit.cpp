#include "BloodlustAwakening/Units/Implementation/CppMeleeUnit.h"

ACppMeleeUnit::ACppMeleeUnit()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACppMeleeUnit::BeginPlay()
{
	Super::BeginPlay();
}

void ACppMeleeUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACppMeleeUnit::Setup(const FCppUnitSpawnParameters& InSpawnParameters)
{
	Super::Setup(InSpawnParameters);
}
