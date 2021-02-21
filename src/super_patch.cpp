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

#define PIN_DAC_0 24
#define PIN_DAC_1 25

#define PIN_OLED_RES 6
#define PIN_OLED_DC 9

#define PIN_LED_SCL 11
#define PIN_LED_SDA 12

#define PIN_BANANA_0 23
#define PIN_BANANA_1 25
#define PIN_BANANA_2 22
#define PIN_BANANA_3 24
#define PIN_BANANA_4 13
#define PIN_BANANA_5 14

void SuperPatch::Init(BananaConfig *banana_config)
{
    // Set Some numbers up for accessors.
    // Initialize the hardware.
    seed.Configure();
    seed.Init();
    dsy_tim_start();
    InitBananas(banana_config);
    InitSwitches();
    InitEncoders();
    InitAnalogControls();
    InitDac();
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

void SuperPatch::StartDac()
{
    dsy_dac_start(DSY_DAC_CHN_BOTH);
}

void SuperPatch::ProcessAnalogControls()
{
    for(size_t i = 0; i < NUM_KNOBS; i++)
    {
        knob[i].Process();
    }

    for (uint8_t i = 0; i < num_banana_adcs; i++)
    {
        uint8_t banana_index = banana_adc_list[i];
        banana[banana_index].analog_input.Process();
    }
}

// void SuperPatch::SetDacOutRaw1(uint16_t val)
// {
//     // Assumes that we're using 12bit DAC, so range is 0-4095
//     val = (val < 0.) ? 0 : (val > 4095) ? 4095 : val;
//     dsy_dac_write(DSY_DAC_CHN1, val);
// }

// void SuperPatch::SetDacOut1(float val)
// {  
//     // Assumes that we're using 12bit DAC, so range is 0-4095
//     SetDacOutRaw1((int) (4095.f * val));
// }

// void SuperPatch::SetDacOutRaw2(uint16_t val)
// {
//     // Assumes that we're using 12bit DAC, so range is 0-4095
//     val = (val < 0.) ? 0 : (val > 4095) ? 4095 : val;
//     dsy_dac_write(DSY_DAC_CHN2, val);
// }

// void SuperPatch::SetDacOut2(float val)
// {  
//     // Assumes that we're using 12bit DAC, so range is 0-4095
//     SetDacOutRaw2((int) (4095.f * val));
//}

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

void SuperPatch::InitBananas(BananaConfig *banana_config)
{
    // Note: this should be done before initializing
    // other peripherals. In particular all the ADCs
    // need configuring together, so should be done after
    // InitBananas.

    uint8_t banana_pin_numbers[NUM_BANANAS] = {
        PIN_BANANA_0,
        PIN_BANANA_1,
        PIN_BANANA_2,
        PIN_BANANA_3,
        PIN_BANANA_4,
        PIN_BANANA_5
    };

    banana_adc_list.clear();

    for (int i = 0; i < NUM_BANANAS; i++)
    {
        if (banana_config == NULL)
        {
            banana[i].config = {OFF, 0};
        }
        else
        {
            banana[i].config = banana_config[i];

            switch (banana[i].config.mode)
            {
            case DIGITAL_INPUT:
                // Any banana can be used as a digital input
                banana[i].gpio.pin = seed.GetPin(banana_pin_numbers[i]);
                banana[i].gpio.mode = DSY_GPIO_MODE_INPUT;
                banana[i].gpio.pull = DSY_GPIO_NOPULL;
                dsy_gpio_init(&banana[i].gpio);
                break;

            case DIGITAL_OUTPUT:
                // Any banana can be used as a digital output
                banana[i].gpio.pin = seed.GetPin(banana_pin_numbers[i]);
                banana[i].gpio.mode = DSY_GPIO_MODE_OUTPUT_PP;
                banana[i].gpio.pull = DSY_GPIO_NOPULL;
                dsy_gpio_init(&banana[i].gpio);
                break;

            case ANALOG_INPUT:
                // Only bananas 0-3 can be used as analog inputs
                if (i <= 3)
                {
                    // Need to setup data to be used in InitAnalogControls() to configure ADCs
                    banana_adc_list.push_back(i);
                    banana[i].gpio.pin = seed.GetPin(banana_pin_numbers[i]);
                }
                else
                {
                    // Should issue some sort of error!
                    banana[i].config.mode = OFF;
                }
                break;

            case ANALOG_OUTPUT:
                // Only bananas 0 and 2 can be analog outputs
                if (i == 0 || i == 2)
                {
                    // In InitDac() we check which bananas are being used as analog outputs
                    // Don't need to do anything here as the mode has already been set
                    banana[i].dac_chn = (i == 0) ? 1 : 2;                    
                }
                else
                {
                    // Should issue some sort of error!
                    banana[i].config.mode = OFF;
                }
                break;

            case SERIAL_INPUT:
                // Only banana 5 can be used for serial input
                if (i == 5)
                {

                }
                else
                {
                    // Should issue some sort of error!
                    banana[i].config.mode = OFF;
                } 
                break;

            case SERIAL_OUTPUT:
                // Only banana 4 can be used for serial output
                if (i == 4)
                {

                }
                else
                {
                    // Should issue some sort of error!
                    banana[i].config.mode = OFF;
                }                 break;

            case MIDI_INPUT:
                // Only banana 5 can be used for MIDI input
                if (i == 5)
                {

                }
                else
                {
                    // Should issue some sort of error!
                    banana[i].config.mode = OFF;
                }                 break;

            case MIDI_OUTPUT:
                // Only banana 4 can be used for MIDI output
                if (i == 4)
                {

                }
                else
                {
                    // Should issue some sort of error!
                    banana[i].config.mode = OFF;
                } 
                break;

            case OFF:
                break;
            }
        }
    }
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
    // Set order of ADCs based on CHANNEL NUMBER + ADCs needed for Banana plugs
    num_banana_adcs = banana_adc_list.size();
    AdcChannelConfig cfg[NUM_KNOBS + num_banana_adcs];

    // Init with Single Pins
    cfg[0].InitSingle(seed.GetPin(PIN_KNOB_0));
    cfg[1].InitSingle(seed.GetPin(PIN_KNOB_1));
    cfg[2].InitSingle(seed.GetPin(PIN_KNOB_2));
    cfg[3].InitSingle(seed.GetPin(PIN_KNOB_3));
    cfg[4].InitSingle(seed.GetPin(PIN_KNOB_4));
    cfg[5].InitSingle(seed.GetPin(PIN_KNOB_5));
    cfg[6].InitSingle(seed.GetPin(PIN_KNOB_6));
    cfg[7].InitSingle(seed.GetPin(PIN_KNOB_7));

    // Init cfg for ADCs needed by banana plugs
    for (uint8_t i = 0; i < num_banana_adcs; i++)
    {
        uint8_t banana_index = banana_adc_list[i];
        cfg[NUM_KNOBS + i].InitSingle(banana[banana_index].gpio.pin);
    }

    seed.adc.Init(cfg, NUM_KNOBS + num_banana_adcs);

    // Make an array of pointers to the knob.
    for(int i = 0; i < NUM_KNOBS; i++)
    {
        knob[i].Init(seed.adc.GetPtr(i), AudioCallbackRate());
    }

    // Get pointers to any AnalogControls needed by bananas
    for (uint8_t i = 0; i < num_banana_adcs; i++)
    {
        uint8_t banana_index = banana_adc_list[i];
        banana[banana_index].analog_input.Init(seed.adc.GetPtr(NUM_KNOBS + i), AudioCallbackRate());
    }
}

void SuperPatch::InitDac()
{
    bool dac1_used = (banana[0].config.mode == ANALOG_OUTPUT);
    bool dac2_used = (banana[2].config.mode == ANALOG_OUTPUT);

    if (dac1_used & dac2_used)
    {
        dsy_dac_init(&seed.dac_handle, DSY_DAC_CHN_BOTH);
    }
    else if (dac1_used)
    {
        dsy_dac_init(&seed.dac_handle, DSY_DAC_CHN1);
    }
    else if (dac2_used)
    {
        dsy_dac_init(&seed.dac_handle, DSY_DAC_CHN2);
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
