// mySwitch.h
#pragma once
#ifndef DSY_MYSWITCH_H
#define DSY_MYSWITCH_H

//#include "daisy_core.h"
#include "per/gpio.h"
#include "myUtils/myRgbLed.h"

class MySwitch
{
public:
    void Init(const dsy_gpio_pin pin, const float update_rate, const bool invert);
    inline void SetMaxCount(const int16_t m) { max_count_ = m; }
    inline void CountOnRising() { count_on_rising_ = true; count_on_falling_ = false; }
    inline void CountOnFalling() { count_on_rising_ = false; count_on_falling_ = true; }
    inline void CountOnRisingAndFalling() { count_on_rising_ = true; count_on_falling_ = true; }
    virtual void LinkLed( MyRgbLed *linked_led );
    virtual void Debounce();
    void SetCount( const int16_t count );
    virtual void Reset();
    inline int16_t Count() { return count_; }
    inline bool Pressed() { return state_ == 0xff; }
    inline uint8_t State() { return state_; }
    inline bool RisingEdge() { return state_ == 0x7f; }
    inline bool FallingEdge() { return state_ == 0x80; }
    inline bool CountChanged() { return count_changed_; }
    inline bool StateChanged() { return state_ == 0x7f || state_ == 0x80; }
    inline bool On() { return state_ == 0xff; }
    inline bool Off() { return state_ == 0x00; }
    inline float TimeHeldMs() { return On() ? time_held_ * 1000.f : 0; }
    
private:
    bool flip_;
    float time_per_update_, time_held_;
    dsy_gpio pin_;
    uint8_t state_;
    int16_t count_;
    int16_t max_count_;
    bool count_changed_;
    bool count_on_rising_;
    bool count_on_falling_;
    MyRgbLed *switch_linked_led_;
};

#endif
