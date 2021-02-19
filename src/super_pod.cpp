// super_pod.cpp
#include "super_pod.h"

using namespace daisy;

#ifndef SAMPLE_RATE
//#define SAMPLE_RATE DSY_AUDIO_SAMPLE_RATE
#define SAMPLE_RATE 48014.f
#endif

// Hardware related defines.
#define PIN_SW_0 3
#define PIN_SW_1 2
#define PIN_SW_2 1
#define PIN_SW_3 0

#define PIN_BUT_0 7
#define PIN_BUT_1 6
#define PIN_BUT_2 5
#define PIN_BUT_3 4

#define PIN_LED_0_RED 11
#define PIN_LED_0_GREEN 12
#define PIN_LED_0_BLUE 13

#define PIN_LED_1_RED 8
#define PIN_LED_1_GREEN 9
#define PIN_LED_1_BLUE 10

#define PIN_ENC_0_A 27
#define PIN_ENC_0_B 26
#define PIN_ENC_0_CLICK 5

#define PIN_ENC_1_A 30
#define PIN_ENC_1_B 29
#define PIN_ENC_1_CLICK 4

#define PIN_KNOB_0 23
#define PIN_KNOB_1 24
#define PIN_KNOB_2 25
#define PIN_KNOB_3 28
#define PIN_KNOB_4 19
#define PIN_KNOB_5 20
#define PIN_KNOB_6 21
#define PIN_KNOB_7 22
#define PIN_KNOB_8 15
#define PIN_KNOB_9 16
#define PIN_KNOB_10 17
#define PIN_KNOB_11 18

#define PIN_MIDI 14

void SuperPod::Init()
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
}

void SuperPod::DelayMs(size_t del)
{
    seed.DelayMs(del);
}

void SuperPod::StartAudio(AudioHandle::InterleavingAudioCallback cb)
{
    seed.StartAudio(cb);
}

void SuperPod::StartAudio(AudioHandle::AudioCallback cb)
{
    seed.StartAudio(cb);
}

void SuperPod::ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void SuperPod::ChangeAudioCallback(AudioHandle::AudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void SuperPod::StopAudio()
{
    seed.StopAudio();
}

void SuperPod::SetAudioBlockSize(size_t size)
{
    seed.SetAudioBlockSize(size);
}

size_t SuperPod::AudioBlockSize()
{
    return seed.AudioBlockSize();
}

void SuperPod::SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate)
{
    seed.SetAudioSampleRate(samplerate);
}

float SuperPod::AudioSampleRate()
{
    return seed.AudioSampleRate();
}

float SuperPod::AudioCallbackRate()
{
    return seed.AudioCallbackRate();
}

void SuperPod::StartAdc()
{
    seed.adc.Start();
}

void SuperPod::StopAdc()
{
    seed.adc.Stop();
}

void SuperPod::ProcessAnalogControls()
{
    for(size_t i = 0; i < NUM_KNOBS; i++)
    {
        knob[i].Process();
        // knob[i].Debounce();
    }
}

float SuperPod::GetKnobValue(uint8_t k)
{
    return knob[k].Value();
}

void SuperPod::ProcessDigitalControls()
{
    for(size_t i = 0; i < NUM_ENCODERS; i++)
    {
        encoder[i].Debounce();
    }

    for(size_t i = 0; i < NUM_SWITCHES; i++)
    {
        switches[i].Debounce();
    }

    for(size_t i = 0; i < NUM_BUTTONS; i++)
    {
        button[i].Debounce();
    }
}

void SuperPod::ClearLeds()
{
    for (int i = 0; i < NUM_LEDS; i++)
	{
		led[i].Set(false, false, false);
	}
}

void SuperPod::UpdateLeds()
{
    for (int i = 0; i < NUM_LEDS; i++)
	{
		led[i].Update();
	}
}

void SuperPod::SetLed(uint8_t idx, bool val)
{
	led[idx].Set(val);
}

