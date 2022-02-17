#include "Rand.h"
#include "VirtualLib.h"

unique_ptr<default_random_engine> randomEngine(new default_random_engine());

void SetRandomSeed(unsigned int seed)
{
	randomEngine.reset(new default_random_engine(seed));
}

float Random(float _min, float _max)
{
	return uniform_real_distribution<float>(_min, _max)(*randomEngine.get());
}

int Random(int _min, int _max)
{
	return uniform_int_distribution<int>(_min, _max)(*randomEngine.get());
}

float RandomAngle()
{
	return Random(0.0f, (float)M_PI * 2.0f);
}

bool RandomChance(float chance)
{
	return Random(0, 1) <= chance;
}