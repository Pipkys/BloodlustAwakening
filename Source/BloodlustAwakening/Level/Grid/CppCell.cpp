#include "BloodlustAwakening/Level/Grid/CppCell.h"

#include "BloodlustAwakening/Level/Grid/CppCellGrid.h"

ACppCellGrid* ACppCell::GetGrid()
{
	return (ACppCellGrid*)Owner;
}
