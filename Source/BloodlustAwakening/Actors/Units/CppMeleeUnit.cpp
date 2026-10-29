#include "BloodlustAwakening/Actors/Units/CppMeleeUnit.h"

#include "BloodlustAwakening/Components/AttackComponent/Melee/CppMeleeAttackComponent.h"

ACppMeleeUnit::ACppMeleeUnit()
{
	PrimaryActorTick.bCanEverTick = true;

	AttackComponent = CreateDefaultSubobject<UCppMeleeAttackComponent>("MeleeAttackComponent");
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
