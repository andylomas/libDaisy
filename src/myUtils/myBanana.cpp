// myBanana.cpp
#include "myUtils/myBanana.h"
#include "per/dac.h"

using namespace daisy;

float MyBanana::Value()
{
    if (config.mode == ANALOG_INPUT)
    {
        return analog_input.Value();
    }
}

void MyBanana::SetValue(float val) {
    if (config.mode == ANALOG_OUTPUT)
    {
        int int_val = (val < 0.f) ? 0 : (val > 1.f) ? 4095 : (int) (4095.f * val);
        if (dac_chn == 1)
            dsy_dac_write(DSY_DAC_CHN1, int_val);
        else
            dsy_dac_write(DSY_DAC_CHN2, int_val);
    }
}