#include "VirtualLib.h"

Color fogColor = { 0.5,0.5,0.5,1 };
bool fogEnable = false;
float fogStart = 10;
float fogRange = 100;

void SetFogEnabled(bool enable)
{
	fogEnable = enable;
}

void SetFogColor(const Color& color)
{
	fogColor = color;
}

void SetFogStart(float start)
{
	fogStart = start;
}

void SetFogRange(float range)
{
	fogRange = range;
}