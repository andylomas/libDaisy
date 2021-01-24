// adafruitPwmLedController.cpp
#include "myUtils/AdafruitPwmLedController.h"
#include "myUtils/shiftOut.h"
#include "per/spi.h"

#define LED_MAX_BRIGHTNESS 65535
#define GLOBAL_MAX_BRIGHTNESS 127

static daisy::SpiHandle h_spi;

AdafruitPwmLedController::AdafruitPwmLedController()
{
    // Constructor

    // Default gamma setting
    SetGamma(2.8f);

    // Initialize data buffer values

    // Write command 0b100101
    SetDataBufferBits(0b100101, 6, 218);
    
    // Function Control code 0b10110
    SetDataBufferBits(0b10110, 5, 213);
    
    // Global brightness values
    SetGlobalBrightness(1, 1, 1);

    // LED brightness values
    for (uint8_t i = 0; i < 4; i ++)
    {
        Set(i, 0);
    }

    use_cs_ = false;
    use_spi_ = false;
}

void AdafruitPwmLedController::Init(const dsy_gpio_pin pin_data, const dsy_gpio_pin pin_clock)
{
    // Initialize to use ShiftOut
    pin_data_.pin = pin_data;
    pin_data_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&pin_data_);

    pin_clock_.pin = pin_clock;
    pin_clock_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&pin_clock_);

    Update();
}

void AdafruitPwmLedController::Init(const dsy_gpio_pin pin_data, const dsy_gpio_pin pin_clock, const dsy_gpio_pin pin_cs)
{
    // Version of Init() with CS pin specified

    // Initialize GPIO for CS
    pin_cs_.pin = pin_cs;
    pin_cs_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&pin_cs_);
    use_cs_ = true;
    
    // Call InitSpi() to set up everything else
    Init(pin_data, pin_clock);
}

void AdafruitPwmLedController::InitSpi()
{
    // Initialize SPI
    h_spi.Init();
    use_spi_ = true;

    Update();
}

void AdafruitPwmLedController::InitSpi(const dsy_gpio_pin pin_cs)
{
    // Version of InitSpi() with CS pin specified

    // Initialize GPIO for CS
    pin_cs_.pin = pin_cs;
    pin_cs_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&pin_cs_);
    use_cs_ = true;
    
    // Call InitSpi() to set up everything else
    InitSpi();
}

void AdafruitPwmLedController::SetDataBufferBits(uint8_t v, uint8_t num_bits, uint8_t pos)
{
    // Note: we assume that we're setting at most 8 bits
    uint8_t byte_pos = pos / 8;
    uint8_t bit_pos = pos % 8;

    for (int i = 0; i < num_bits; i++)
    {
        if ( v & (1 << i))
        {
            // Set bit_pos at byte_pos
            data_buffer_[DATA_BUFFER_SIZE - byte_pos - 1] |= (1 << bit_pos);
        }
        else
        {
            // Clear bit_pos at byte_pos
            data_buffer_[DATA_BUFFER_SIZE - byte_pos - 1] &= ~(1 << bit_pos);
        }
        
        bit_pos++;
        if (bit_pos > 7)
        {
            bit_pos = 0;
            byte_pos++;
        }
    }
}

void AdafruitPwmLedController::SetGlobalBrightness(const float r, const float g, const float b)
{
    uint8_t global_brightness_r = powf(r, gamma_) * GLOBAL_MAX_BRIGHTNESS;
    uint8_t global_brightness_g = powf(g, gamma_) * GLOBAL_MAX_BRIGHTNESS;
    uint8_t global_brightness_b = powf(b, gamma_) * GLOBAL_MAX_BRIGHTNESS;

    SetDataBufferBits(global_brightness_r, 7, 192);
    SetDataBufferBits(global_brightness_g, 7, 199);
    SetDataBufferBits(global_brightness_b, 7, 206);
}

void AdafruitPwmLedController::SetGamma(const float gamma)
{
    gamma_ = gamma;
}

void AdafruitPwmLedController::SetRaw(uint8_t n, const uint16_t r, const uint16_t g, const uint16_t b)
{
    uint8_t buffer_pos = 6 * n;
    data_buffer_[DATA_BUFFER_SIZE - buffer_pos - 1] = r % 256;
    data_buffer_[DATA_BUFFER_SIZE - buffer_pos - 2] = r / 256;
    data_buffer_[DATA_BUFFER_SIZE - buffer_pos - 3] = g % 256;
    data_buffer_[DATA_BUFFER_SIZE - buffer_pos - 4] = g / 256;
    data_buffer_[DATA_BUFFER_SIZE - buffer_pos - 5] = b % 256;
    data_buffer_[DATA_BUFFER_SIZE - buffer_pos - 6] = b / 256;   
}

    
void AdafruitPwmLedController::Set(uint8_t n, const bool r, const bool g, const bool b)
{
    uint16_t led_brightness_r = r ? LED_MAX_BRIGHTNESS : 0;
    uint16_t led_brightness_g = g ? LED_MAX_BRIGHTNESS : 0;
    uint16_t led_brightness_b = b ? LED_MAX_BRIGHTNESS : 0;
    SetRaw(n, led_brightness_r, led_brightness_g, led_brightness_b);
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
    uint16_t led_brightness_r = powf(r, gamma_) * LED_MAX_BRIGHTNESS;
    uint16_t led_brightness_g = powf(g, gamma_) * LED_MAX_BRIGHTNESS;
    uint16_t led_brightness_b = powf(b, gamma_) * LED_MAX_BRIGHTNESS;
    SetRaw(n, led_brightness_r, led_brightness_g, led_brightness_b);
}

void AdafruitPwmLedController::SetFloat(uint8_t n, float v)
{
    SetFloat(n, v);
}

void AdafruitPwmLedController::Clear()
{
    for (uint8_t i = 0; i < 4; i ++)
    {
        Set(i, 0, 0, 0);
    }

    Update();
}
   
void AdafruitPwmLedController::Update()
{
    // If we're using a CS pin, set it high
    if (use_cs_)
    {
        dsy_gpio_write(&pin_cs_, 1);
    }

    if (use_spi_)
    {
        h_spi.BlockingTransmit(data_buffer_, DATA_BUFFER_SIZE);
    }
    else
    {
        // Could have test here whether there has been any change to the data...
        // We send data to the TLC59711 in MSB order
        shiftOutBuffer(&pin_data_, &pin_clock_, MSBFIRST, data_buffer_, DATA_BUFFER_SIZE);

        // Set the data value back to 0 clean up
        dsy_gpio_write(&pin_data_, 0);
    }

    // If we're using a CS pin, set it low
    if (use_cs_)
    {
        dsy_gpio_write(&pin_cs_, 0);
    }
}
