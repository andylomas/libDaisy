#include "hid/parameter.h"
#include <math.h>

using namespace daisy;

void Parameter::Init(AnalogControl input, float min, float max, Curve curve)
{
    SetRange(min, max);
    pcurve_ = curve;
    in_     = input;
}

void Parameter::SetRange(float min, float max)
{
    pmin_   = min;
    pmax_   = max;
    lmin_   = logf(min < 0.0000001f ? 0.0000001f : min);
    lmax_   = logf(max);  
}

void Parameter::SetMapValues(const uint8_t num_items, const float* map_values)
{
    num_map_values_ = num_items;
    if (num_map_values_ > MAX_NUM_MAP_VALUES)
    {
        num_map_values_ = MAX_NUM_MAP_VALUES;
    }

    for (uint8_t i = 0; i < num_map_values_; i++)
    {
        map_values_[i] = map_values[i];
    }

    pcurve_ = MAP;
}

float Parameter::Process()
{
    uint8_t index;

    switch(pcurve_)
    {
        case LINEAR:
            val_ = (in_.Process() * (pmax_ - pmin_)) + pmin_;
            break;
        case EXPONENTIAL:
        case QUADRATIC:
            val_ = in_.Process();
            val_ = ((val_ * val_) * (pmax_ - pmin_)) + pmin_;
            break;
        case LOGARITHMIC:
            val_ = expf((in_.Process() * (lmax_ - lmin_)) + lmin_);
            break;
        case CUBE:
            val_ = in_.Process();
            val_ = ((val_ * (val_ * val_)) * (pmax_ - pmin_)) + pmin_;
            break;
        case MAP:
            val_ = in_.Process();
            index = val_ * num_map_values_;
            if (index >= num_map_values_)
            {
                index = num_map_values_ - 1;
            }
            val_ = map_values_[index];
            break;
        default:
            break;
    }
    return val_;
}