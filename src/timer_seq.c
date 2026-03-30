/**
 * @file timer_seq.c
 * @brief Hardware timer implementation for sequencer timing.
 */


#include "TM4C123GH6PM.h"
#include "timer_seq.h"
#include "sequencer.h"

void TimerSeq_Init(uint16_t bpm)
{
    (void)bpm;
    /* TODO: Configure timer peripheral, load interval, enable interrupt */
}

void TimerSeq_UpdatePeriod(uint16_t bpm)
{
    (void)bpm;
    /* TODO: Recalculate and update timer reload value */
}

void TimerSeq_Start(void)
{
    /* TODO: Enable timer counting */
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