void SuperPod::SetLed(uint8_t idx, bool r, bool g, bool b)
{
    led[idx].Set(r, g, b);
}

void SuperPod::SetLedValue(uint8_t idx, int val)
{
    led[idx].SetValue(val);
}

void SuperPod::SetLedFloat(uint8_t idx, float r, float g, float b)
{
    led[idx].SetFloat(r, g, b);
}

void SuperPod::SetOnboardLed(bool val)
{
    seed.SetLed(val);
}

void SuperPod::InitSwitches()
{
    uint8_t switch_pin_numbers[NUM_SWITCHES] = {
        PIN_SW_0,
        PIN_SW_1,
        PIN_SW_2,
        PIN_SW_3
    };

    for(size_t i = 0; i < NUM_SWITCHES; i++)
    {
        switches[i].Init(seed.GetPin(switch_pin_numbers[i]), AudioCallbackRate());
    }

    uint8_t button_pin_numbers[NUM_BUTTONS] = {
        PIN_BUT_0,
        PIN_BUT_1,
        PIN_BUT_2,
        PIN_BUT_3
    };

    for(size_t i = 0; i < NUM_BUTTONS; i++)
    {
        button[i].Init(seed.GetPin(button_pin_numbers[i]), AudioCallbackRate());
    }    
}

void SuperPod::InitEncoders()
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

void SuperPod::InitLeds()
{
    dsy_gpio_pin pin_0_red = seed.GetPin(PIN_LED_0_RED);
    dsy_gpio_pin pin_0_green = seed.GetPin(PIN_LED_0_GREEN);
    dsy_gpio_pin pin_0_blue = seed.GetPin(PIN_LED_0_BLUE);

    dsy_gpio_pin pin_1_red = seed.GetPin(PIN_LED_1_RED);
    dsy_gpio_pin pin_1_green = seed.GetPin(PIN_LED_1_GREEN);
    dsy_gpio_pin pin_1_blue = seed.GetPin(PIN_LED_1_BLUE);

    led[0].Init(pin_0_red, pin_0_green, pin_0_blue);
    led[1].Init(pin_1_red, pin_1_green, pin_1_blue);

	for (int i = 0; i < NUM_LEDS; i++)
	{
		led[i].Set(false, false, false);
	}
}

void SuperPod::InitAnalogControls()
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
    cfg[8].InitSingle(seed.GetPin(PIN_KNOB_8));
    cfg[9].InitSingle(seed.GetPin(PIN_KNOB_9));
    cfg[10].InitSingle(seed.GetPin(PIN_KNOB_10));
    cfg[11].InitSingle(seed.GetPin(PIN_KNOB_11));

    seed.adc.Init(cfg, NUM_KNOBS);
    // Make an array of pointers to the knob.
    for(int i = 0; i < NUM_KNOBS; i++)
    {
        knob[i].Init(seed.adc.GetPtr(i), AudioCallbackRate());
    }

    // knob[0].Init(seed.GetPin(PIN_KNOB_0));
    // knob[1].Init(seed.GetPin(PIN_KNOB_1));
    // knob[2].Init(seed.GetPin(PIN_KNOB_2));
    // knob[3].Init(seed.GetPin(PIN_KNOB_3));
    // knob[4].Init(seed.GetPin(PIN_KNOB_4));
    // knob[5].Init(seed.GetPin(PIN_KNOB_5));
    // knob[6].Init(seed.GetPin(PIN_KNOB_6));
    // knob[7].Init(seed.GetPin(PIN_KNOB_7));
    // knob[8].Init(seed.GetPin(PIN_KNOB_8));
    // knob[9].Init(seed.GetPin(PIN_KNOB_9));
    // knob[10].Init(seed.GetPin(PIN_KNOB_10));
    // knob[11].Init(seed.GetPin(PIN_KNOB_11));
}
