// super_petal.cpp
#include "super_petal.h"

using namespace daisy;

#ifndef SAMPLE_RATE
//#define SAMPLE_RATE DSY_AUDIO_SAMPLE_RATE
#define SAMPLE_RATE 48014.f
#endif

// Hardware related defines.
// Switches
#define PIN_SW_0 8
#define PIN_SW_1 9
#define PIN_SW_2 10
#define PIN_SW_3 13

// Encoders
#define PIN_ENC_0_A 28
#define PIN_ENC_0_B 27
#define PIN_ENC_0_CLICK 30

#define PIN_ENC_1_A 26
#define PIN_ENC_1_B 25
#define PIN_ENC_1_CLICK 29

#define PIN_ENC_2_A 5
#define PIN_ENC_2_B 6
#define PIN_ENC_2_CLICK 4

#define PIN_ENC_3_A 11
#define PIN_ENC_3_B 12
#define PIN_ENC_3_CLICK 7

// Knobs
#define PIN_KNOB_0 16
#define PIN_KNOB_1 17
#define PIN_KNOB_2 18
#define PIN_KNOB_3 19
#define PIN_KNOB_4 20
#define PIN_KNOB_5 21
#define PIN_KNOB_6 22
#define PIN_KNOB_7 23
#define PIN_KNOB_8 24

// Expression
#define PIN_EXPRESSION 15

// Serial connections to LED and LCD controllers
#define PIN_SERIAL_DATA 0
#define PIN_SERIAL_CLOCK 1
#define PIN_SERIAL_LCD_SELECT 2
#define PIN_SERIAL_LED_SELECT 3

// MIDI pin
#define PIN_MIDI 14

void SuperPetal::Init()
{
    // Set Some numbers up for accessors.
    // Initialize the hardware.
    seed.Configure();
    seed.Init();
    dsy_tim_start();
    InitSwitches();
    InitEncoders();
    InitLeds();
    InitLcd();
    InitAnalogControls();
    SetAudioBlockSize(48);
}

void SuperPetal::DelayMs(size_t del)
{
    seed.DelayMs(del);
}

void SuperPetal::StartAudio(AudioHandle::InterleavingAudioCallback cb)
{
    seed.StartAudio(cb);
}

void SuperPetal::StartAudio(AudioHandle::AudioCallback cb)
{
    seed.StartAudio(cb);
}

void SuperPetal::ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void SuperPetal::ChangeAudioCallback(AudioHandle::AudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void SuperPetal::StopAudio()
{
    seed.StopAudio();
}

void SuperPetal::SetAudioBlockSize(size_t size)
{
    seed.SetAudioBlockSize(size);
}

size_t SuperPetal::AudioBlockSize()
{
    return seed.AudioBlockSize();
}

void SuperPetal::SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate)
{
    seed.SetAudioSampleRate(samplerate);
}

float SuperPetal::AudioSampleRate()
{
    return seed.AudioSampleRate();
}

float SuperPetal::AudioCallbackRate()
{
    return seed.AudioCallbackRate();
}

void SuperPetal::StartAdc()
{
    seed.adc.Start();
}

void SuperPetal::StopAdc()
{
    seed.adc.Stop();
}

void SuperPetal::ProcessAnalogControls()
{
    for(size_t i = 0; i < NUM_KNOBS; i++)
    {
        knob[i].Process();
    }
    expression.Process();
}

float SuperPetal::GetKnobValue(uint8_t k)
{
    return knob[k].Value();
}

float SuperPetal::GetExpression()
{
    return expression.Value();
}

void SuperPetal::ProcessDigitalControls()
{
    for(size_t i = 0; i < NUM_ENCODERS; i++)
    {
        encoder[i].Debounce();
    }

    for(size_t i = 0; i < NUM_SWITCHES; i++)
    {
        switches[i].Debounce();
    }
}

void SuperPetal::ClearLeds()
{
    led_controller.Clear();
}

void SuperPetal::UpdateLeds()
{
    led_controller.Update();
}

void SuperPetal::SetLed(uint8_t idx, bool val)
{
    led_controller.Set(idx, val);
}

void SuperPetal::SetLed(uint8_t idx, bool r, bool g, bool b)
{
    led_controller.Set(idx, r, g, b);
}

void SuperPetal::SetLedValue(uint8_t idx, int val)
{
    led_controller.SetValue(idx, val);
}

void SuperPetal::SetFootswitchLed(uint8_t idx, bool val)
{
    led_controller.Set(idx, val);
}

void SuperPetal::SetFootswitchLed(uint8_t idx, bool r, bool g, bool b)
{
    led_controller.Set(idx, r, g, b);
}

