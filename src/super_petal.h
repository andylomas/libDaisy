// super_petal.h
#pragma once
#ifndef SUPER_PETAL_H
#define SUPER_PETAL_H /**< & */

#include "daisy_seed.h"
#include "myUtils/shiftOut.h"
#include "myUtils/mySwitch.h"
#include "myUtils/myLedController.h"
#include "myUtils/myEncoder.h"
#include "myUtils/myShiftedLCD.h"

namespace daisy
{
/**
   @brief Helpers and hardware definitions for daisy petal.
   @ingroup boards
*/

#define NUM_SWITCHES 4
#define NUM_KNOBS 9
#define NUM_ENCODERS 4

class SuperPetal
{
  public:
    /** Constructor */
    SuperPetal() {}
    /** Destructor */
    ~SuperPetal() {}

    /** Initialize daisy petal */
    void Init();

    /**
       Wait before moving on.
       \param del Delay time in ms.
     */
    void DelayMs(size_t del);

    /** Starts the callback
    \cb Interleaved callback function
    */
    void StartAudio(AudioHandle::InterleavingAudioCallback cb);

    /** Starts the callback
    \cb multichannel callback function
    */
    void StartAudio(AudioHandle::AudioCallback cb);

    /**
       Switch callback functions
       \param cb New interleaved callback function.
    */
    void ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb);

    /**
       Switch callback functions
       \param cb New multichannel callback function.
    */
    void ChangeAudioCallback(AudioHandle::AudioCallback cb);

    /** Stops the audio if it is running. */
    void StopAudio();

    /** Updates the Audio Sample Rate, and reinitializes.
     ** Audio must be stopped for this to work.
     */
    void SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate);

    /** Returns the audio sample rate in Hz as a floating point number.
     */
    float AudioSampleRate();

    /** Sets the number of samples processed per channel by the audio callback.
       \param size Audio block size
     */
    void SetAudioBlockSize(size_t size);

    /** Returns the number of samples per channel in a block of audio. */
    size_t AudioBlockSize();

    /** Returns the rate in Hz that the Audio callback is called */
    float AudioCallbackRate();

    /** Start analog to digital conversion. */
    void StartAdc();

    /** Stops Transfering data from the ADC */
    void StopAdc();

    /** Returns number of audio channels */
    int NumChannels() { return 2; }

    /** Call at the same frequency as controls are read for stable readings.*/
    void ProcessAnalogControls();

    /** Process Analog and Digital Controls */
    inline void ProcessAllControls()
    {
        ProcessAnalogControls();
        ProcessDigitalControls();
    }

    /** Get value per knob.
    \param k Which knob to get
    \return Floating point knob position.
    */
    float GetKnobValue(uint8_t k);

    /** & */
    float GetExpression();

    /** Process digital controls */
    void ProcessDigitalControls();

    /** Turn all leds off */
    void ClearLeds();

    /** Update Leds to values you had set. */
    void UpdateLeds();

    /**
       Set any LED (footswitch 0-1, Encoder 4-7)
       \param idx Led Index
     */
    void SetLed(uint8_t idx, bool val);
    void SetLed(uint8_t idx, bool r, bool g, bool b);
    void SetLedValue(uint8_t idx, int val);

    /**
       Set footswitch LED
       \param idx Led Index
     */
    void SetFootswitchLed(uint8_t idx, bool val);
    void SetFootswitchLed(uint8_t idx, bool r, bool g, bool b);
    void SetFootswitchLedValue(uint8_t idx, int val);

    /**
       Set encoder LED
       \param idx Led Index
     */
    void SetEncoderLed(uint8_t idx, bool val);
    void SetEncoderLed(uint8_t idx, bool r, bool g, bool b);
    void SetEncoderLedValue(uint8_t idx, int val);
    void SetOnboardLed(bool val);
    
    DaisySeed seed;    /**< & */

    AnalogControl knob[NUM_KNOBS]; /**< & */
    AnalogControl expression;      /**< & */
    MySwitch      switches[NUM_SWITCHES] /**< & */;
    MyEncoder     encoder[NUM_ENCODERS];

    MyLedController led_controller;
    LiquidCrystal lcd;

  private:
    void InitSwitches();
    void InitEncoders();
    void InitLeds();
    void InitAnalogControls();
    void InitLcd();

    inline uint16_t* adc_ptr(const uint8_t chn) { return seed.adc.GetPtr(chn); }
};

} // namespace daisy

#endif
