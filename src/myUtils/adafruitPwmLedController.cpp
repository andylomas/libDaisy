// adafruitPwmLedController.cpp
#include "myUtils/AdafruitPwmLedController.h"
#include "myUtils/shiftOut.h"
#include "per/spi.h"

#define LED_MAX_BRIGHTNESS 65535
#define GLOBAL_MAX_BRIGHTNESS 127

//static SpiHandle h_spi;

void AdafruitPwmLedController::Init(const dsy_gpio_pin pin_data, const dsy_gpio_pin pin_clock)
{
    // Initialize GPIO pins
    pin_data_.pin = pin_data;
    pin_data_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&pin_data_);

    pin_clock_.pin = pin_clock;
    pin_clock_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&pin_clock_);

    // Set initial variable values
    global_brightness_r_ = GLOBAL_MAX_BRIGHTNESS;
    global_brightness_g_ = GLOBAL_MAX_BRIGHTNESS;
    global_brightness_b_ = GLOBAL_MAX_BRIGHTNESS;

    gamma_ = 2.8f;
    
    Clear();
}

// void AdafruitPwmLedController::InitUseSpi(dsy_gpio_pin* pin_cs)
// {
//     // Initialize GPIO pins
//     pin_cs_.pin = pin_cs;
//     pin_cs_.mode = DSY_GPIO_MODE_OUTPUT_PP;
//     dsy_gpio_init(&pin_cs_);

//     // Initialize SPI
//     h_spi.Init();
// }

void AdafruitPwmLedController::SetGlobalBrightness(const float r, const float g, const float b)
{
    global_brightness_r_ = powf(r, gamma_) * GLOBAL_MAX_BRIGHTNESS;
    global_brightness_g_ = powf(g, gamma_) * GLOBAL_MAX_BRIGHTNESS;
    global_brightness_b_ = powf(b, gamma_) * GLOBAL_MAX_BRIGHTNESS;
}

void AdafruitPwmLedController::SetGamma(const float gamma)
{
    gamma_ = gamma;
}
    
void AdafruitPwmLedController::Set(uint8_t n, const bool r, const bool g, const bool b)
{
    led_brightness_r_[n] = r ? LED_MAX_BRIGHTNESS : 0;
    led_brightness_g_[n] = g ? LED_MAX_BRIGHTNESS : 0;
    led_brightness_b_[n] = b ? LED_MAX_BRIGHTNESS : 0;
}

void AdafruitPwmLedController::Set(uint8_t n, const bool v)
{
    Set(n, v, v, v);
}

void AdafruitPwmLedController::SetValue(uint8_t n, const int v)
{
    Set(n, v & 1, v & 2, v & 4);
}
void AdafruitPwmLedController::SetFloat(uint8_t n, float r, float g, float b)
{
    led_brightness_r_[n] = powf(r, gamma_) * LED_MAX_BRIGHTNESS;
    led_brightness_g_[n] = powf(g, gamma_) * LED_MAX_BRIGHTNESS;
    led_brightness_b_[n] = powf(b, gamma_) * LED_MAX_BRIGHTNESS;
}

void AdafruitPwmLedController::SetFloat(uint8_t n, float v)
{
    SetFloat(n, v);
}

void AdafruitPwmLedController::Clear()
{
    for (uint8_t i = 0; i < 4; i ++)
    {
        led_brightness_r_[i] = 0;
        led_brightness_g_[i] = 0;
        led_brightness_b_[i] = 0;
    }

    Update();
}
   
void AdafruitPwmLedController::Update()
{
    // Could have test here whether there has been any change to the data...
    // We send data to the TLC59711 in MSB order

    // // Send Write command (100101b)
    shiftOut(&pin_data_, &pin_clock_, MSBFIRST, 0b100101, 6);

    // // Send Function Control code 10110
    shiftOut(&pin_data_, &pin_clock_, MSBFIRST, 0b10110, 5);

    // // Send Global Brightness Control data (3x 7bits)
    shiftOut(&pin_data_, &pin_clock_, MSBFIRST, global_brightness_b_, 7);
    shiftOut(&pin_data_, &pin_clock_, MSBFIRST, global_brightness_g_, 7);
    shiftOut(&pin_data_, &pin_clock_, MSBFIRST, global_brightness_r_, 7);

    // Send the 16 bit Grayscale data (12 x 16bits)
    for (int16_t i = 3; i >=0; i--)
    {
        shiftOut(&pin_data_, &pin_clock_, MSBFIRST, led_brightness_b_[i] / 256);
        shiftOut(&pin_data_, &pin_clock_, MSBFIRST, led_brightness_b_[i] % 256);
        shiftOut(&pin_data_, &pin_clock_, MSBFIRST, led_brightness_g_[i] / 256);
        shiftOut(&pin_data_, &pin_clock_, MSBFIRST, led_brightness_g_[i] % 256);
        shiftOut(&pin_data_, &pin_clock_, MSBFIRST, led_brightness_r_[i] / 256);
        shiftOut(&pin_data_, &pin_clock_, MSBFIRST, led_brightness_r_[i] % 256);
    }
}
