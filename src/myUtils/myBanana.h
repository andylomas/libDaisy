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
    DIGITAL_INPUT,
    DIGITAL_OUTPUT,
    ANALOG_INPUT,
    ANALOG_OUTPUT,
    SERIAL_INPUT,
    SERIAL_OUTPUT,
    MIDI_INPUT,
    MIDI_OUTPUT,
    OFF
};

struct BananaConfig {
    BananaMode mode = OFF;
    uint32_t baud_rate = 115200;
};

struct MyBanana
{
    AnalogControl analog_input;
    dsy_gpio gpio;
    BananaConfig config;
};

} // namespace daisy

#endif
