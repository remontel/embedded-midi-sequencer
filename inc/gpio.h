#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file gpio.h
 * @brief Low-level project GPIO helper interface.
 *
 * This module provides project-wide GPIO initialization and helper
 * functions for track LED control and other shared digital I/O tasks.
 */

/**
 * @brief Initialize project-wide GPIO resources.
 *
 * Enables required GPIO ports and configures digital pins used by the project.
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
 * @brief Blink the LED corresponding to a track.
 *
 * @param track Track index (0 to 3).
 */
void GPIO_BlinkTrackLED(uint8_t track);

#endif