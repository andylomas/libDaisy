// super_patch.cpp
#include "super_patch.h"

using namespace daisy;

#ifndef SAMPLE_RATE
//#define SAMPLE_RATE DSY_AUDIO_SAMPLE_RATE
#define SAMPLE_RATE 48014.f
#endif

// Hardware related defines.
#define PIN_BUT_0 29
#define PIN_BUT_1 30

#define PIN_ENC_0_A 0
#define PIN_ENC_0_B 1
#define PIN_ENC_0_CLICK 2

#define PIN_ENC_1_A 3
#define PIN_ENC_1_B 4
#define PIN_ENC_1_CLICK 5

#define PIN_KNOB_0 15
#define PIN_KNOB_1 16
#define PIN_KNOB_2 17
#define PIN_KNOB_3 18
#define PIN_KNOB_4 19
#define PIN_KNOB_5 20
#define PIN_KNOB_6 21
#define PIN_KNOB_7 28

#define PIN_OLED_RES 6
#define PIN_OLED_DC 9

#define PIN_LED_SCL 11
#define PIN_LED_SDA 12

#define PIN_SERIAL_IN 14
#define PIN_SERIAL_OUT 14

#define PIN_BANANA_0 25
#define PIN_BANANA_1 23
#define PIN_BANANA_2 24
#define PIN_BANANA_3 22

void SuperPatch::Init()
{
    // Set Some numbers up for accessors.
    // Initialize the hardware.
    seed.Configure();
    seed.Init();
    dsy_tim_start();
    InitSwitches();
    InitEncoders();
    InitAnalogControls();
    InitOledDisplay();
    InitLedController();
    SetAudioBlockSize(48);
}

void SuperPatch::DelayMs(size_t del)
{
    seed.DelayMs(del);
}

void SuperPatch::StartAudio(AudioHandle::InterleavingAudioCallback cb)
{
    seed.StartAudio(cb);
}

void SuperPatch::StartAudio(AudioHandle::AudioCallback cb)
{
    seed.StartAudio(cb);
}

void SuperPatch::ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void SuperPatch::ChangeAudioCallback(AudioHandle::AudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void SuperPatch::StopAudio()
{
    seed.StopAudio();
}

void SuperPatch::SetAudioBlockSize(size_t size)
{
    seed.SetAudioBlockSize(size);
}

size_t SuperPatch::AudioBlockSize()
{
    return seed.AudioBlockSize();
}

void SuperPatch::SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate)
{
    seed.SetAudioSampleRate(samplerate);
}

float SuperPatch::AudioSampleRate()
{
    return seed.AudioSampleRate();
}

float SuperPatch::AudioCallbackRate()
{
    return seed.AudioCallbackRate();
}

void SuperPatch::StartAdc()
{
    seed.adc.Start();
}

void SuperPatch::StopAdc()
{
    seed.adc.Stop();
}

void SuperPatch::ProcessAnalogControls()
{
    for(size_t i = 0; i < NUM_KNOBS; i++)
    {
        knob[i].Process();
        // knob[i].Debounce();
    }
}

float SuperPatch::GetKnobValue(uint8_t k)
{
    return knob[k].Value();
}

void SuperPatch::ProcessDigitalControls()
{
    for(size_t i = 0; i < NUM_ENCODERS; i++)
    {
        encoder[i].Debounce();
    }

    for(size_t i = 0; i < NUM_BUTTONS; i++)
    {
        button[i].Debounce();
    }
}

void SuperPatch::ClearLeds()
{
    led_controller.Clear();
}

void SuperPatch::UpdateLeds()
{
    led_controller.Update();
}

void SuperPatch::SetLed(uint8_t idx, bool val)
{
	led_controller.Set(idx, val);
}

void SuperPatch::SetLed(uint8_t idx, bool r, bool g, bool b)
{
    led_controller.Set(idx, r, g, b);
}

void SuperPatch::SetLedValue(uint8_t idx, int val)
{
    led_controller.SetValue(idx, val);
}

void SuperPatch::SetLedFloat(uint8_t idx, float r, float g, float b)
{
    led_controller.SetFloat(idx, r, g, b);
}

void SuperPatch::SetOnboardLed(bool val)
{
    seed.SetLed(val);
}

void SuperPatch::InitSwitches()
{
    uint8_t button_pin_numbers[NUM_BUTTONS] = {
        PIN_BUT_0,
        PIN_BUT_1
    };

    for(size_t i = 0; i < NUM_BUTTONS; i++)
    {
        button[i].Init(seed.GetPin(button_pin_numbers[i]), AudioCallbackRate());
    }    
}

void SuperPatch::InitEncoders()
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
}

void SuperPatch::InitAnalogControls()
{
    // Set order of ADCs based on CHANNEL NUMBER
    AdcChannelConfig cfg[NUM_KNOBS];

    // Init with Single Pins
    cfg[0].InitSingle(seed.GetPin(PIN_KNOB_0));
    cfg[1].InitSingle(seed.GetPin(PIN_KNOB_1));
    cfg[2].InitSingle(seed.GetPin(PIN_KNOB_2));
    cfg[3].InitSingle(seed.GetPin(PIN_KNOB_3));
    cfg[4].InitSingle(seed.GetPin(PIN_KNOB_4));
    cfg[5].InitSingle(seed.GetPin(PIN_KNOB_5));
    cfg[6].InitSingle(seed.GetPin(PIN_KNOB_6));
    cfg[7].InitSingle(seed.GetPin(PIN_KNOB_7));

    seed.adc.Init(cfg, NUM_KNOBS);

    // Make an array of pointers to the knob.
    for(int i = 0; i < NUM_KNOBS; i++)
    {
        knob[i].Init(seed.adc.GetPtr(i), AudioCallbackRate());
    }
}

void SuperPatch::InitOledDisplay()
{
    // Initialize the SDD1309 128x64 pixel OLED display (using hardware SPI)
    dsy_gpio_pin oled_display_pins[OledDisplay::NUM_PINS];
    oled_display_pins[OledDisplay::DATA_COMMAND] = seed.GetPin(PIN_OLED_DC);
    oled_display_pins[OledDisplay::RESET]        = seed.GetPin(PIN_OLED_RES);
    dsy_system_delay(1000);
    display.Init(oled_display_pins, true);
}

void SuperPatch::InitLedController()
{
    // Initialize the Adafruit LED Controller
    dsy_gpio_pin led_clock_pin = seed.GetPin(PIN_LED_SCL);
    dsy_gpio_pin led_data_pin = seed.GetPin(PIN_LED_SDA); 
    led_controller.Init(led_data_pin, led_clock_pin);
    led_controller.SetGlobalBrightness(1.0, 0.35, 0.3);
    led_controller.Clear();
}
