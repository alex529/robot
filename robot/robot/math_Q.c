/*
 * math_Q.c
 *
 * Created: 24/04/2015 15:37:40
 *  Author: SavinA
 */ 
#include "common.h"
#include <stdint.h>
/**
 * Calculates the absolute value of a 16bit int.
 * @param x Used to specify int16_t number.
 * @return Returns the absolute of the value.
 */
int16_t int16_abs_Q(int16_t x)
{
    int16_t y = (x >> 15);
    return (x + y) ^ y;
}