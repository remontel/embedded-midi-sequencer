#ifndef MIDI_UART_H
#define MIDI_UART_H

#include <stdint.h>

/**
 * @file midi_uart.h
 * @brief UART-based MIDI transmission interface.
 *
 * This module initializes the UART peripheral used for MIDI output
 * and provides helper functions for transmitting raw bytes and
 * standard MIDI Note On / Note Off messages.
 */

/**
 * @brief Initialize the UART peripheral for MIDI transmission.
 */
void MIDI_UART_Init(void);

/**
 * @brief Send a single byte over the MIDI UART interface.
 *
 * @param data Byte to transmit.
 */
void MIDI_UART_SendByte(uint8_t data);

/**
 * @brief Send a MIDI Note On message.
 *
 * @param channel  MIDI channel (0 to 15).
 * @param note     MIDI note number.
 * @param velocity MIDI velocity value.
 */
void MIDI_UART_SendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);

/**
 * @brief Send a MIDI Note Off message.
 *
 * @param channel  MIDI channel (0 to 15).
 * @param note     MIDI note number.
 * @param velocity MIDI velocity value.
 */
void MIDI_UART_SendNoteOff(uint8_t channel, uint8_t note, uint8_t velocity);

#endif