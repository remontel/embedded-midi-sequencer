/**
 * @file buttons.c
 * @brief Pushbutton input handling and debounce logic.
 *
 * This file reads the Tiva LaunchPad and EduBase pushbuttons, applies
 * software debounce where needed, and provides clean button press events
 * to the main application logic.
 */

#include "TM4C123GH6PM.h"
#include "buttons.h"

/* Tiva LaunchPad buttons */
#define TIVA_SW1_MASK      0x10    /* PF4, active low */
#define TIVA_SW2_MASK      0x01    /* PF0, active low */

/* EduBase buttons */
#define EDUBASE_SW2_MASK   0x08    /* PD3, active high */
#define EDUBASE_SW3_MASK   0x04    /* PD2, active high */
#define EDUBASE_SW4_MASK   0x02    /* PD1, active high */
#define EDUBASE_SW5_MASK   0x01    /* PD0, active high */

#define DEBOUNCE_COUNT     5
#define GPIO_LOCK_KEY      0x4C4F434B

/* Debounced states: 0 = released, 1 = pressed */
unsigned char tiva_sw1_state;
unsigned char tiva_sw2_state;
unsigned char edubase_sw2_state;
unsigned char edubase_sw3_state;
unsigned char edubase_sw4_state;
unsigned char edubase_sw5_state;

/* Debounce counters */
unsigned char tiva_sw1_count;
unsigned char tiva_sw2_count;
unsigned char edubase_sw2_count;
unsigned char edubase_sw3_count;
unsigned char edubase_sw4_count;
unsigned char edubase_sw5_count;

/* One-shot event flags */
bool tiva_sw1_event;
bool tiva_sw2_event;
bool edubase_sw2_event;
bool edubase_sw3_event;
bool edubase_sw4_event;
bool edubase_sw5_event;

void Buttons_Init(void)
{
    /* Enable clock for Port D and Port F */
    SYSCTL->RCGCGPIO |= 0x28;

    /* ---------------- Port D: EduBase SW2-SW5 ---------------- */
    GPIOD->DIR &= ~0x0F;      /* PD3-PD0 input */
    GPIOD->AFSEL &= ~0x0F;    /* GPIO function */
    GPIOD->DEN |= 0x0F;       /* digital enable */
    GPIOD->AMSEL &= ~0x0F;    /* disable analog */
    GPIOD->PDR |= 0x0F;       /* weak pull-downs for active-high buttons */

    /* ---------------- Port F: Tiva SW1 and SW2 ---------------- */
    GPIOF->LOCK = GPIO_LOCK_KEY;
    GPIOF->CR |= 0x11;        /* allow changes to PF4 and PF0 */

    GPIOF->DIR &= ~0x11;      /* PF4 and PF0 input */
    GPIOF->AFSEL &= ~0x11;    /* GPIO function */
    GPIOF->DEN |= 0x11;       /* digital enable */
    GPIOF->AMSEL &= ~0x11;    /* disable analog */
    GPIOF->PUR |= 0x11;       /* pull-ups for active-low switches */

    /* Initialize debounced button states */
    tiva_sw1_state = ((GPIOF->DATA & TIVA_SW1_MASK) == 0);
    tiva_sw2_state = ((GPIOF->DATA & TIVA_SW2_MASK) == 0);

    edubase_sw2_state = ((GPIOD->DATA & EDUBASE_SW2_MASK) != 0);
    edubase_sw3_state = ((GPIOD->DATA & EDUBASE_SW3_MASK) != 0);
    edubase_sw4_state = ((GPIOD->DATA & EDUBASE_SW4_MASK) != 0);
    edubase_sw5_state = ((GPIOD->DATA & EDUBASE_SW5_MASK) != 0);

    /* Clear counters */
    tiva_sw1_count = 0;
    tiva_sw2_count = 0;
    edubase_sw2_count = 0;
    edubase_sw3_count = 0;
    edubase_sw4_count = 0;
    edubase_sw5_count = 0;

    /* Clear one-shot events */
    tiva_sw1_event = false;
    tiva_sw2_event = false;
    edubase_sw2_event = false;
    edubase_sw3_event = false;
    edubase_sw4_event = false;
    edubase_sw5_event = false;
}

