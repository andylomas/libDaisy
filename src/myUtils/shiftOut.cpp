// shiftOut.cpp
#include "myUtils/shiftOut.h"

void shiftOut(const dsy_gpio *data, const dsy_gpio *clock, uint8_t bit_order, uint8_t val, uint8_t num_bits)
{
    for (uint8_t i = 0; i < num_bits; i++)
    {
        if (bit_order == LSBFIRST)
            dsy_gpio_write(data, !!(val & (1 << i)));
        else 
            dsy_gpio_write(data, !!(val & (1 << (num_bits - 1 - i))));
                  
        dsy_gpio_write(clock, 1);
        dsy_gpio_write(clock, 0);
    }
}

void shiftOutBuffer(const dsy_gpio *data, const dsy_gpio *clock, uint8_t bit_order, uint8_t *vals, uint16_t num_bytes)
{
    if (bit_order == LSBFIRST)
    {
        for (uint16_t i = 0; i < num_bytes; i++)
        {
            shiftOut(data, clock, bit_order, *(vals + i));
        }
    }
    else
    {
        for (int16_t i = num_bytes - 1; i >= 0; i--)
        {
            shiftOut(data, clock, bit_order, *(vals + i));
        }
    }  
}
