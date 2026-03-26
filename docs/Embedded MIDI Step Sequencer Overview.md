# Embedded MIDI Step Sequencer (TM4C123G + EduBase V2)

## Overview

This project implements a **hardware step sequencer** using the **TM4C123G LaunchPad** and **[EduBase V2 board](https://trainer4edu.com/edubase_v2/index.html)**.

The system allows users to:
- Program 16-step patterns across 4 tracks
- See tempo (BPM)
- Start/stop playback
- Output MIDI to external software (e.g. Ableton Live)
- Optionally generate audio locally using the onboard speaker
- Optionally input tap tempo

The project demonstrates core embedded systems concepts including:
- GPIO input/output

- Timer-based scheduling

- Interrupt-driven design

- UART communication (MIDI)

- Real-time user interaction

  

---



## Features

### Core Features
- 4 independent tracks
- 16-step sequencer
- Real-time step editing
- Start / Stop playback
- MIDI output (UART)
- Visual feedback via LCD, LEDs, and 7-segment display
- View tempo (BPM)

### Core System Behavior

- user selects 1 of 4 tracks
- user toggles 16 steps on/off with keypad
- hardware timer advances playback step at tempo-derived interval
- active step on each track transmits MIDI note events over UART
- LCD and LEDs provide visual feedback
- 7-segment shows BPM (Default 120 BPM)

### Optional Features
- Adjust Tempo (BPM)
  - Tap tempo
  - Numeric BPM entry

- Local PWM audio (kick/snare/hat)
- ALT LCD displays Playback Info
- Track mute
- Track clear pattern
- Save/Load patterns
- Swing



---



## Hardware Components

![EduBase-V2_Tiva-100](/Users/werk/Library/Mobile Documents/com~apple~CloudDocs/Typora/EduBase-V2_Tiva-100.jpg)



### TM4C123G LaunchPad

- SW1: Shift / Alt modifier
- SW2: Start/Stop (Shift: Tap Tempo)

### EduBase V2 Board
- 16x2 LCD display

- 4-digit 7-segment display (DSP1–DSP4)

- 4 LEDs (LED0–LED3)

- 4 pushbuttons (SW2–SW5)

- 4x4 keypad (K0–K15)

- onboard speaker (PWM audio output)

  

---



## Control Layout

### Track Selection
| Control           | Function         |
| ----------------- | ---------------- |
| SW2–SW5 (EduBase) | Select Track 1–4 |

- LED0–LED3 indicate active track
- Selected track LED remains ON
- LED briefly blinks when note is triggered



### Step Sequencer Input

| Control | Function            |
| ------- | ------------------- |
| K0–K15  | Toggle steps (1–16) |

- Each key represents one step
- Active step = ON
- Inactive step = OFF



### Transport Controls
| Control     | Function     |
| ----------- | ------------ |
| Tiva SW2    | Start / Stop |
| Shift + SW2 | Tap Tempo    |



### Shift (Alt Mode)

| Control  | Function       |
| -------- | -------------- |
| Tiva SW1 | Shift modifier |

Used for:
- Tap tempo

- Numeric BPM entry

- Track editing functions (future expansion)

  

---



## Display System

### 7-Segment Display (DSP1–DSP4)
Displays tempo:

0120: 120 BPM

0096: 96 BPM



### LCD Display (16x2)

Displays sequence and system state.

#### Example (Playback Mode)

```
X . . . X . . . 
X . . . X . . . 
```

- Each row shows 8 steps (total = 16 steps)
- `X` = active step
- `.` = inactive step
- Current step flashes or is highlighted



---



## Timing Engine

The sequencer is driven by a hardware timer interrupt.

### Operation:
1. Timer configured based on BPM

2. Interrupt fires at each step interval

3. System:
   - Advances step index
   
   - Triggers note if step is active
   
   - Updates LEDs and LCD
   
     

---



## MIDI Output

MIDI messages are transmitted via UART.

### Note Mapping (Channel 1)

| Track | MIDI Note | Instrument |
| ----- | --------- | ---------- |
| 1     | C1 (36)   | Kick       |
| 2     | D1 (38)   | Snare      |
| 3     | F#1 (42)  | Hi-hat     |
| 4     | A#1 (46)  | Clap       |

### MIDI Messages

Note ON: 0x90, note, velocity

Note OFF: 0x80, note, 0



---



## Software Architecture

### Simplified Architecture

```
User presses buttons / keypad
        ↓
MCU reads inputs
        ↓
MCU updates sequencer state
        ↓
Timer interrupt advances playback
        ↓
MCU updates LEDs / LCD / BPM display
        ↓
MCU sends MIDI over UART
```

### Detailed Architecture

```
User Inputs
  ├─ Tiva SW1: Shift/Alt
  ├─ Tiva SW2: Start/Stop
  ├─ EduBase SW2–SW5: Track Select
  └─ Keypad K0–K15: Step toggle
        ↓
Input Handling Layer
  ├─ button scan / debounce
  ├─ keypad scan
  └─ event generation
        ↓
Sequencer State Machine
  ├─ current_track
  ├─ steps[4][16]
  ├─ current_step
  ├─ bpm
  ├─ play/stop state
  └─ shift state
        ↓
Real-Time Engine
  └─ timer ISR advances sequencer
        ↓
Output Layer
  ├─ MIDI UART transmit
  ├─ LEDs
  ├─ LCD
  └─ 7-segment BPM
```



---



## Modules/Files

```
main.c
sequencer.c / sequencer.h
gpio.c / gpio.h
keypad.c / keypad.h
lcd.c / lcd.h
display7seg.c / display7seg.h
midi_uart.c / midi_uart.h
timer_seq.c / timer_seq.h
buttons.c / buttons.h
```



#### main.c

**Purpose:**

Main entry point of the program. Initializes all modules, then runs the foreground loop that handles user input, UI updates, and non-time-critical tasks.

**What it should do:**

- initialize peripherals/modules
- run while(1)
- scan inputs
- process UI events
- update LCD / LEDs / 7-seg
- respond to ISR flags

**What it should not do:**

- contain giant piles of hardware setup code

- contain all logic directly

- do timer-critical sequencing itself

  

#### **sequencer.c / sequencer.h**

**Purpose:**

Contains the core application logic and sequencer state.

**What it should do:**

- store steps [4] [16]
- store current_track, current_step, bpm, playback state
- toggle a step on/off
- select track
- start/stop playback
- compute next step behavior
- map tracks to MIDI notes



#### **gpio.c / gpio.h**

**Purpose:**

Low-level GPIO setup and control for LEDs and possibly shared digital I/O lines.

**What it should do:**

- initialize GPIO ports used in project
- set LED outputs
- maybe wrap simple pin writes/reads

*Note: Some LED or button responsibilities may move elsewhere, but GPIO init itself belongs here.*



#### **buttons.c / buttons.h**

**Purpose:**

Handles pushbutton reading and debounce logic.

**What it should do:**

- read Tiva SW1/SW2

- read EduBase SW2–SW5

- debounce presses

- provide clean “button event” outputs

  

#### **keypad.c / keypad.h**

**Purpose:**

Handles 4x4 keypad scanning and key detection.

**What it should do:**

- initialize keypad I/O lines
- scan rows/columns
- identify which key is pressed
- debounce keypad input
- return key index 0–15



#### **lcd.c / lcd.h**

**Purpose:**

Controls the 16x2 LCD and provides higher-level display functions.

**What it should do:**

- initialize LCD
- clear LCD
- write characters/strings
- place cursor
- update sequence view

*Note: Later, may also support alternate display modes.*



#### **display7seg.c / display7seg.h**

**Purpose:**

Controls the 4-digit 7-segment display for BPM display.

**What it should do:**

- initialize display
- split BPM into digits
- write digits to display
- refresh display if multiplexed



#### **midi_uart.c / midi_uart.h**

**Purpose:**

Implements UART-based MIDI transmission.

**What it should do:**

- initialize UART for MIDI use
- send raw bytes
- send MIDI Note On
- send MIDI Note Off
- possibly provide helper functions for track-to-note mapping



#### **timer_seq.c / timer_seq.h**

**Purpose:**

Implements the hardware timer used for sequencer step timing and contains the timer interrupt service routine.

**What it should do:**

- initialize timer

- configure timer interval from BPM

- enable interrupt

- define ISR

- set lightweight flags for main loop

  

---



### Data Structures

```c
#include <stdint.h>
#include <stdbool.h>

volatile uint8_t steps[4][16];
volatile uint8_t current_step;
volatile uint8_t current_track;
volatile uint16_t bpm;
volatile bool is_playing;
volatile bool shift_pressed;
volatile bool step_advanced_flag;
volatile bool led_trigger_flags[4];
```

Note: `volatile` tells compiler this variable might change unexpectedly outside normal code flow, so do not assume its value stays the same.



---



## **Other Features** (Optional)

- Additional ALT Functions:
  - Track mute / solo
  - Pattern save/load
  - Swing timing
  - Display Playback Info on Display `T2 BPM120 PLAY`
  
- External MIDI clock sync

- Rotary encoder support

- Sample playback (advanced)

  

### Shift/Alt LCD Display

```
T2 BPM120 PLAY
X...X...X...X...
```

- Show Playback on First Line
- Compressed step info

### Local Audio 

Audio is generated using PWM through the onboard speaker.

#### Sound Synthesis

Instead of loading samples, sounds are generated procedurally:

- Kick: decaying sine wave
- Snare : filtered noise burst
- Hi-hat:  high-frequency noise

Advantages:

- No external storage required
- Demonstrates embedded DSP concepts
- Real-time synthesis using timers

