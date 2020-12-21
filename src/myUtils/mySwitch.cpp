// mySwitch.cpp
#include "myUtils/mySwitch.h"

void MySwitch::Init(const dsy_gpio_pin pin, const float update_rate, const bool invert)
{
    pin_.pin = pin;
    pin_.mode = DSY_GPIO_MODE_INPUT;
    pin_.pull = DSY_GPIO_PULLUP;
    dsy_gpio_init(&pin_);

    flip_ = invert;
    time_per_update_ = 1.f / update_rate;
    state_ = 0;
    time_held_ = 0.f;

    count_ = 0;
    max_count_ = 0x7FFF;
    count_on_rising_ = true;
    count_on_falling_ = false;
    switch_linked_led_ = NULL;
}

void MySwitch::Debounce()
{
    uint8_t in = dsy_gpio_read(&pin_);
    state_ = (state_ << 1) | (flip_ ? !in : in);
    
    if (state_ == 0x7f || state_ == 0x80)
        time_held_ = 0;
    
    if (state_ == 0xff)
        time_held_ += time_per_update_;

    if ((count_on_rising_ && state_ == 0x7f) || (count_on_falling_ && state_ == 0x80))
    {
        count_++;
        count_changed_ = true;
        if (count_ >= max_count_)
        {
            count_ = 0;
        }        
    }
    else
    {
        count_changed_ = false;
    }

    if (switch_linked_led_ != NULL && count_changed_)
	{
		switch_linked_led_->SetValue(count_);
	}
}

void MySwitch::SetCount( const int16_t count )
{
    count_ = count;

    if (switch_linked_led_ != NULL)
	{
		switch_linked_led_->SetValue(count_);
	}
}

void MySwitch::Reset()
{
    SetCount(0);
}

void MySwitch::LinkLed( MyRgbLed *linked_led )
{
    switch_linked_led_ = linked_led;

    if (switch_linked_led_ != NULL)
	{
		switch_linked_led_->SetValue(count_);
	}
}