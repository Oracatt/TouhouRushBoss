#include "Mathf.h"

float GetAngle(const Vector2& vec)
{
	return atan2(vec.y, vec.x);
}

Vector2 GetVector(float rad, float length)
{
	return Vector2(cos(rad), sin(rad)) * length;
}

float AngleSubAngle(float a1, float a2)
{
	float _a1 = fmod(a1, M_PI * 2);
	float _a2 = fmod(a2, M_PI * 2);
	float s = fmax(_a1, _a2) - fmin(_a1, _a2);
	if (s < M_PI)
		return s;
	return M_PI * 2 - s;
}

bool IsOutOfRange(const Vector2& loc, float tolerance)
{
	if (loc.x < -320 - tolerance)
		return true;
	if (loc.x > 320 + tolerance)
		return true;
	if (loc.y < -240 - tolerance)
		return true;
	if (loc.y > 240 + tolerance)
		return true;
	return false;
}