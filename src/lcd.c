/**
 * @file lcd.c
 * @brief LCD driver and sequencer display helper functions.
 *
 * This file initializes and controls the 16x2 LCD used by the sequencer.
 * It provides helper functions for writing characters and strings and for
 * displaying the current sequencer pattern and playback position.
 */

#include "TM4C123GH6PM.h"
#include "lcd.h"
#include "sequencer.h"

void LCD_Init(void)
{
    /* TODO: Initialize LCD interface */
}

void LCD_Clear(void)
{
    /* TODO: Clear LCD and return cursor home */
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    (void)row;
    (void)col;
    /* TODO: Set LCD cursor position */
}

void LCD_WriteChar(char c)
{
    (void)c;
    /* TODO: Write one character to LCD */
}

void LCD_WriteString(const char *str)
{
    (void)str;
    /* TODO: Write string to LCD */
}

void LCD_DisplayTrackSteps(uint8_t track, uint8_t current_step)
{
    uint8_t step;
    (void)current_step;

    /* TODO: Render selected track pattern on LCD */
    for (step = 0U; step < 16U; step++)
    {
        (void)Sequencer_GetStepState(track, step);
    }
}