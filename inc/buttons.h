#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdbool.h>

/**
 * @file buttons.h
 * @brief Pushbutton input and debounce interface.
 *
 * This module reads the Tiva LaunchPad and EduBase pushbuttons,
 * applies debounce logic, and provides edge-detected button press
 * events to the rest of the application.
 */

/**
 * @brief Initialize all button-related GPIO pins and internal state.
 */
void Buttons_Init(void);

/**
 * @brief Update button state and debounce logic.
 *
 * This function should be called regularly from the foreground loop.
 */
void Buttons_Update(void);

/**
 * @brief Check whether Tiva SW1 generated a new press event.
 *
 * @return true if pressed since last check, false otherwise.
 */
bool Buttons_WasTivaSW1Pressed(void);

/**
 * @brief Check whether Tiva SW2 generated a new press event.
 *
 * @return true if pressed since last check, false otherwise.
 */
bool Buttons_WasTivaSW2Pressed(void);

/**
 * @brief Check whether EduBase SW2 generated a new press event.
 *
 * @return true if pressed since last check, false otherwise.
 */
bool Buttons_WasEduBaseSW2Pressed(void);

/**
 * @brief Check whether EduBase SW3 generated a new press event.
 *
 * @return true if pressed since last check, false otherwise.
 */
bool Buttons_WasEduBaseSW3Pressed(void);

/**
 * @brief Check whether EduBase SW4 generated a new press event.
 *
 * @return true if pressed since last check, false otherwise.
 */
bool Buttons_WasEduBaseSW4Pressed(void);

/**
 * @brief Check whether EduBase SW5 generated a new press event.
 *
 * @return true if pressed since last check, false otherwise.
 */
bool Buttons_WasEduBaseSW5Pressed(void);

#endif