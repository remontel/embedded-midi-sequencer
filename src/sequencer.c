/**
 * @file sequencer.c
 * @brief Core sequencer logic and shared application state.
 *
 * This file contains the main data structures and logic used to implement
 * the 4-track, 16-step MIDI step sequencer. It manages the current track,
 * current playback step, tempo, playback state, and step on/off data for
 * all tracks. It also provides helper functions for track selection, step
 * editing, playback control, and note mapping.
 */