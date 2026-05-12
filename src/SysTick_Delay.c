/**
 * @file SysTick_Delay.c
 *
 * @brief Source code for the SysTick_Delay driver.
 *
 * It provides two blocking functions, SysTick_Delay1ms and SysTick_Delay1us,
 * to create delays with a busy-wait loop. The implementation counts elapsed
 * time in software from SysTick interrupts.
 *
 * Important timing note:
 * The code loads SysTick for a 4-cycle period and treats each interrupt as
 * 1 us. Delay accuracy therefore depends on the actual SysTick clock source
 * and the active system clock configuration.
 *
 * @author Aaron Nanas
 */

#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"

// Internal software counter for elapsed microsecond ticks
static uint32_t us_elapsed = 0;

// Internal software counter for elapsed milliseconds
static uint32_t ms_elapsed = 0;

// Enables millisecond accumulation inside the SysTick ISR
static uint8_t ms_active = 0;

void SysTick_Delay_Init(void)
{	
	// Load SysTick for a 4-cycle interval.
	// The implementation assumes this corresponds to a 1 us tick.
	SysTick->LOAD = (4 - 1);
	
	// Clear the VAL register by writing any value to it
	SysTick->VAL = 0;
	
	// Enable the SysTick counter and interrupt.
	// Timing depends on the currently selected SysTick clock source.
	SysTick->CTRL |= 0x03;
}

void SysTick_Delay1us(uint32_t delay_in_us)
{
	// Reset the internal microsecond counter
	us_elapsed = 0;
	
	// Busy-wait until the ISR has counted the requested number of ticks
	while (delay_in_us > us_elapsed);
}

void SysTick_Delay1ms(uint32_t delay_in_ms)
{
	// Reset the internal elapsed-time counters
	us_elapsed = 0;
	ms_elapsed = 0;
	
	// Enable millisecond accumulation in the ISR
	ms_active = 0x01;
	
	// Busy-wait until the requested number of milliseconds has elapsed
	while (delay_in_ms > ms_elapsed);
	
	// Disable millisecond accumulation
	ms_active = 0x00;
}

void SysTick_Handler(void)
{
	// Count one microsecond tick
	us_elapsed = us_elapsed + 1;
	
	// Fold 1000 microsecond ticks into one millisecond when ms mode is active
	if (us_elapsed == 1000 && (ms_active == 0x01))
	{
		// Restart the microsecond counter
		us_elapsed = 0;
		
		// Record one elapsed millisecond
		ms_elapsed = ms_elapsed + 1;
	}
}
