/**
 * @file timer_seq.c
 * @brief Timer configuration and interrupt-based step timing engine.
 *
 * This file configures a hardware timer to generate periodic interrupts
 * based on the sequencer BPM setting. The timer interrupt service routine
 * advances the playback step and signals the rest of the system when
 * real-time sequencer events occur.
 */