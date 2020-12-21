// myRgbLed.cpp
#include "myUtils/myRgbLed.h"

#define safeMod(i, n) (i % n + n) % n

#define MIN_FLOAT_R 0.003f
#define MIN_FLOAT_G 0.003f
#define MIN_FLOAT_B 0.003f

void MyRgbLed::Init(const dsy_gpio_pin pin_r, const dsy_gpio_pin pin_g, const dsy_gpio_pin pin_b)
{
    pin_r_.pin = pin_r;
    pin_r_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&pin_r_);

    pin_g_.pin = pin_g;
    pin_g_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&pin_g_);

    pin_b_.pin = pin_b;
    pin_b_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&pin_b_);

    table_ = NULL;

    r_val_ = 0.0f;
    g_val_ = 0.0f;
    b_val_ = 0.0f;

    r_flag_ = false;
    g_flag_ = false;
    b_flag_ = false;

    float_mode_ = false;
}

void MyRgbLed::Init(MyLedController *led_controller, uint8_t n)
{
    led_controller_ = led_controller;
    led_index_ = n;

    table_ = NULL;

    r_val_ = 0.0f;
    g_val_ = 0.0f;
    b_val_ = 0.0f;

    float_mode_ = false;
}

void MyRgbLed::Set(const bool r, const bool g, const bool b)
{
    if (led_controller_ == NULL)
    {
        dsy_gpio_write(&pin_r_, r);
        dsy_gpio_write(&pin_g_, g);
        dsy_gpio_write(&pin_b_, b);
    }
    else
    {
        led_controller_->Set(led_index_, r, g, b);
    }
    
}

void MyRgbLed::SetValue(const int v)
{
    // If a color table has been defined use that
    if (table_ != NULL)
    {
        int index = safeMod(v, table_size_);
        Set(table_[index].r, table_[index].g, table_[index].b);
    }
    else
    {
        // Uses v modulo 8 to set the RGB colour
        Set(v & 1, v & 2, v & 4);
    }
}

void MyRgbLed::SetTable(int size, RgbLedColor *table)
{
    table_size_ = size;
    table_ = table;
}

void MyRgbLed::SetFloat(const float r, const float g, const float b)
{
    float rr = r*r;
    if (rr < 0.5)
    {
        if (rr < MIN_FLOAT_R) rr = 0.0f;
        r_main_ = false;
        r_inc_ = rr / (1.0f - rr);
    }
    else
    {
        r_main_ = true;
        r_inc_ = (1.0f - rr) / rr;
    }
    
    float gg = g*g;
    if (gg < 0.5)
    {
        if (gg < MIN_FLOAT_G) gg = 0.0f;
        g_main_ = false;
        g_inc_ = gg / (1.0f - gg);
    }
    else
    {
        g_main_ = true;
        g_inc_ = (1.0f - gg) / gg;
    }

    float bb = b*b;
    if (bb < 0.5)
    {
        if (bb < MIN_FLOAT_B) bb = 0.0f;
        b_main_ = false;
        b_inc_ = bb / (1.0f - bb);
    }
    else
    {
        b_main_ = true;
        b_inc_ = (1.0f - bb) / bb;
    }

    // Set the flags so that the led gets updates next cycle
    r_flag_ = true;
    g_flag_ = true;
    b_flag_ = true;

    float_mode_ = true;
}

void MyRgbLed::Update()
{
    // Only used in float mode to switch the RGB values on or off each cycle
    if (float_mode_)
    {
        bool r, g, b;

        if (r_val_ >= 1.0f)
        {
            r = !r_main_;
            r_val_ -= 1.0f;
        }
        else
        {
            r = r_main_;
            r_val_ += r_inc_; 
        }
        
        if (g_val_ >= 1.0f)
        {
            g = !g_main_;
            g_val_ -= 1.0f;
        }
        else
        {
            g = g_main_;
            g_val_ += g_inc_; 
        }

        if (b_val_ >= 1.0f)
        {
            b = !b_main_;
            b_val_ -= 1.0f;
        }
        else
        {
            b = b_main_;
            b_val_ += b_inc_; 
        }

        Set(r, g, b);
    }
}