void Buttons_Update(void)
{
    unsigned char raw_state;

    /* ---------------- Tiva SW1 ---------------- */
    raw_state = ((GPIOF->DATA & TIVA_SW1_MASK) == 0);

    if (raw_state != tiva_sw1_state)
    {
        tiva_sw1_count++;

        if (tiva_sw1_count >= DEBOUNCE_COUNT)
        {
            tiva_sw1_state = raw_state;
            tiva_sw1_count = 0;

            if (tiva_sw1_state == 1)
            {
                tiva_sw1_event = true;
            }
        }
    }
    else
    {
        tiva_sw1_count = 0;
    }

    /* ---------------- Tiva SW2 ---------------- */
    raw_state = ((GPIOF->DATA & TIVA_SW2_MASK) == 0);

    if (raw_state != tiva_sw2_state)
    {
        tiva_sw2_count++;

        if (tiva_sw2_count >= DEBOUNCE_COUNT)
        {
            tiva_sw2_state = raw_state;
            tiva_sw2_count = 0;

            if (tiva_sw2_state == 1)
            {
                tiva_sw2_event = true;
            }
        }
    }
    else
    {
        tiva_sw2_count = 0;
    }

    /* ---------------- EduBase SW2 ---------------- */
    raw_state = ((GPIOD->DATA & EDUBASE_SW2_MASK) != 0);

    if (raw_state != edubase_sw2_state)
    {
        edubase_sw2_count++;

        if (edubase_sw2_count >= DEBOUNCE_COUNT)
        {
            edubase_sw2_state = raw_state;
            edubase_sw2_count = 0;

            if (edubase_sw2_state == 1)
            {
                edubase_sw2_event = true;
            }
        }
    }
    else
    {
        edubase_sw2_count = 0;
    }

    /* ---------------- EduBase SW3 ---------------- */
    raw_state = ((GPIOD->DATA & EDUBASE_SW3_MASK) != 0);

    if (raw_state != edubase_sw3_state)
    {
        edubase_sw3_count++;

        if (edubase_sw3_count >= DEBOUNCE_COUNT)
        {
            edubase_sw3_state = raw_state;
            edubase_sw3_count = 0;

            if (edubase_sw3_state == 1)
            {
                edubase_sw3_event = true;
            }
        }
    }
    else
    {
        edubase_sw3_count = 0;
    }

    /* ---------------- EduBase SW4 ---------------- */
    raw_state = ((GPIOD->DATA & EDUBASE_SW4_MASK) != 0);

    if (raw_state != edubase_sw4_state)
    {
        edubase_sw4_count++;

        if (edubase_sw4_count >= DEBOUNCE_COUNT)
        {
            edubase_sw4_state = raw_state;
            edubase_sw4_count = 0;

            if (edubase_sw4_state == 1)
            {
                edubase_sw4_event = true;
            }
        }
    }
    else
    {
        edubase_sw4_count = 0;
    }

    /* ---------------- EduBase SW5 ---------------- */
    raw_state = ((GPIOD->DATA & EDUBASE_SW5_MASK) != 0);

    if (raw_state != edubase_sw5_state)
    {
        edubase_sw5_count++;

        if (edubase_sw5_count >= DEBOUNCE_COUNT)
        {
            edubase_sw5_state = raw_state;
            edubase_sw5_count = 0;

            if (edubase_sw5_state == 1)
            {
                edubase_sw5_event = true;
            }
        }
    }
    else
    {
        edubase_sw5_count = 0;
    }
}

bool Buttons_WasTivaSW1Pressed(void)
{
    bool pressed;

    pressed = tiva_sw1_event;
    tiva_sw1_event = false;

    return pressed;
}

bool Buttons_WasTivaSW2Pressed(void)
{
    bool pressed;

    pressed = tiva_sw2_event;
    tiva_sw2_event = false;

    return pressed;
}

bool Buttons_WasEduBaseSW2Pressed(void)
{
    bool pressed;

    pressed = edubase_sw2_event;
    edubase_sw2_event = false;

    return pressed;
}

bool Buttons_WasEduBaseSW3Pressed(void)
{
    bool pressed;

    pressed = edubase_sw3_event;
    edubase_sw3_event = false;

    return pressed;
}

bool Buttons_WasEduBaseSW4Pressed(void)
{
    bool pressed;

    pressed = edubase_sw4_event;
    edubase_sw4_event = false;

    return pressed;
}

bool Buttons_WasEduBaseSW5Pressed(void)
{
    bool pressed;

    pressed = edubase_sw5_event;
    edubase_sw5_event = false;

    return pressed;
}
