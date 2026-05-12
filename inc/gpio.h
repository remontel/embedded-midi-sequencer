#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file gpio.h
 * @brief Low-level project GPIO helper interface.
 *
 * This module provides LED-specific GPIO initialization and helper
 * functions for the four EduBase track LEDs.
 * 
 * @author Ignacio Martinez-Laparra, Rene Montelongo
 */

/**
 * @brief Initialize GPIO resources used by the track LEDs.
 *
 * Enables Port B and configures PB0-PB3 as digital outputs.
 */
void GPIO_ProjectInit(void);

/**
 * @brief Set a track LED ON or OFF.
 *
 * @param track Track index (0 to 3).
 * @param on    true to turn LED on, false to turn LED off.
 */
void GPIO_SetTrackLED(uint8_t track, bool on);

/**
 * @brief Turn all track LEDs OFF.
 */
void GPIO_ClearAllTrackLEDs(void);

/**
 * @brief Pulse the LED corresponding to a track.
 *
 * This helper turns the LED on and then immediately back off. It does not
 * insert a visible delay, so the observed pulse width depends on the caller.
 *
 * @param track Track index (0 to 3).
 */
void GPIO_BlinkTrackLED(uint8_t track);

#endif
