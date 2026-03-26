#ifndef LCD_H
#define LCD_H

#include <stdint.h>

/**
 * @file lcd.h
 * @brief LCD control and display helper interface.
 *
 * This module initializes and controls the 16x2 LCD used by the project.
 * It provides low-level cursor and character functions as well as a
 * higher-level helper to display the selected track's step pattern.
 */

/**
 * @brief Initialize the LCD hardware.
 */
void LCD_Init(void);

/**
 * @brief Clear the LCD display and reset cursor position.
 */
void LCD_Clear(void);

/**
 * @brief Set the LCD cursor position.
 *
 * @param row Row index (typically 0 or 1).
 * @param col Column index.
 */
void LCD_SetCursor(uint8_t row, uint8_t col);

/**
 * @brief Write a single character to the LCD.
 *
 * @param c Character to display.
 */
void LCD_WriteChar(char c);

/**
 * @brief Write a null-terminated string to the LCD.
 *
 * @param str String to display.
 */
void LCD_WriteString(const char *str);

/**
 * @brief Display the selected track's 16-step pattern.
 *
 * This function is intended as the main MVP display helper.
 * It should show active/inactive steps and indicate the current step.
 *
 * @param track        Currently selected track index.
 * @param current_step Current playback step index.
 */
void LCD_DisplayTrackSteps(uint8_t track, uint8_t current_step);

#endif