#include "BloodlustAwakening/Actors/Units/CppRangedUnit.h"

#include "BloodlustAwakening/Components/AttackComponent/Ranged/CppRangedAttackComponent.h"

ACppRangedUnit::ACppRangedUnit()
{
	PrimaryActorTick.bCanEverTick = true;

	AttackComponent = CreateDefaultSubobject<UCppRangedAttackComponent>("RangedAttackComponent");
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
