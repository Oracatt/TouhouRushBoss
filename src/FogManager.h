#pragma once
#include "VirtualLib.h"

extern Color fogColor;
extern bool fogEnable;
extern float fogStart;
extern float fogRange;

void SetFogEnabled(bool enable);

void SetFogColor(const Color& color);

void SetFogStart(float start);

void SetFogRange(float range);