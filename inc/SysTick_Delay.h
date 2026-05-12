/**
 * @file SysTick_Delay.h
 *
 * @brief Header file for the SysTick_Delay driver.
 *
 * It provides two blocking functions, SysTick_Delay1ms and SysTick_Delay1us,
 * to create delays with a busy-wait loop driven by the SysTick interrupt.
 * The implementation loads SysTick for a 4-cycle period and counts one
 * software "microsecond" per interrupt.
 *
 * Important timing note:
 * The current implementation assumes a 4 MHz SysTick clock so that
 * 4 cycles correspond to 1 us. The actual delay accuracy therefore depends
 * on the active SysTick clock source and system clock configuration.
 *
 * @author Aaron Nanas
 */
 
#include "TM4C123GH6PM.h"

/**
 * @brief The SysTick_Delay_Init function initializes the SysTick timer to be used for a blocking delay function.
 *
 * This function configures the SysTick reload value and enables the SysTick
 * counter and interrupt so the ISR can maintain the internal elapsed-time
 * counters used by the blocking delay helpers.
 */
void SysTick_Delay_Init(void);

/**
 * @brief The SysTick_Delay1us function provides a blocking delay in microseconds using the SysTick timer.
 *
 * This function resets the internal microsecond counter to zero and then
 * busy-waits until the SysTick ISR has counted the requested number of
 * microsecond ticks.
 *
 * @param delay_in_us The delay time in microseconds.
 */
void SysTick_Delay1us(uint32_t delay_in_us);

/**
 * @brief The SysTick_Delay1ms function provides a blocking delay in milliseconds using the SysTick timer.
 *
 * This function clears the internal counters, enables millisecond
 * accumulation, and busy-waits until the requested number of millisecond
 * intervals has elapsed.
 *
 * @param delay_in_ms The delay time in milliseconds.
 */
void SysTick_Delay1ms(uint32_t delay_in_ms);

/**
 * @brief The SysTick_Handler function is the interrupt service routine for the SysTick timer.
 *
 * This ISR increments the internal microsecond counter on each SysTick event.
 * When millisecond mode is active, every 1000 microsecond ticks are folded
 * into the internal millisecond counter used by SysTick_Delay1ms().
 */
void SysTick_Handler(void);
