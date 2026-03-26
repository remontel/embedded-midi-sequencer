#ifndef DISPLAY7SEG_H
#define DISPLAY7SEG_H

#include <stdint.h>

/**
 * @file display7seg.h
 * @brief 7-segment display interface for BPM output.
 *
 * This module controls the 4-digit 7-segment display used to show
 * the current tempo value in BPM.
 */

/**
 * @brief Initialize the 7-segment display hardware.
 */
void Display7Seg_Init(void);

/**
 * @brief Display a numeric value on the 7-segment display.
 *
 * @param value Value to display.
 */
void Display7Seg_ShowNumber(uint16_t value);

#endif