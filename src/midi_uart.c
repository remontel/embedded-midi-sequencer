/**
 * @file midi_uart.c
 * @brief UART-based MIDI transmission driver.
 *
 * This implementation sends raw MIDI bytes over UART0 TX.
 *
 * For the project demo, UART0 is configured for 115200 baud so the
 * LaunchPad debug USB serial port can be used with a computer-side
 * serial-to-MIDI bridge.
 *
 * Pin used:
 *   PA1 = UART0 TX
 */

#include "TM4C123GH6PM.h"
#include "midi_uart.h"

#define MIDI_UART_BAUD_RATE 115200U
#define SYSTEM_CLOCK_HZ     50000000U

static void MIDI_UART_SetBaudRate(uint32_t baud_rate);

void MIDI_UART_Init(void)
{
    /*
     * Enable clocks for UART0 and GPIO Port A.
     * UART0 is connected to PA0/PA1.
     * We only need PA1 for transmit.
     */
    SYSCTL->RCGCUART |= 0x01;
    SYSCTL->RCGCGPIO |= 0x01;

    /*
     * Small delay to allow peripheral clocks to stabilize.
     */
    (void)SYSCTL->RCGCUART;
    (void)SYSCTL->RCGCGPIO;

    /*
     * Disable UART0 before configuration.
     */
    UART0->CTL &= ~0x01;

    /*
     * Configure PA1 for UART0 TX.
     *
     * PA1 alternate function = U0TX.
     * PCTL value 1 selects UART function.
     */
    GPIOA->AFSEL |= 0x02;
    GPIOA->PCTL &= ~0x000000F0;
    GPIOA->PCTL |=  0x00000010;
    GPIOA->DEN   |= 0x02;
    GPIOA->AMSEL &= ~0x02;

    /*
     * Use system clock for UART.
     */
    UART0->CC = 0x00;

    /*
     * Configure baud rate.
     */
    MIDI_UART_SetBaudRate(MIDI_UART_BAUD_RATE);

    /*
     * 8 data bits, no parity, 1 stop bit, FIFO enabled.
     */
    UART0->LCRH = 0x70;

    /*
     * Enable UART0 and TX.
     * Bit 0 = UART enable
     * Bit 8 = TX enable
     */
    UART0->CTL |= 0x101;
}

static void MIDI_UART_SetBaudRate(uint32_t baud_rate)
{
    uint32_t brd_times_64;
    uint32_t integer_part;
    uint32_t fractional_part;

    /*
     * UART baud divisor:
     *
     * BRD = UARTSysClk / (16 * baud)
     *
     * To avoid floating-point math:
     *
     * BRD_times_64 = round((UARTSysClk * 64) / (16 * baud))
     *              = round((UARTSysClk * 4) / baud)
     */
    brd_times_64 = ((SYSTEM_CLOCK_HZ * 4U) + (baud_rate / 2U)) / baud_rate;

    integer_part = brd_times_64 / 64U;
    fractional_part = brd_times_64 % 64U;

    UART0->IBRD = integer_part;
    UART0->FBRD = fractional_part;
}

void MIDI_UART_SendByte(uint8_t data)
{
    /*
     * Wait while UART transmit FIFO is full.
     * UARTFR bit 5 = TXFF.
     */
    while ((UART0->FR & 0x20) != 0U)
    {
    }

    UART0->DR = data;
}

void MIDI_UART_SendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
    MIDI_UART_SendByte((uint8_t)(0x90U | (channel & 0x0FU)));
    MIDI_UART_SendByte(note & 0x7FU);
    MIDI_UART_SendByte(velocity & 0x7FU);
}

void MIDI_UART_SendNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
    MIDI_UART_SendByte((uint8_t)(0x80U | (channel & 0x0FU)));
    MIDI_UART_SendByte(note & 0x7FU);
    MIDI_UART_SendByte(velocity & 0x7FU);
}