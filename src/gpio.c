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

/* EduBase LEDs: PB0-PB3 */
#define LED0_MASK   0x01
#define LED1_MASK   0x02
#define LED2_MASK   0x04
#define LED3_MASK   0x08
#define ALL_LED_MASK 0x0F

void GPIO_ProjectInit(void)
{
    /* Enable clock for Port B */
    SYSCTL->RCGCGPIO |= 0x02;

    /* Configure PB0-PB3 as GPIO outputs for track LEDs */
    GPIOB->DIR |= ALL_LED_MASK;
    GPIOB->AFSEL &= ~ALL_LED_MASK;
    GPIOB->DEN |= ALL_LED_MASK;
    GPIOB->AMSEL &= ~ALL_LED_MASK;

    /* Start with all LEDs off */
    GPIOB->DATA &= ~ALL_LED_MASK;
}

void GPIO_SetTrackLED(uint8_t track, bool on)
{
    switch (track)
    {
        case 0:
            if (on)
            {
                GPIOB->DATA |= LED0_MASK;
            }
            else
            {
                GPIOB->DATA &= ~LED0_MASK;
            }
            break;

        case 1:
            if (on)
            {
                GPIOB->DATA |= LED1_MASK;
            }
            else
            {
                GPIOB->DATA &= ~LED1_MASK;
            }
            break;

        case 2:
            if (on)
            {
                GPIOB->DATA |= LED2_MASK;
            }
            else
            {
                GPIOB->DATA &= ~LED2_MASK;
            }
            break;

        case 3:
            if (on)
            {
                GPIOB->DATA |= LED3_MASK;
            }
            else
            {
                GPIOB->DATA &= ~LED3_MASK;
            }
            break;

        default:
            break;
    }
}

void GPIO_ClearAllTrackLEDs(void)
{
    GPIOB->DATA &= ~ALL_LED_MASK;
}

void GPIO_BlinkTrackLED(uint8_t track)
{
    GPIO_SetTrackLED(track, true);
    GPIO_SetTrackLED(track, false);
}
