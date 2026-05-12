#ifndef DISPLAY7SEG_H
#define DISPLAY7SEG_H

#include <stdint.h>

/**
 * @file display7seg.h
 * @brief 7-segment display interface for BPM output.
 *
 * This module controls the 4-digit 7-segment display used to show
 * the current tempo value in BPM.
 *
 * The display is refreshed by multiplexing one digit at a time, so
 * Display7Seg_ShowNumber() must be called repeatedly from the foreground
 * loop to maintain a stable visible output.
 * 
 * @author Ignacio Martinez-Laparra, Rene Montelongo
 */

/**
 * @brief Initialize the 7-segment display hardware.
 */
void Display7Seg_Init(void);

/**
 * @brief Display a numeric value on the 7-segment display.
 *
 * Performs one multiplexing pass across the four digits. Values above 9999
 * are clamped before display.
 *
 * @param value Value to display.
 */
void Display7Seg_ShowNumber(uint16_t value);

#endif
