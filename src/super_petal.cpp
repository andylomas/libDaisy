#include "super_petal.h"

using namespace daisy;

#ifndef SAMPLE_RATE
//#define SAMPLE_RATE DSY_AUDIO_SAMPLE_RATE
#define SAMPLE_RATE 48014.f
#endif

// Hardware related defines.
// Switches
#define SW_0_PIN 8
#define SW_1_PIN 9
#define SW_2_PIN 10
#define SW_3_PIN 13

// Encoders
// #define ENC_A_PIN 28
// #define ENC_B_PIN 27
// #define ENC_CLICK_PIN 30

#define ENC_0_A_PIN 28
#define ENC_0_B_PIN 27
#define ENC_0_CLICK_PIN 30

#define ENC_1_A_PIN 26
#define ENC_1_B_PIN 25
#define ENC_1_CLICK_PIN 29

#define ENC_2_A_PIN 5
#define ENC_2_B_PIN 6
#define ENC_2_CLICK_PIN 4

#define ENC_3_A_PIN 11
#define ENC_3_B_PIN 12
#define ENC_3_CLICK_PIN 7

// Knobs
#define PIN_EXPRESSION 15
#define PIN_KNOB_0 16
#define PIN_KNOB_1 17
#define PIN_KNOB_2 18
#define PIN_KNOB_3 19
#define PIN_KNOB_4 20
#define PIN_KNOB_5 21
#define PIN_KNOB_6 22
#define PIN_KNOB_7 23
#define PIN_KNOB_8 24

// Serial connections to LED and LCD controllers
#define PIN_SERIAL_DATA 0
#define PIN_SERIAL_CLOCK 1
#define PIN_LCD_SELECT 2
#define PIN_LED_SELECT 3

// MIDI pin
#define PIN_MIDI

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
    InitAnalogControls();
    SetAudioBlockSize(48);
    //seed.usb_handle.Init(UsbHandle::FS_INTERNAL);
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
    for(size_t i = 0; i < KNOB_LAST; i++)
    {
        knob[i].Process();
    }
    expression.Process();
}

float SuperPetal::GetKnobValue(Knob k)
{
    size_t idx;
    idx = k < KNOB_LAST ? k : KNOB_1;
    return knob[idx].Value();
}

float SuperPetal::GetExpression()
{
    return expression.Value();
}

void SuperPetal::ProcessDigitalControls()
{
    for(size_t i = 0; i < 4; i++)
    {
        encoder[i].Debounce();
    }

    for(size_t i = 0; i < SW_LAST; i++)
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

void SuperPetal::InitSwitches()
{
    uint8_t pin_numbers[SW_LAST] = {
        SW_0_PIN,
        SW_1_PIN,
        SW_2_PIN,
        SW_3_PIN
    };

    for(size_t i = 0; i < SW_LAST; i++)
    {
        switches[i].Init(seed.GetPin(pin_numbers[i]), AudioCallbackRate());
    }
}

void SuperPetal::InitEncoders()
{
    dsy_gpio_pin a, b, click;

    a     = seed.GetPin(ENC_0_A_PIN);
    b     = seed.GetPin(ENC_0_B_PIN);
    click = seed.GetPin(ENC_0_CLICK_PIN);
    encoder[0].Init(a, b, click, AudioCallbackRate());

    a     = seed.GetPin(ENC_1_A_PIN);
    b     = seed.GetPin(ENC_1_B_PIN);
    click = seed.GetPin(ENC_1_CLICK_PIN);
    encoder[1].Init(a, b, click, AudioCallbackRate());

    a     = seed.GetPin(ENC_2_A_PIN);
    b     = seed.GetPin(ENC_2_B_PIN);
    click = seed.GetPin(ENC_2_CLICK_PIN);
    encoder[2].Init(a, b, click, AudioCallbackRate());

    a     = seed.GetPin(ENC_3_A_PIN);
    b     = seed.GetPin(ENC_3_B_PIN);
    click = seed.GetPin(ENC_3_CLICK_PIN);
    encoder[3].Init(a, b, click, AudioCallbackRate());
}

void SuperPetal::InitLeds()
{
    dsy_gpio_pin serial_data = seed.GetPin(PIN_SERIAL_DATA);
    dsy_gpio_pin serial_clock = seed.GetPin(PIN_SERIAL_CLOCK);
    dsy_gpio_pin serial_led_select = seed.GetPin(PIN_LED_SELECT);

    led_controller.Init(serial_data, serial_clock, serial_led_select);
}

void SuperPetal::InitAnalogControls()
{
    // Set order of ADCs based on CHANNEL NUMBER
    // KNOB_LAST + 1 because of Expression input
    AdcChannelConfig cfg[KNOB_LAST + 1];
    // Init with Single Pins
    cfg[KNOB_0].InitSingle(seed.GetPin(PIN_KNOB_0));
    cfg[KNOB_1].InitSingle(seed.GetPin(PIN_KNOB_1));
    cfg[KNOB_2].InitSingle(seed.GetPin(PIN_KNOB_2));
    cfg[KNOB_3].InitSingle(seed.GetPin(PIN_KNOB_3));
    cfg[KNOB_4].InitSingle(seed.GetPin(PIN_KNOB_4));
    cfg[KNOB_5].InitSingle(seed.GetPin(PIN_KNOB_5));
    cfg[KNOB_6].InitSingle(seed.GetPin(PIN_KNOB_6));
    cfg[KNOB_7].InitSingle(seed.GetPin(PIN_KNOB_7));
    cfg[KNOB_8].InitSingle(seed.GetPin(PIN_KNOB_8));
    // Special case for Expression
    cfg[KNOB_LAST].InitSingle(seed.GetPin(PIN_EXPRESSION));

    printf("KNOB_LAST = %d\n", KNOB_LAST);

    seed.adc.Init(cfg, KNOB_LAST + 1);
    // Make an array of pointers to the knob.
    for(int i = 0; i < KNOB_LAST; i++)
    {
        knob[i].Init(seed.adc.GetPtr(i), AudioCallbackRate());
    }
    expression.Init(seed.adc.GetPtr(KNOB_LAST), AudioCallbackRate());
}
