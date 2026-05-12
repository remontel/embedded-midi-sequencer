#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

/**
 * @file keypad.h
 * @brief 4x4 keypad scanning interface.
 *
 * This module scans the EduBase 4x4 keypad, applies basic debounce,
 * and returns the pressed key index used for sequencer step editing.
 *
 * Hardware note:
 * Keypad rows share PD0-PD3 with the EduBase SW2-SW5 buttons, and keypad
 * columns share PA2-PA5 with the LCD data bus. The scan logic therefore
 * suppresses keypad detection when the shared row lines already appear active.
 * 
 * @author Ignacio Martinez-Laparra, Rene Montelongo
 */

/**
 * @brief Initialize keypad GPIO lines.
 */
void Keypad_Init(void);

/**
 * @brief Scan the keypad and return the active key.
 *
 * Performs a one-shot scan with basic debounce. A held key is reported once
 * and will not repeat until it is released and pressed again.
 *
 * @return -1 if no key is pressed, otherwise 0 to 15 for the detected key.
 */
int8_t Keypad_Scan(void);

#endif
