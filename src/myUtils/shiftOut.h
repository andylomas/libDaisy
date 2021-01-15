// shiftOut.h
#pragma once
#ifndef DSY_SHIFTOUT_H
#define DSY_SHIFTOUT_H

#include "per/gpio.h"

#define LSBFIRST 0
#define MSBFIRST 1

void shiftOut(const dsy_gpio *data, const dsy_gpio *clock, uint8_t bit_order, uint8_t val, uint8_t num_bits = 8);
void shiftOutBuffer(const dsy_gpio *data, const dsy_gpio *clock, uint8_t bit_order, uint8_t *vals, uint16_t num_bytes);

#endif