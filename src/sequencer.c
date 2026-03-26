/**
 * @file sequencer.c
 * @brief Core sequencer state and logic implementation.
 */

#include "sequencer.h"

static volatile uint8_t steps[4][16];
static volatile uint8_t current_track = 0U;
static volatile uint8_t current_step = 0U;
static volatile uint16_t bpm = 120U;
static volatile bool is_playing = false;
static volatile bool step_advanced_flag = false;

void Sequencer_Init(void)
{
    uint8_t track;
    uint8_t step;

    for (track = 0U; track < 4U; track++)
    {
        for (step = 0U; step < 16U; step++)
        {
            steps[track][step] = 0U;
        }
    }

    current_track = 0U;
    current_step = 0U;
    bpm = 120U;
    is_playing = false;
    step_advanced_flag = false;
}

void Sequencer_SetBPM(uint16_t new_bpm)
{
    bpm = new_bpm;
}

uint16_t Sequencer_GetBPM(void)
{
    return bpm;
}

void Sequencer_Start(void)
{
    is_playing = true;
}

void Sequencer_Stop(void)
{
    is_playing = false;
}

void Sequencer_TogglePlay(void)
{
    is_playing = !is_playing;
}

bool Sequencer_IsPlaying(void)
{
    return is_playing;
}

void Sequencer_SelectTrack(uint8_t track)
{
    if (track < 4U)
    {
        current_track = track;
    }
}

uint8_t Sequencer_GetCurrentTrack(void)
{
    return current_track;
}

void Sequencer_ToggleStep(uint8_t track, uint8_t step)
{
    if ((track < 4U) && (step < 16U))
    {
        steps[track][step] ^= 1U;
    }
}

uint8_t Sequencer_GetStepState(uint8_t track, uint8_t step)
{
    if ((track < 4U) && (step < 16U))
    {
        return steps[track][step];
    }

    return 0U;
}

void Sequencer_AdvanceStep(void)
{
    current_step = (uint8_t)((current_step + 1U) % 16U);
    step_advanced_flag = true;
}

uint8_t Sequencer_GetCurrentStep(void)
{
    return current_step;
}

bool Sequencer_HasStepAdvanced(void)
{
    if (step_advanced_flag)
    {
        step_advanced_flag = false;
        return true;
    }

    return false;
}

uint8_t Sequencer_GetMidiNoteForTrack(uint8_t track)
{
    static const uint8_t midi_notes[4] = {36U, 38U, 42U, 46U};

    if (track < 4U)
    {
        return midi_notes[track];
    }

    return 0U;
}