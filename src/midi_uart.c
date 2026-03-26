/**
 * @file midi_uart.c
 * @brief UART-based MIDI transmission driver.
 *
 * This file initializes the UART peripheral for MIDI message transmission
 * and provides helper functions for sending raw MIDI bytes and formatted
 * MIDI note messages such as Note On and Note Off events.
 */

#include "TM4C123GH6PM.h"
#include "midi_uart.h"

void MIDI_UART_Init(void)
{
    /* TODO: Configure UART peripheral and GPIO pins for MIDI transmission */
}

void MIDI_UART_SendByte(uint8_t data)
{
    (void)data;
    /* TODO: Wait for TX ready and transmit byte */
}

void MIDI_UART_SendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
    MIDI_UART_SendByte((uint8_t)(0x90U | (channel & 0x0FU)));
    MIDI_UART_SendByte(note);
    MIDI_UART_SendByte(velocity);
}

void MIDI_UART_SendNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
    MIDI_UART_SendByte((uint8_t)(0x80U | (channel & 0x0FU)));
    MIDI_UART_SendByte(note);
    MIDI_UART_SendByte(velocity);
}