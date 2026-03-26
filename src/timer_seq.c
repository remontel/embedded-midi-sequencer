/**
 * @file timer_seq.c
 * @brief Timer configuration and interrupt-based step timing engine.
 *
 * This file configures a hardware timer to generate periodic interrupts
 * based on the sequencer BPM setting. The timer interrupt service routine
 * advances the playback step and signals the rest of the system when
 * real-time sequencer events occur.
 */


#include "TM4C123GH6PM.h"
#include "timer_seq.h"
#include "sequencer.h"

void TimerSeq_Init(uint16_t bpm)
{
    uint32_t reload_value;

    if (bpm == 0U)
    {
        bpm = 1U;
    }

    reload_value = (SystemCoreClock / ((uint32_t)bpm * 4U)) - 1U;

    SYSCTL->RCGCTIMER |= (1UL << 0);
    (void)SYSCTL->RCGCTIMER;

    TIMER0->CTL &= ~(1UL << 0);
    TIMER0->CFG = 0x0U;
    TIMER0->TAMR = 0x2U;
    TIMER0->TAILR = reload_value;
    TIMER0->ICR = (1UL << 0);
    TIMER0->IMR |= (1UL << 0);

    NVIC_EnableIRQ(TIMER0A_IRQn);
}

void TimerSeq_UpdatePeriod(uint16_t bpm)
{
    (void)bpm;
    /* TODO: Recalculate and update timer reload value */
}

void TimerSeq_Start(void)
{
    TIMER0->CTL |= 0x1U;
}

void TimerSeq_Stop(void)
{
    /* TODO: Disable timer counting */
}

/*
 * NOTE:
 * The actual ISR name must match the timer peripheral used.
 * Example: Timer0A_Handler(void)
 */
void Timer0A_Handler(void)
{
    /* TODO: Clear timer interrupt flag */

    if (Sequencer_IsPlaying())
    {
        Sequencer_AdvanceStep();
    }
}
