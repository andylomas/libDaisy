// adafruitPwmLedController.h
#pragma once
#ifndef DSY_MYLEDCONTROLLER_H
#define DSY_MYLEDCONTROLLER_H

// Class for controlling LEDs using Adafruit 12 Channel
// 16 bit PWM LED Controller

#include <math.h>
#include "per/gpio.h"
#include "myUtils/shiftOut.h"

class AdafruitPwmLedController
{
public:
    void Init(const dsy_gpio_pin pin_data, const dsy_gpio_pin pin_clock);
    void SetGamma(const float gamma);
    void SetGlobalBrightness(const float r, const float g, const float b);
    void Set(uint8_t n, const bool r, const bool g, const bool b);
    void Set(uint8_t n, const bool v);
    void SetValue(uint8_t n, const int v);
    void SetFloat(uint8_t n, float r, float g, float b);
    void SetFloat(uint8_t n, float v);
    void Clear();
    void Update();

private:
    dsy_gpio pin_data_;
    dsy_gpio pin_clock_;
    uint8_t global_brightness_r_;
    uint8_t global_brightness_g_;
    uint8_t global_brightness_b_;
    uint16_t led_brightness_r_[4];
    uint16_t led_brightness_g_[4];
    uint16_t led_brightness_b_[4];
    float gamma_;
};

#endif
