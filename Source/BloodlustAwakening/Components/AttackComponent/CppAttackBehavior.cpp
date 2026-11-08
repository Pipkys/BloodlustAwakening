#include "BloodlustAwakening/Components/AttackComponent/CppAttackBehavior.h"

#include "BloodlustAwakening/Components/AttackComponent/CppAttackParams.h"

#include "NiagaraFunctionLibrary.h"

void UCppAttackBehavior::SetupBehavior(AActor* InOwner, const TInstancedStruct<FCppAttackParams>& InParams)
{
	Owner = InOwner;
	Params = InParams.GetPtr<FCppAttackParams>();
}

float UCppAttackBehavior::GetRange() const
{
	return Params->Range;
}

void UCppAttackBehavior::SpawnNiagaraEffect(UNiagaraSystem* InEffect, const FTransform& InTransform) const
{
	if (IsValid(InEffect))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
			InEffect,
			InTransform.GetLocation(),
			InTransform.GetRotation().Rotator(),
			InTransform.GetScale3D());
	}
}
