/**
 * @file keypad.c
 * @brief 4x4 keypad scanning and key detection.
 *
 * This file implements the keypad scanning logic for the EduBase 4x4 keypad.
 * It detects key presses, maps them to sequencer step indices, and provides
 * clean step input events to the application.
 *
 * Hardware note:
 * The keypad shares PD0-PD3 with EduBase pushbuttons and PA2-PA5 with the
 * LCD data bus, so this module includes guard logic to avoid false keypad
 * detections when the shared row lines are already high.
 *
 * @author Ignacio Martinez-Laparra, Rene Montelongo
 */

#include "TM4C123GH6PM.h"
#include "keypad.h"
#include "SysTick_Delay.h"

// Rows: PD0-PD3
#define ROW0_MASK   0x01
#define ROW1_MASK   0x02
#define ROW2_MASK   0x04
#define ROW3_MASK   0x08
#define ROW_MASK    0x0F

// Columns: PA2-PA5
#define COL0_MASK   0x04
#define COL1_MASK   0x08
#define COL2_MASK   0x10
#define COL3_MASK   0x20
#define COL_MASK    0x3C

#define KEYPAD_DEBOUNCE_COUNT   2

int8_t last_key;
unsigned char key_count;
unsigned char key_lock;

void Keypad_Init(void)
{
    // Enable clock for Port A and Port D
    SYSCTL->RCGCGPIO |= 0x09;

    // Port A: columns
    GPIOA->DIR |= COL_MASK;        // PA2-PA5 output
    GPIOA->AFSEL &= ~COL_MASK;     // GPIO function
    GPIOA->DEN |= COL_MASK;        // digital enable
    GPIOA->AMSEL &= ~COL_MASK;     // disable analog
    GPIOA->DATA &= ~COL_MASK;      // all columns low

    // Port D: rows
    GPIOD->DIR &= ~ROW_MASK;       // PD0-PD3 input
    GPIOD->AFSEL &= ~ROW_MASK;     // GPIO function
    GPIOD->DEN |= ROW_MASK;        // digital enable
    GPIOD->AMSEL &= ~ROW_MASK;     // disable analog
    GPIOD->PDR |= ROW_MASK;        // weak pull-down resistors

    last_key = -1;
    key_count = 0;
    key_lock = 0;
}

int8_t Keypad_Scan(void)
{
    int8_t raw_key;
    unsigned char row_data;

    raw_key = -1;

    // Turn all columns off before checking rows
    GPIOA->DATA &= ~COL_MASK;
    SysTick_Delay1us(50);

    /*
     * If any row is already high while all columns are off,
     * that is likely one of the EduBase SW2-SW5 buttons,
     * not a keypad press. Ignore keypad scanning in that case.
     */
    if ((GPIOD->DATA & ROW_MASK) != 0)
    {
        last_key = -1;
        key_count = 0;
        key_lock = 0;
        return -1;
    }

    // Scan Column 0
    GPIOA->DATA &= ~COL_MASK;
    GPIOA->DATA |= COL0_MASK;
    SysTick_Delay1us(50);
    row_data = GPIOD->DATA & ROW_MASK;

    if ((row_data & ROW0_MASK) != 0) { raw_key = 0; }
    else if ((row_data & ROW1_MASK) != 0) { raw_key = 4; }
    else if ((row_data & ROW2_MASK) != 0) { raw_key = 8; }
    else if ((row_data & ROW3_MASK) != 0) { raw_key = 12; }

    // Scan Column 1
    if (raw_key == -1)
    {
        GPIOA->DATA &= ~COL_MASK;
        GPIOA->DATA |= COL1_MASK;
        SysTick_Delay1us(50);
        row_data = GPIOD->DATA & ROW_MASK;

        if ((row_data & ROW0_MASK) != 0) { raw_key = 1; }
        else if ((row_data & ROW1_MASK) != 0) { raw_key = 5; }
        else if ((row_data & ROW2_MASK) != 0) { raw_key = 9; }
        else if ((row_data & ROW3_MASK) != 0) { raw_key = 13; }
    }

    // Scan Column 2
    if (raw_key == -1)
    {
        GPIOA->DATA &= ~COL_MASK;
        GPIOA->DATA |= COL2_MASK;
        SysTick_Delay1us(50);
        row_data = GPIOD->DATA & ROW_MASK;

        if ((row_data & ROW0_MASK) != 0) { raw_key = 2; }
        else if ((row_data & ROW1_MASK) != 0) { raw_key = 6; }
        else if ((row_data & ROW2_MASK) != 0) { raw_key = 10; }
        else if ((row_data & ROW3_MASK) != 0) { raw_key = 14; }
    }

    // Scan Column 3
    if (raw_key == -1)
    {
        GPIOA->DATA &= ~COL_MASK;
        GPIOA->DATA |= COL3_MASK;
        SysTick_Delay1us(50);
        row_data = GPIOD->DATA & ROW_MASK;

        if ((row_data & ROW0_MASK) != 0) { raw_key = 3; }
        else if ((row_data & ROW1_MASK) != 0) { raw_key = 7; }
        else if ((row_data & ROW2_MASK) != 0) { raw_key = 11; }
        else if ((row_data & ROW3_MASK) != 0) { raw_key = 15; }
    }

    // Turn all columns off after scan
    GPIOA->DATA &= ~COL_MASK;

    // One-shot behavior: do not repeat while held
    if (key_lock == 1)
    {
        if (raw_key == -1)
        {
            key_lock = 0;
            last_key = -1;
            key_count = 0;
        }

        return -1;
    }

    // Basic debounce
    if (raw_key == -1)
    {
        last_key = -1;
        key_count = 0;
        return -1;
    }

    if (raw_key == last_key)
    {
        key_count++;

        if (key_count >= KEYPAD_DEBOUNCE_COUNT)
        {
            key_lock = 1;
            key_count = 0;
            last_key = -1;
            return raw_key;
        }
    }
    else
    {
        last_key = raw_key;
        key_count = 1;
    }

    return -1;
}
