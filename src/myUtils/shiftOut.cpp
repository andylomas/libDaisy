// shiftOut.cpp
#include "myUtils/shiftOut.h"

void shiftOut(const dsy_gpio *data, const dsy_gpio *clock, uint8_t bitOrder, uint8_t val)
{
      uint8_t i;

     for (i = 0; i < 8; i++)  {
           if (bitOrder == LSBFIRST)
                 dsy_gpio_write(data, !!(val & (1 << i)));
           else      
                 dsy_gpio_write(data, !!(val & (1 << (7 - i))));
                 
           dsy_gpio_write(clock, 1);
           dsy_gpio_write(clock, 0);            
     }
}
