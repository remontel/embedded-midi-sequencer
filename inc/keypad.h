#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

/**
 * @file keypad.h
 * @brief 4x4 keypad scanning interface.
 *
 * This module scans the EduBase 4x4 keypad, applies basic debounce,
 * and returns the pressed key index used for sequencer step editing.
 */

/**
 * @brief Initialize keypad GPIO lines.
 */
void Keypad_Init(void);

/**
 * @brief Scan the keypad and return the active key.
 *
 * @return -1 if no key is pressed, otherwise 0 to 15 for the detected key.
 */
int8_t Keypad_Scan(void);

#endif