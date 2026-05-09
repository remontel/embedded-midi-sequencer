/**
 * @file display7seg.c
 * @brief 7-segment display driver for BPM visualization.
 *
 * This file controls the 4-digit 7-segment display and provides functions
 * for showing the current tempo value in BPM.
 */

#include "TM4C123GH6PM.h"
#include "display7seg.h"
#include "SysTick_Delay.h"

const uint8_t number_pattern[16] =
{
    0xC0, /* 0 */
    0xF9, /* 1 */
    0xA4, /* 2 */
    0xB0, /* 3 */
    0x99, /* 4 */
    0x92, /* 5 */
    0x82, /* 6 */
    0xF8, /* 7 */
    0x80, /* 8 */
    0x98, /* 9 */
    0x88, /* A */
    0x83, /* B */
    0xC6, /* C */
    0xA1, /* D */
    0x86, /* E */
    0x8E  /* F */
};

void SSI2_Write(uint8_t data)
{
    GPIOC->DATA &= ~0x80;
    SSI2->DR = data;

    while (SSI2->SR & 0x10)
    {
    }

    GPIOC->DATA |= 0x80;
}

void Display7Seg_Init(void)
{
    SYSCTL->RCGCSSI |= 0x04;
    SYSCTL->RCGCGPIO |= 0x02;
    SYSCTL->RCGCGPIO |= 0x04;

    /* PB4 = SSI2Clk, PB7 = SSI2Tx */
    GPIOB->AFSEL |= 0x90;
    GPIOB->PCTL &= ~0xF00F0000;
    GPIOB->PCTL |= 0x20020000;
    GPIOB->DEN |= 0x90;
    GPIOB->AMSEL &= ~0x90;

    /* PC7 = slave select */
    GPIOC->DIR |= 0x80;
    GPIOC->AFSEL &= ~0x80;
    GPIOC->DEN |= 0x80;
    GPIOC->AMSEL &= ~0x80;
    GPIOC->DATA |= 0x80;

    SSI2->CR1 &= ~0x02;
    SSI2->CR1 &= ~0x01;
    SSI2->CR1 &= ~0x04;

    /* Use PIOSC */
    SSI2->CC = 0x05;

    /* 16 MHz / 16 = 1 MHz */
    SSI2->CPSR = 16;
    SSI2->CR0 &= ~0xFF00;

    SSI2->CR0 &= ~0x0080;
    SSI2->CR0 &= ~0x0040;
    SSI2->CR0 &= ~0x0030;
    SSI2->CR0 |= 0x0007;

    SSI2->CR1 |= 0x02;
}

void Display7Seg_ShowNumber(uint16_t value)
{
    uint8_t ones;
    uint8_t tens;
    uint8_t hundreds;
    uint8_t thousands;

    if (value > 9999)
    {
        value = 9999;
    }

    ones = value % 10;
    tens = (value / 10) % 10;
    hundreds = (value / 100) % 10;
    thousands = (value / 1000) % 10;

    SSI2_Write(number_pattern[ones]);
    SSI2_Write(1);
    SysTick_Delay1ms(1);

    SSI2_Write(number_pattern[tens]);
    SSI2_Write(2);
    SysTick_Delay1ms(1);

    SSI2_Write(number_pattern[hundreds]);
    SSI2_Write(4);
    SysTick_Delay1ms(1);

    SSI2_Write(number_pattern[thousands]);
    SSI2_Write(8);
    SysTick_Delay1ms(1);
}
