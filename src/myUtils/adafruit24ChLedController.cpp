// adafruit23ChLedController.cpp
#include "myUtils/Adafruit24ChLedController.h"

#define LED_MAX_BRIGHTNESS 4095
#define NUM_LEDS 8
#define NUM_CHANNELS 24

static daisy::SpiHandle h_spi;

Adafruit24ChLedController::Adafruit24ChLedController()
{
    // Constructor

    // Default gamma setting
    SetGamma(2.8f);
    
    // Global brightness values
    SetGlobalBrightness(1, 1, 1);

    // LED brightness values
    for (uint8_t i = 0; i < NUM_LEDS; i ++)
    {
        Set(i, 0);
    }
}

void Adafruit24ChLedController::Init(const dsy_gpio_pin pin_data, const dsy_gpio_pin pin_clock, const dsy_gpio_pin pin_latch)
{
    pin_data_.pin = pin_data;
    pin_data_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&pin_data_);

    pin_clock_.pin = pin_clock;
    pin_clock_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&pin_clock_);

    pin_latch_.pin = pin_latch;
    pin_latch_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&pin_latch_);
    
    Update();
}

void Adafruit24ChLedController::SetGlobalBrightness(const float r, const float g, const float b)
{
    global_brightness_r_ = r;
    global_brightness_g_ = g;
    global_brightness_b_ = b;

    led_max_value_r_ = global_brightness_r_ * LED_MAX_BRIGHTNESS;
    led_max_value_g_ = global_brightness_g_ * LED_MAX_BRIGHTNESS;
    led_max_value_b_ = global_brightness_b_ * LED_MAX_BRIGHTNESS;
}

void Adafruit24ChLedController::SetGamma(const float gamma)
{
    gamma_ = gamma;
}

void Adafruit24ChLedController::SetRaw(uint8_t n, const uint16_t r, const uint16_t g, const uint16_t b)
{
    pwm_buffer_[3 * n] = r;
    pwm_buffer_[3 * n + 1] = g;
    pwm_buffer_[3 * n + 2] = b; 
}

    
void Adafruit24ChLedController::Set(uint8_t n, const bool r, const bool g, const bool b)
{
    uint16_t led_brightness_r = r ? led_max_value_r_ : 0;
    uint16_t led_brightness_g = g ? led_max_value_g_ : 0;
    uint16_t led_brightness_b = b ? led_max_value_b_ : 0;
    SetRaw(n, led_brightness_r, led_brightness_g, led_brightness_b);
}

void Adafruit24ChLedController::Set(uint8_t n, const bool v)
{
    Set(n, v, v, v);
}

void Adafruit24ChLedController::SetValue(uint8_t n, const int v)
{
    Set(n, v & 1, v & 2, v & 4);
}

void Adafruit24ChLedController::SetFloat(uint8_t n, float r, float g, float b)
{
    uint16_t led_brightness_r = powf(r, gamma_) * led_max_value_r_;
    uint16_t led_brightness_g = powf(g, gamma_) * led_max_value_g_;
    uint16_t led_brightness_b = powf(b, gamma_) * led_max_value_b_;
    SetRaw(n, led_brightness_r, led_brightness_g, led_brightness_b);
}

void Adafruit24ChLedController::SetFloat(uint8_t n, float v)
{
    SetFloat(n, v, v, v);
}

void Adafruit24ChLedController::Clear()
{
    for (uint8_t i = 0; i < NUM_LEDS; i ++)
    {
        Set(i, 0, 0, 0);
    }

    Update();
}
   
void Adafruit24ChLedController::Update()
{
    dsy_gpio_write(&pin_latch_, 0);

    // 24 channels per TLC5974
    for (int16_t c = NUM_CHANNELS - 1; c >= 0; c--) {
        // 12 bits per channel, send MSB first
        for (int8_t b = 11; b >= 0; b--) {
            dsy_gpio_write(&pin_clock_, 0);

        if (pwm_buffer_[c] & (1 << b))
            dsy_gpio_write(&pin_data_, 1);
        else
            dsy_gpio_write(&pin_data_, 0);

        dsy_gpio_write(&pin_clock_, 1);
        }
    }
    dsy_gpio_write(&pin_clock_, 0);

    dsy_gpio_write(&pin_latch_, 1);
    dsy_gpio_write(&pin_latch_, 0);
}
