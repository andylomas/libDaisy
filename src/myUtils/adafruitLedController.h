// adafruitPwmLedController.h
#pragma once
#ifndef DSY_ADAFRUITLEDCONTROLLER_H
#define DSY_ADAFRUITLEDCONTROLLER_H

// Class for controlling LEDs using Adafruit 12 Channel
// 16 bit PWM LED Controller

#include <math.h>
#include "per/gpio.h"
#include "daisy_core.h"

#define ADALED_DATA_BUFFER_SIZE 28
 
class AdafruitLedController
{
public:
    AdafruitLedController();
    void Init(const dsy_gpio_pin pin_data, const dsy_gpio_pin pin_clock);
    void Init(const dsy_gpio_pin pin_data, const dsy_gpio_pin pin_clock, const dsy_gpio_pin pin_cs);
    void InitSpi();
    void InitSpi(const dsy_gpio_pin pin_cs);
    void SetDataBufferBits(uint8_t v, uint8_t num_bits, uint8_t pos);
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
    dsy_gpio pin_cs_;
    float gamma_;
    bool use_spi_;
    bool use_cs_;
    uint8_t data_buffer_[ADALED_DATA_BUFFER_SIZE];
};

#endif
