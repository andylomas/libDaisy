// adafruitPwmLedController.h
#pragma once
#ifndef DSY_ADAFRUIT24CHLEDCONTROLLER_H
#define DSY_ADAFRUIT24CHLEDCONTROLLER_H

// Class for controlling LEDs using Adafruit 24 Channel 12 bit PWM LED Controller

// Based on Arduino library: https://github.com/adafruit/Adafruit_TLC5947

#include <math.h>
#include "per/gpio.h"
#include "daisy_core.h"
 
class Adafruit24ChLedController
{
public:
    Adafruit24ChLedController();
    void Init(const dsy_gpio_pin pin_data, const dsy_gpio_pin pin_clock, const dsy_gpio_pin pin_latch);
    void SetGamma(const float gamma);
    void SetGlobalBrightness(const float r, const float g, const float b);
    void SetRaw(uint8_t n, const uint16_t r, const uint16_t g, const uint16_t b);
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
    dsy_gpio pin_latch_;
    float gamma_;
    float global_brightness_r_;
    float global_brightness_g_;
    float global_brightness_b_;
    uint16_t led_max_value_r_;
    uint16_t led_max_value_g_;
    uint16_t led_max_value_b_;
    uint16_t pwm_buffer_[24];
};

#endif
