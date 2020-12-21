// myPotentiometer.h
#pragma once
#ifndef DSY_MYPOTENTIOMETER_H
#define DSY_MYPOTENTIOMETER_H

#define MAX_MAP_VALUES 32

//#include "daisy_core.h"
#include "hid/ctrl.h"

class MyPotentiometer
{
public:
    void Init(const dsy_gpio_pin pin);
    void SetRange(const float min_val, const float max_val);
    void SetLogRange(const float min_val, const float max_val);
    void SetLogZeroRange(const float min_val, const float max_val);
    void SetMap(const uint8_t num_items, const float* map_values);
    void SetSensitivity(const float sensitivity) { sensitivity_ = sensitivity; }
    void Debounce();
    inline float Value() { return val_; }
    float MappedValue();
    
private:
    dsy_gpio pin_;
    float val_;
    float min_val_;
    float range_;
    float a_, k_;
    float map_values_[MAX_MAP_VALUES];
    uint8_t num_map_values_;
    uint8_t mode_;
    float sensitivity_;
};

#endif
