// myEncoder.h
#pragma once
#ifndef DSY_MYENCODER_H
#define DSY_MYENCODER_H

//#include "daisy_core.h"
#include "per/gpio.h"
#include "myUtils/mySwitch.h"

class MyEncoder : public MySwitch
{
public:
    void Init(
        const dsy_gpio_pin pinA, const dsy_gpio_pin pinB, const dsy_gpio_pin pin_click,
        const float update_rate, const int16_t min_val=-32768, const int16_t max_val=32767);
    void SetRange(const int16_t min_val, const int16_t max_val);
    inline void SetValueOnRising() { value_on_rising_ = true; value_on_falling_ = false; }
    inline void SetValueOnFalling() { value_on_rising_ = false; value_on_falling_ = true; }
    inline void SetValueOnRisingAndFalling() { value_on_rising_ = true; value_on_falling_ = true; }
    virtual void LinkLed( MyRgbLed *linked_led );
    inline void ResetOnPressed(bool v = true) { reset_on_pressed_ = v; }
    inline void WrapMode(bool mode) {wrap_mode_ = mode; }
    void SetValue(const int16_t val);
    virtual void Debounce();
    virtual void Reset();
    inline int32_t Increment() { return inc_; }
    inline int32_t Value() { return val_; }
    inline bool ValueChanged() { return inc_ != 0; }
    inline int Transitions() { return transitions_; }
    inline int CurState() { return transitions_ & 0b11; }

private:    
    dsy_gpio pinA_, pinB_;
    uint8_t prev_state_;
    uint8_t transitions_;
    int16_t inc_;
    int16_t val_;
    int16_t min_val_;
    int16_t max_val_;
    bool value_on_rising_;
    bool value_on_falling_;
    bool reset_on_pressed_;
    bool wrap_mode_;
    MyRgbLed *encoder_linked_led_;
};

#endif
