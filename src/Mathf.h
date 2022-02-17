#pragma once
#include "VirtualLib.h"

template <class T>
T Max(const T& v1, const T& v2)
{
	return v1 > v2 ? v1 : v2;
}

template <class T>
T Min(const T& v1, const T& v2)
{
	return v1 < v2 ? v1 : v2;
}

template <class T>
T Lerp(const T& min, const T& max, float value)
{
	return min + (max - min) * value;
}

template <class T,class U>
float Vector2Distance(const T& v1, const U& v2)
{
	return (Vector2(v1) - Vector2(v2)).GetLength();
}

float GetAngle(const Vector2& vec);

Vector2 GetVector(float rad, float length = 1);

float AngleSubAngle(float a1, float a2);

bool IsOutOfRange(const Vector2& loc, float tolerance);