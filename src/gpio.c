/**
 * @file gpio.c
 * @brief Low-level GPIO initialization and digital I/O helper functions.
 *
 * This file contains GPIO configuration code and utility functions used by
 * the sequencer system for LEDs, buttons, keypad lines, and other digital
 * signals connected to the TM4C123G and EduBase V2 board.
 */

#include "TM4C123GH6PM.h"
#include "gpio.h"

void GPIO_ProjectInit(void)
{
    /* TODO: Enable and configure all project GPIO ports/pins */
}

void GPIO_SetTrackLED(uint8_t track, bool on)
{
    (void)track;
    (void)on;
    /* TODO: Set specific track LED state */
}

void GPIO_ClearAllTrackLEDs(void)
{
    /* TODO: Turn all track LEDs off */
}

void GPIO_BlinkTrackLED(uint8_t track)
{
    (void)track;
    /* TODO: Blink track LED to indicate note trigger */
}