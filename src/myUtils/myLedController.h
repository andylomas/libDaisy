// myLedController.h
#pragma once
#ifndef DSY_MYLEDCONTROLLER_H
#define DSY_MYLEDCONTROLLER_H

// Class for up to 8 RGB LEDs controlled using three shift registers
// Data is communicated to the shift registers using shiftOut()

#include "per/gpio.h"
#include "myUtils/shiftOut.h"

class MyLedController
{
public:
    void Init(const dsy_gpio_pin pin_data, const dsy_gpio_pin pin_clock, const dsy_gpio_pin pin_led_select);
    void Set(uint8_t n, const bool r, const bool g, const bool b);
    void Set(uint8_t n, const bool v);
    void SetValue(uint8_t n, const int v);
    void Clear();
    void Update(bool force_update = false);

private:
    dsy_gpio pin_data_;
    dsy_gpio pin_clock_;
    dsy_gpio pin_led_select_;
    uint8_t r_data_;
    uint8_t g_data_;
    uint8_t b_data_;
    uint32_t prev_data_;
};

#endif
