#include "BloodlustAwakening/Core/SpawnParameters/CppActorSpawnParameters.h"

FActorSpawnParameters FCppActorSpawnParameters::ConvertToEngineType() const
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Owner;
	SpawnParameters.Instigator = Instigator;
	SpawnParameters.SpawnCollisionHandlingOverride = SpawnCollisionHandlingOverride;
	SpawnParameters.TransformScaleMethod = TransformScaleMethod;
	return SpawnParameters;
}
