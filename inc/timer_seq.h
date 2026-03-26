#ifndef TIMER_SEQ_H
#define TIMER_SEQ_H

#include <stdint.h>

/**
 * @file timer_seq.h
 * @brief Hardware timer interface for sequencer step timing.
 *
 * This module configures and controls the timer used as the timing
 * engine for the sequencer. The timer period is derived from BPM,
 * and the associated interrupt advances the playback step.
 */

/**
 * @brief Initialize the sequencer timing timer.
 *
 * Configures the hardware timer peripheral, loads the initial period,
 * and prepares the timer interrupt.
 *
 * @param bpm Initial tempo in beats per minute.
 */
void TimerSeq_Init(uint16_t bpm);

/**
 * @brief Update the timer period based on a new BPM value.
 *
 * @param bpm New tempo in beats per minute.
 */
void TimerSeq_UpdatePeriod(uint16_t bpm);

/**
 * @brief Start the sequencer timer.
 */
void TimerSeq_Start(void);

/**
 * @brief Stop the sequencer timer.
 */
void TimerSeq_Stop(void);

#endif