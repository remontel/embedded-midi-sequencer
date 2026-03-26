#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file sequencer.h
 * @brief Core sequencer state and logic interface.
 *
 * This module owns the shared application state for the 4-track, 16-step
 * sequencer. It provides functions for tempo control, playback state,
 * track selection, step editing, step advancement, and track-to-MIDI-note
 * mapping.
 */

/**
 * @brief Initialize sequencer state to default values.
 *
 * Sets all steps to OFF, selects track 0, sets current step to 0,
 * initializes BPM to 120, and places the sequencer in the stopped state.
 */
void Sequencer_Init(void);

/**
 * @brief Set the sequencer tempo in beats per minute.
 *
 * @param new_bpm New BPM value.
 */
void Sequencer_SetBPM(uint16_t new_bpm);

/**
 * @brief Get the current tempo in beats per minute.
 *
 * @return Current BPM value.
 */
uint16_t Sequencer_GetBPM(void);

/**
 * @brief Start sequencer playback.
 */
void Sequencer_Start(void);

/**
 * @brief Stop sequencer playback.
 */
void Sequencer_Stop(void);

/**
 * @brief Toggle between play and stop states.
 */
void Sequencer_TogglePlay(void);

/**
 * @brief Check whether the sequencer is currently playing.
 *
 * @return true if playing, false if stopped.
 */
bool Sequencer_IsPlaying(void);

/**
 * @brief Select the active editing track.
 *
 * @param track Track index (0 to 3).
 */
void Sequencer_SelectTrack(uint8_t track);

/**
 * @brief Get the currently selected track.
 *
 * @return Current track index (0 to 3).
 */
uint8_t Sequencer_GetCurrentTrack(void);

/**
 * @brief Toggle the ON/OFF state of a step in a given track.
 *
 * @param track Track index (0 to 3).
 * @param step  Step index (0 to 15).
 */
void Sequencer_ToggleStep(uint8_t track, uint8_t step);

/**
 * @brief Get the ON/OFF state of a given step.
 *
 * @param track Track index (0 to 3).
 * @param step  Step index (0 to 15).
 *
 * @return Step state: 0 = OFF, nonzero = ON.
 */
uint8_t Sequencer_GetStepState(uint8_t track, uint8_t step);

/**
 * @brief Advance the current playback step by one.
 *
 * Wraps from step 15 back to step 0 and sets the internal
 * step-advanced flag for foreground processing.
 */
void Sequencer_AdvanceStep(void);

/**
 * @brief Get the current playback step index.
 *
 * @return Current step index (0 to 15).
 */
uint8_t Sequencer_GetCurrentStep(void);

/**
 * @brief Check whether a new step advancement event has occurred.
 *
 * This function returns true once per step advancement and clears
 * the internal flag after it is read.
 *
 * @return true if a step-advance event is pending, false otherwise.
 */
bool Sequencer_HasStepAdvanced(void);

/**
 * @brief Get the MIDI note assigned to a given track.
 *
 * Default mapping:
 * - Track 0 -> 36
 * - Track 1 -> 38
 * - Track 2 -> 42
 * - Track 3 -> 46
 *
 * @param track Track index (0 to 3).
 *
 * @return MIDI note number for the track.
 */
uint8_t Sequencer_GetMidiNoteForTrack(uint8_t track);

#endif