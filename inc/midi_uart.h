#ifndef MIDI_UART_H
#define MIDI_UART_H

#include <stdint.h>

/**
 * @file midi_uart.h
 * @brief UART-based MIDI transmission interface.
 *
 * This module initializes UART0 for transmitting MIDI-style byte messages
 * from the TM4C123 LaunchPad to a computer over the debug USB serial port.
 *
 * Hardware connection used by this project:
 *   PA1 = UART0 TX
 *
 * Demo configuration:
 *   Baud rate = 115200 baud
 *
 * Note:
 *   Standard 5-pin DIN MIDI normally uses 31250 baud. This project uses
 *   115200 baud so the LaunchPad USB debug serial connection can send raw
 *   MIDI bytes to a computer-side serial-to-MIDI bridge for Ableton Live or
 *   another MIDI receiver.
 */

/**
 * @brief Initialize UART0 for MIDI byte transmission.
 *
 * Configures PA1 as UART0 TX and sets UART0 to 115200 baud,
 * 8 data bits, no parity, and 1 stop bit.
 */
void MIDI_UART_Init(void);

/**
 * @brief Send a single raw MIDI byte over UART0.
 *
 * This function blocks until there is room in the UART transmit FIFO.
 *
 * @param data Byte to transmit.
 */
void MIDI_UART_SendByte(uint8_t data);

/**
 * @brief Send a MIDI Note On message.
 *
 * Sends three bytes:
 *   status byte = 0x90 | channel
 *   data byte 1 = note
 *   data byte 2 = velocity
 *
 * @param channel  MIDI channel number, 0 to 15.
 * @param note     MIDI note number, 0 to 127.
 * @param velocity MIDI velocity value, 0 to 127.
 */
void MIDI_UART_SendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);

/**
 * @brief Send a MIDI Note Off message.
 *
 * Sends three bytes:
 *   status byte = 0x80 | channel
 *   data byte 1 = note
 *   data byte 2 = velocity
 *
 * @param channel  MIDI channel number, 0 to 15.
 * @param note     MIDI note number, 0 to 127.
 * @param velocity MIDI release velocity value, 0 to 127.
 */
void MIDI_UART_SendNoteOff(uint8_t channel, uint8_t note, uint8_t velocity);

#endif