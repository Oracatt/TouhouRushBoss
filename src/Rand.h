#pragma once
#include <random>
#include <memory>

using namespace std;

extern unique_ptr<default_random_engine> randomEngine;

void SetRandomSeed(unsigned int seed);

float Random(float _min, float _max);

int Random(int _min, int _max);

float RandomAngle();

bool RandomChance(float chance);

template <class T>
void RandomSort(vector<T>& _array)
{
	shuffle(_array.begin(), _array.end(), *randomEngine.get());
}