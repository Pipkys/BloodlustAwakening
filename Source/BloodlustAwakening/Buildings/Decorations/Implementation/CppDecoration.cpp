#include "BloodlustAwakening/Buildings/Decorations/Implementation/CppDecoration.h"

ACppDecoration::ACppDecoration()
{
	bStaticBuilding = true;
	RestoreActorDefaults();
	EnableBoxBody(true);
}
