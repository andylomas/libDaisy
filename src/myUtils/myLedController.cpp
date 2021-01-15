// myLedController.cpp
#include "myUtils/myLedController.h"

void MyLedController::Init(const dsy_gpio_pin pin_data, const dsy_gpio_pin pin_clock, const dsy_gpio_pin pin_led_select)
{
    pin_data_.pin = pin_data;
    pin_data_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&pin_data_);

    pin_clock_.pin = pin_clock;
    pin_clock_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&pin_clock_);

    pin_led_select_.pin = pin_led_select;
    pin_led_select_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&pin_led_select_);

    r_data_ = 0;
    g_data_ = 0;
    b_data_ = 0;
    
    Clear();
}
    
void MyLedController::Set(uint8_t n, const bool r, const bool g, const bool b)
{
    uint8_t m = 1 << n;

    if (r)
        r_data_ |= m;
    else
        r_data_ &= ~m;
    
    if (g)
        g_data_ |= m;
    else
        g_data_ &= ~m;

    if (b)
        b_data_ |= m;
    else
        b_data_ &= ~m;
}

void MyLedController::Set(uint8_t n, const bool v)
{
    Set(n, v, v, v);
}

void MyLedController::SetValue(uint8_t n, const int v)
{
    Set(n, v & 1, v & 2, v & 4);
}

void MyLedController::Clear()
{
    r_data_ = 0;
    g_data_ = 0;
    b_data_ = 0;

    Update(true);
}
   
void MyLedController::Update(bool force_update)
{
    uint32_t cur_data = r_data_ + (g_data_ << 8) + (b_data_ << 16);

    if (force_update || cur_data != prev_data_)
    {
        shiftOut(&pin_data_, &pin_clock_, MSBFIRST, b_data_);
        shiftOut(&pin_data_, &pin_clock_, MSBFIRST, g_data_);
        shiftOut(&pin_data_, &pin_clock_, MSBFIRST, r_data_);

        // send high pulse to latch pin so the LEDs will light up:
        dsy_gpio_write(&pin_led_select_, 1);
        dsy_gpio_write(&pin_led_select_, 0); 

        prev_data_ = cur_data;
    }
}
