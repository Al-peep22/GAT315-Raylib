#pragma once
#include "raylib.h"

inline float GetRandomFloat()
{
    return GetRandomValue(0, 10000) / 10000.0f;
}

inline float GetRandomFloat(float max)
{
    return GetRandomFloat() * max;
}

inline float GetRandomFloat(float min, float max)
{
    return min + GetRandomFloat() * (max - min);
}