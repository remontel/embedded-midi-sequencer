/**
 * @file display7seg.c
 * @brief 7-segment display driver for BPM visualization.
 *
 * This file controls the 4-digit 7-segment display and provides functions
 * for showing the current tempo value in BPM.
 */

#include "TM4C123GH6PM.h"
#include "display7seg.h"

void Display7Seg_Init(void)
{
    /* TODO: Configure GPIO pins and display hardware */
}

void Display7Seg_ShowNumber(uint16_t value)
{
    (void)value;
    /* TODO: Split digits and output number to display */
}