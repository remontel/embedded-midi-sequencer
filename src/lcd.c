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
#include "SysTick_Delay.h"

/* LCD pin mapping on EduBase
 * RS  -> PE0
 * E   -> PC6
 * DB4 -> PA2
 * DB5 -> PA3
 * DB6 -> PA4
 * DB7 -> PA5
 */

#define LCD_RS_MASK   0x01
#define LCD_E_MASK    0x40
#define LCD_DATA_MASK 0x3C

void LCD_Ports_Init(void)
{
    /* Enable clocks for Port A, C, and E */
    SYSCTL->RCGCGPIO |= 0x15;

    /* PA2-PA5 = LCD data pins */
    GPIOA->DIR |= LCD_DATA_MASK;
    GPIOA->AFSEL &= ~LCD_DATA_MASK;
    GPIOA->DEN |= LCD_DATA_MASK;
    GPIOA->AMSEL &= ~LCD_DATA_MASK;

    /* PC6 = LCD Enable */
    GPIOC->DIR |= LCD_E_MASK;
    GPIOC->AFSEL &= ~LCD_E_MASK;
    GPIOC->DEN |= LCD_E_MASK;
    GPIOC->AMSEL &= ~LCD_E_MASK;

    /* PE0 = LCD RS */
    GPIOE->DIR |= LCD_RS_MASK;
    GPIOE->AFSEL &= ~LCD_RS_MASK;
    GPIOE->DEN |= LCD_RS_MASK;
    GPIOE->AMSEL &= ~LCD_RS_MASK;

    /* Start with control lines low */
    GPIOC->DATA &= ~LCD_E_MASK;
    GPIOE->DATA &= ~LCD_RS_MASK;
}

void LCD_Pulse_Enable(void)
{
    GPIOC->DATA |= LCD_E_MASK;
    SysTick_Delay1us(1);
    GPIOC->DATA &= ~LCD_E_MASK;
    SysTick_Delay1us(100);
}

void LCD_Write_4_Bits(uint8_t nibble)
{
    GPIOA->DATA &= ~LCD_DATA_MASK;
    GPIOA->DATA |= ((nibble & 0x0F) << 2);
    LCD_Pulse_Enable();
}

void LCD_Send_Command(uint8_t command)
{
    GPIOE->DATA &= ~LCD_RS_MASK;

    LCD_Write_4_Bits(command >> 4);
    LCD_Write_4_Bits(command & 0x0F);

    if ((command == 0x01) || (command == 0x02))
    {
        SysTick_Delay1ms(2);
    }
    else
    {
        SysTick_Delay1us(50);
    }
}

void LCD_Send_Data(uint8_t data)
{
    GPIOE->DATA |= LCD_RS_MASK;

    LCD_Write_4_Bits(data >> 4);
    LCD_Write_4_Bits(data & 0x0F);

    SysTick_Delay1us(50);
}

void LCD_Init(void)
{
    LCD_Ports_Init();

    SysTick_Delay1ms(20);

    GPIOE->DATA &= ~LCD_RS_MASK;

    /* Initialize LCD into 4-bit mode */
    LCD_Write_4_Bits(0x03);
    SysTick_Delay1ms(5);

    LCD_Write_4_Bits(0x03);
    SysTick_Delay1us(200);

    LCD_Write_4_Bits(0x03);
    SysTick_Delay1us(200);

    LCD_Write_4_Bits(0x02);
    SysTick_Delay1us(200);

    /* Function set: 4-bit, 2-line, 5x8 font */
    LCD_Send_Command(0x28);

    /* Display on, cursor off, blink off */
    LCD_Send_Command(0x0C);

    /* Entry mode: increment, no shift */
    LCD_Send_Command(0x06);

    /* Clear display */
    LCD_Send_Command(0x01);
}

void LCD_Clear(void)
{
    LCD_Send_Command(0x01);
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t address;

    if (row == 0U)
    {
        address = 0x80 + col;
    }
    else
    {
        address = 0xC0 + col;
    }

    LCD_Send_Command(address);
}

void LCD_WriteChar(char c)
{
    LCD_Send_Data((uint8_t)c);
}

void LCD_WriteString(const char *str)
{
    while (*str != '\0')
    {
        LCD_WriteChar(*str);
        str++;
    }
}

void LCD_DisplayTrackSteps(uint8_t track, uint8_t current_step)
{
    uint8_t step;
    char c;

    /* Row 0: steps 0 to 7 */
    LCD_SetCursor(0U, 0U);
    for (step = 0U; step < 8U; step++)
    {
        if (Sequencer_GetStepState(track, step) != 0U)
        {
            c = 'X';
        }
        else
        {
            c = '.';
        }

        if (step == current_step)
        {
            if (c == 'X')
            {
                c = '*';
            }
            else
            {
                c = '-';
            }
        }

        LCD_WriteChar(c);
        LCD_WriteChar(' ');
    }

    /* Row 1: steps 8 to 15 */
    LCD_SetCursor(1U, 0U);
    for (step = 8U; step < 16U; step++)
    {
        if (Sequencer_GetStepState(track, step) != 0U)
        {
            c = 'X';
        }
        else
        {
            c = '.';
        }

        if (step == current_step)
        {
            if (c == 'X')
            {
                c = '*';
            }
            else
            {
                c = '-';
            }
        }

        LCD_WriteChar(c);
        LCD_WriteChar(' ');
    }
}
