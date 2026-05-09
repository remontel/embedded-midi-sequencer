/**
 * @file timer_seq.c
 * @brief Hardware timer implementation for sequencer timing.
 */

#include "TM4C123GH6PM.h"
#include "timer_seq.h"
#include "sequencer.h"

uint32_t timer_ms_elapsed = 0;
uint32_t timer_step_period_ms = 125;

void TimerSeq_Init(uint16_t bpm)
{
    /* Enable clock for Timer0 */
    SYSCTL->RCGCTIMER |= 0x01;

    /* Disable Timer0A before configuration */
    TIMER0->CTL &= ~0x01;

    /* 16-bit timer configuration */
    TIMER0->CFG = 0x04;

    /* Periodic mode */
    TIMER0->TAMR = 0x02;

    /* Prescale 50 MHz to 1 MHz */
    TIMER0->TAPR = 50 - 1;

    /* 1 ms interval at 1 MHz */
    TIMER0->TAILR = 1000 - 1;

    /* Clear timeout flag */
    TIMER0->ICR = 0x01;

    /* Enable timeout interrupt */
    TIMER0->IMR |= 0x01;

    /* Set priority 2 for Timer0A (IRQ 19) */
    NVIC->IPR[4] &= ~0xE0000000;
    NVIC->IPR[4] |= (2 << 29);

    /* Enable IRQ 19 */
    NVIC->ISER[0] |= (1 << 19);

    TimerSeq_UpdatePeriod(bpm);
    timer_ms_elapsed = 0;
}

void TimerSeq_UpdatePeriod(uint16_t bpm)
{
    if (bpm == 0U)
    {
        bpm = 120U;
    }

    /* sixteenth-note period in ms */
    timer_step_period_ms = 60000U / (4U * bpm);

    if (timer_step_period_ms == 0U)
    {
        timer_step_period_ms = 1U;
    }
}

void TimerSeq_Start(void)
{
    timer_ms_elapsed = 0;
    TIMER0->ICR = 0x01;
    TIMER0->CTL |= 0x01;
}

void TimerSeq_Stop(void)
{
    TIMER0->CTL &= ~0x01;
    timer_ms_elapsed = 0;
}

void TIMER0A_Handler(void)
{
    /* Clear timeout interrupt flag */
    TIMER0->ICR = 0x01;

    if (Sequencer_IsPlaying())
    {
        timer_ms_elapsed++;

        if (timer_ms_elapsed >= timer_step_period_ms)
        {
            timer_ms_elapsed = 0;
            Sequencer_AdvanceStep();
        }
    }
}
