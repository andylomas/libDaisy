// myPotentiometer.cpp
#include "myUtils/myPotentiometer.h"

void MyPotentiometer::Init(const dsy_gpio_pin pin)
{
    pin_ = pin;
    mode_ = 0;
    sensitivity_ = 0.0f;
}

void MyPotentiometer::SetRange(const float min_val, const float max_val)
{
    min_val_ = min_val;
    range_ = max_val - min_val;
    mode_ = 1;
}


void MyPotentiometer::SetLogRange(const float min_val, const float max_val)
{
    k_ = logf(max_val / min_val);
    a_ = min_val;
    mode_ = 2;
}

void MyPotentiometer::SetLogZeroRange(const float min_val, const float max_val)
{
    // Modified version of log mapping where 0.5 still maps to the
    // geometric mean but 0.0 maps to 0.0
    k_ = 2.0f * logf(sqrtf(max_val / min_val) - 1.0f);
    a_ = max_val / (expf(k_) - 1.0f);
    mode_ = 3;
}

void MyPotentiometer::SetMap(const uint8_t num_items, const float* map_values)
{
    num_map_values_ = num_items;
    if (num_map_values_ > MAX_MAP_VALUES)
    {
        num_map_values_ = MAX_MAP_VALUES;
    }

    for (uint8_t i = 0; i < num_map_values_; i++)
    {
        map_values_[i] = map_values[i];
    }

    mode_ = 4;
}

void MyPotentiometer::Debounce() {
    float new_val = analogRead(pin_) / 1023.f;
    if (sensitivity_ == 0 || abs(new_val - val_) > sensitivity_)
    {
        val_ = new_val;
    }
}

float MyPotentiometer::MappedValue()
{
    switch (mode_)
    {
    case 0:
        // Unmapped
        return val_;

    case 1:
        // Linear  range
        return min_val_ + range_ * val_;

    case 2:
        // Log  range
        return a_ * expf(k_ * val_);

    case 3:
        // Log range with min_val mapped to 0, but maximum still max_val
        return a_ * (expf(k_ * val_) - 1.0f);

    case 4:
        // Mapped values
        uint8_t index = val_ * num_map_values_;
        if (index >= num_map_values_)
        {
            index = num_map_values_ - 1;
        }
        return map_values_[index];
    }
}
