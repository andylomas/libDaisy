// super_pod.h
#pragma once
#ifndef SUPER_POD_H
#define SUPER_POD_H /**< & */

#include "daisy_seed.h"
#include "myUtils/myEncoder.h"
#include "myUtils/myRgbLed.h"

namespace daisy
{
/**
   @brief Helpers and hardware definitions for daisy petal.
   @ingroup boards
*/

#define NUM_SWITCHES 4
#define NUM_BUTTONS 4
#define NUM_KNOBS 12
#define NUM_ENCODERS 2
#define NUM_LEDS 2

class SuperPod
{
  public:
    /** Constructor */
    SuperPod() {}
    /** Destructor */
    ~SuperPod() {}

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

    /** Process digital controls */
    void ProcessDigitalControls();

    /** Turn all leds off */
    void ClearLeds();

    /** Update Leds to values you had set. */
    void UpdateLeds();

    /**
       Set any LED
       \param idx Led Index
     */
    void SetLed(uint8_t idx, bool val);
    void SetLed(uint8_t idx, bool r, bool g, bool b);
    void SetLedValue(uint8_t idx, int val);

    DaisySeed seed;    /**< & */

    AnalogControl knob[NUM_KNOBS];
    Switch        switches[NUM_SWITCHES];
    Switch        buttons[NUM_BUTTONS];
    MyEncoder     encoder[NUM_ENCODERS];
    MyRgbLed      led[NUM_LEDS];

  private:
    void InitSwitches();
    void InitEncoders();
    void InitLeds();
    void InitAnalogControls();

    inline uint16_t* adc_ptr(const uint8_t chn) { return seed.adc.GetPtr(chn); }
};

} // namespace daisy

#endif
