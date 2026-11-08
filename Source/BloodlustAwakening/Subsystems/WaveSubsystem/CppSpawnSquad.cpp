#include "CppSpawnSquad.h"

float FCppSpawnSquad::GetWeight(int32 InWaveNumber, float InDifficulty) const
{
	if (InWaveNumber < MinWave || InWaveNumber > MaxWave)
	{
		return 0.0f;
	}

	float Weight = BaseWeight * (1.0f + InDifficulty * 0.6f);
	Weight *= (1.0f + (InWaveNumber - 1) * 0.12f);
	return FMath::Max(Weight, 0.0f);
}
