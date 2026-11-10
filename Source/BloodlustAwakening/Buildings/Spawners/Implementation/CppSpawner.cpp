#include "BloodlustAwakening/Buildings/Spawners/Implementation/CppSpawner.h"

ACppSpawner::ACppSpawner()
{
	bStaticBuilding = true;
	RestoreActorDefaults();
	EnableBoxBody(false);
}
