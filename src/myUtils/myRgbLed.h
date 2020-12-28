// myRgbLed.h
#pragma once
#ifndef DSY_MYRGBLED_H
#define DSY_MYRGBLED_H

//#include "daisy_core.h"
#include "per/gpio.h"
#include "myUtils/myLedController.h"

struct RgbLedColor
{
    bool r, g, b;
};

class MyRgbLed
{
public:
    void Init(const dsy_gpio_pin pin_r, const dsy_gpio_pin pin_g, const dsy_gpio_pin pin_b);
    void Init(MyLedController *led_controller, uint8_t n);
    virtual void Set(const bool r, const bool g, const bool b);
    virtual void Set(const bool v);
    virtual void SetValue(const int v);
    void SetFloat(const float r, const float g, const float b);
    void SetTable(int size, RgbLedColor *table);
    virtual void Update();

protected:
    dsy_gpio pin_r_, pin_g_, pin_b_;
    bool r_main_, g_main_, b_main_;
    float r_inc_, g_inc_, b_inc_;
    float r_val_, g_val_, b_val_;
    RgbLedColor *table_;
    int table_size_;
    bool float_mode_;
    MyLedController *led_controller_ = NULL;
    uint8_t led_index_;
};

#endif
