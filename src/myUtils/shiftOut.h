// shiftOut.h
#pragma once
#ifndef DSY_SHIFTOUT_H
#define DSY_SHIFTOUT_H

//#include "daisy_core.h"
#include "per/gpio.h"

#define LSBFIRST 0
#define MSBFIRST 1

void shiftOut(const dsy_gpio *data, const dsy_gpio *clock, uint8_t bitOrder, uint8_t val);

#endif