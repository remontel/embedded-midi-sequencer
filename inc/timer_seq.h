#ifndef TIMER_SEQ_H
#define TIMER_SEQ_H

#include <stdint.h>

/**
 * @file timer_seq.h
 * @brief Hardware timer interface for sequencer step timing.
 *
 * This module configures Timer0A as a fixed 1 ms time base for the
 * sequencer. A software millisecond counter converts BPM into a
 * sixteenth-note step interval, and the associated interrupt advances
 * the playback step when that interval expires.
 * 
 * @author Ignacio Martinez-Laparra, Rene Montelongo
 */

/**
 * @brief Initialize the sequencer timing timer.
 *
 * Configures Timer0A in periodic mode, sets up a 1 ms interrupt period,
 * prepares the NVIC entry, and initializes the software step interval
 * from the supplied BPM value.
 *
 * @param bpm Initial tempo in beats per minute.
 */
void TimerSeq_Init(uint16_t bpm);

/**
 * @brief Update the timer period based on a new BPM value.
 *
 * Recomputes the software step interval in milliseconds. The underlying
 * hardware timer remains a fixed 1 ms time base.
 *
 * @param bpm New tempo in beats per minute.
 */
void TimerSeq_UpdatePeriod(uint16_t bpm);

/**
 * @brief Start the sequencer timing engine.
 *
 * Resets the elapsed millisecond counter, clears any pending timeout
 * status, and enables Timer0A counting.
 */
void TimerSeq_Start(void);

/**
 * @brief Stop the sequencer timing engine.
 *
 * Disables Timer0A counting and clears the elapsed millisecond counter.
 */
void TimerSeq_Stop(void);

#endif