void SuperPetal::SetFootswitchLedValue(uint8_t idx, int val)
{
    led_controller.SetValue(idx, val);
}

void SuperPetal::SetEncoderLed(uint8_t idx, bool val)
{
    led_controller.Set(idx + 4, val);
}

void SuperPetal::SetEncoderLed(uint8_t idx, bool r, bool g, bool b)
{
    led_controller.Set(idx + 4, r, g, b);
}

void SuperPetal::SetEncoderLedValue(uint8_t idx, int val)
{
    led_controller.SetValue(idx + 4, val);
}

void SuperPetal::SetOnboardLed(bool val)
{
    seed.SetLed(val);
}

void SuperPetal::InitSwitches()
{
    uint8_t pin_numbers[NUM_SWITCHES] = {
        PIN_SW_0,
        PIN_SW_1,
        PIN_SW_2,
        PIN_SW_3
    };

    for(size_t i = 0; i < NUM_SWITCHES; i++)
    {
        switches[i].Init(seed.GetPin(pin_numbers[i]), AudioCallbackRate());
    }
}

void SuperPetal::InitEncoders()
{
    dsy_gpio_pin a, b, click;

    a     = seed.GetPin(PIN_ENC_0_A);
    b     = seed.GetPin(PIN_ENC_0_B);
    click = seed.GetPin(PIN_ENC_0_CLICK);
    encoder[0].Init(a, b, click, AudioCallbackRate());

    a     = seed.GetPin(PIN_ENC_1_A);
    b     = seed.GetPin(PIN_ENC_1_B);
    click = seed.GetPin(PIN_ENC_1_CLICK);
    encoder[1].Init(a, b, click, AudioCallbackRate());

    a     = seed.GetPin(PIN_ENC_2_A);
    b     = seed.GetPin(PIN_ENC_2_B);
    click = seed.GetPin(PIN_ENC_2_CLICK);
    encoder[2].Init(a, b, click, AudioCallbackRate());

    a     = seed.GetPin(PIN_ENC_3_A);
    b     = seed.GetPin(PIN_ENC_3_B);
    click = seed.GetPin(PIN_ENC_3_CLICK);
    encoder[3].Init(a, b, click, AudioCallbackRate());
}

void SuperPetal::InitLeds()
{
    dsy_gpio_pin serial_data = seed.GetPin(PIN_SERIAL_DATA);
    dsy_gpio_pin serial_clock = seed.GetPin(PIN_SERIAL_CLOCK);
    dsy_gpio_pin serial_led_select = seed.GetPin(PIN_SERIAL_LED_SELECT);

    led_controller.Init(serial_data, serial_clock, serial_led_select);
}

void SuperPetal::InitLcd()
{
    dsy_gpio_pin serial_data = seed.GetPin(PIN_SERIAL_DATA);
    dsy_gpio_pin serial_clock = seed.GetPin(PIN_SERIAL_CLOCK);
    dsy_gpio_pin serial_lcd_select = seed.GetPin(PIN_SERIAL_LCD_SELECT);

    lcd.init(serial_data, serial_clock, serial_lcd_select);
    lcd.begin(16, 2);
    lcd.clear();
}

void SuperPetal::InitAnalogControls()
{
    // Set order of ADCs based on CHANNEL NUMBER
    // NUN_KNOBS + 1 because of Expression input
    AdcChannelConfig cfg[NUM_KNOBS + 1];
    // Init with Single Pins
    cfg[0].InitSingle(seed.GetPin(PIN_KNOB_0));
    cfg[1].InitSingle(seed.GetPin(PIN_KNOB_1));
    cfg[2].InitSingle(seed.GetPin(PIN_KNOB_2));
    cfg[3].InitSingle(seed.GetPin(PIN_KNOB_3));
    cfg[4].InitSingle(seed.GetPin(PIN_KNOB_4));
    cfg[5].InitSingle(seed.GetPin(PIN_KNOB_5));
    cfg[6].InitSingle(seed.GetPin(PIN_KNOB_6));
    cfg[7].InitSingle(seed.GetPin(PIN_KNOB_7));
    cfg[8].InitSingle(seed.GetPin(PIN_KNOB_8));

    // Special case for Expression
    cfg[9].InitSingle(seed.GetPin(PIN_EXPRESSION));

    seed.adc.Init(cfg, NUM_KNOBS + 1);
    // Make an array of pointers to the knob.
    for(int i = 0; i < NUM_KNOBS; i++)
    {
        knob[i].Init(seed.adc.GetPtr(i), AudioCallbackRate());
    }
    expression.Init(seed.adc.GetPtr(NUM_KNOBS), AudioCallbackRate());
}
