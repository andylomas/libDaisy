#pragma once
#ifndef DSY_MYBANANA_H
#define DSY_MYBANANA_H

#include "per/gpio.h"
#include "hid/ctrl.h"
#include "myUtils/mySwitch.h"

// Note: digital outputs will be set to push-pull, and inputs to no pull.

namespace daisy
{
enum BananaMode {
    DIGITAL_INPUT = 1,
    DIGITAL_OUTPUT = 2,
    ANALOG_INPUT = 4,
    ANALOG_OUTPUT = 8,
    SERIAL_INPUT = 16,
    SERIAL_OUTPUT = 32,
    OFF = 0
};

struct BananaConfig {
    BananaMode mode = OFF;
    dsy_gpio_pull pull = DSY_GPIO_NOPULL;
    //uint32_t baud_rate = 115200;
    uint32_t baud_rate = 6250000; // Highest baud rate available
};

class MyBanana
{
public:
    float AnalogInput();
    void AnalogOutput(float val);
    bool DigitalInput();
    void DigitalOutput(bool state);

    AnalogControl analog_control;
    dsy_gpio gpio;
    BananaConfig config;
    uint8_t dac_chn;
};

} // namespace daisy

#endif
