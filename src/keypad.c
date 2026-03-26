/**
 * @file keypad.c
 * @brief 4x4 keypad scanning and key detection.
 *
 * This file implements the keypad scanning logic for the EduBase 4x4 keypad.
 * It detects key presses, maps them to sequencer step indices, and provides
 * clean step input events to the application.
 */

#include "TM4C123GH6PM.h"
#include "keypad.h"

void Keypad_Init(void)
{
    /* TODO: Configure keypad row/column GPIO pins */
}

int8_t Keypad_Scan(void)
{
    /* TODO: Drive rows, read columns, debounce keypress */
    return -1;
}