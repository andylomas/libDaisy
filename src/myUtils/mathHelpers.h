//mathHelpers.h
#ifndef MATHHELPERS_H
#define MATHHELPERS_H

#define safeMod(i, n) (i % n + n) % n
//#define PI_F 3.1415927f
#define PI_RECIP_F 0.3183098f
#define TAU_F 6.2831853f
#define TAU_RECIP_F 0.1591549f

float linMap(const float v, const float min, const float max)
{
  return v * (max - min) + min;
}

float logMap(const float v, const float min, const float max)
{
  return min * expf(logf(max/min) * v);
}

float discretise(const float f)
{
    return (f >= 1) ? round(f) : 1.0f / round(1.0f / f);
}

float bias(const float v , const float bias)
{
    return v / ((1.0f / bias - 2.0f) * (1.0f - v) + 1.0f);
}

float gain(const float v, const float gain)
{
    if(v < 0.5f)
        return 0.5f * bias(v * 2.0f, gain);
    else
        return 0.5f * bias(v * 2.0f - 1.0f, 1.0f - gain) + 0.5f;
}

#endif