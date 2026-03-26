# Module Responsibility



## Module Structure

| Module           | Purpose                            | Inputs                     | Outputs                                    | Owner Suggestion |
| ---------------- | ---------------------------------- | -------------------------- | ------------------------------------------ | ---------------- |
| main.c           | System startup and foreground loop | module APIs, status flags  | system orchestration                       | Rene             |
| sequencer.c/.h   | Core state + sequencing logic      | button/keypad events, BPM  | step state, playback state, note decisions | Rene             |
| timer_seq.c/.h   | Timer setup and ISR timing engine  | BPM value, play state      | periodic step advancement                  | Rene             |
| midi_uart.c/.h   | UART MIDI transmission             | note, velocity, channel    | MIDI byte stream                           | Rene             |
| buttons.c/.h     | Read and debounce pushbuttons      | raw GPIO states            | clean button press events                  | Ignacio          |
| keypad.c/.h      | Scan 4x4 keypad                    | row/column states          | key index (0–15)                           | Ignacio          |
| lcd.c/.h         | LCD control and sequence display   | track, steps, current step | LCD output                                 | Shared           |
| display7seg.c/.h | BPM display                        | BPM value                  | 7-segment display output                   | Ignacio          |
| gpio.c/.h        | Low-level GPIO init/helpers        | pin configuration          | hardware access abstraction                | Shared / Rene    |



## Function-Level Plan

### **main.c**

**Purpose**

System startup and foreground loop.



**Functions**

```c
int main(void);
```



**Responsibilities**

- initialize all modules
- set default BPM (120)
- continuously scan inputs
- process user interaction
- respond to timer-driven events
- update display outputs

**Typical flow**

```c
int main(void)
{
    // initialize hardware/modules

    while (1)
    {
        // scan buttons
        // scan keypad
        // handle track selection
        // handle step editing
        // react to step advance events
        // update displays
    }
}
```



### sequencer.h / sequencer.c

**Purpose**

Own sequencer state and core logic.



**Suggested state variables**

```c
extern volatile uint8_t steps[4][16];
extern volatile uint8_t current_track;
extern volatile uint8_t current_step;
extern volatile uint16_t bpm;
extern volatile bool is_playing;
extern volatile bool step_advanced_flag;
```



**Suggested Functions**

```c
void Sequencer_Init(void);

void Sequencer_SetBPM(uint16_t new_bpm);
uint16_t Sequencer_GetBPM(void);

void Sequencer_Start(void);
void Sequencer_Stop(void);
void Sequencer_TogglePlay(void);
bool Sequencer_IsPlaying(void);

void Sequencer_SelectTrack(uint8_t track);
uint8_t Sequencer_GetCurrentTrack(void);

void Sequencer_ToggleStep(uint8_t track, uint8_t step);
uint8_t Sequencer_GetStepState(uint8_t track, uint8_t step);

void Sequencer_AdvanceStep(void);
uint8_t Sequencer_GetCurrentStep(void);

bool Sequencer_HasStepAdvanced(void);

uint8_t Sequencer_GetMidiNoteForTrack(uint8_t track);
```



**Notes**

- `Sequencer_Init()` 

  - Initializes all steps to OFF
  - Sets track = 0, step = 0
  - Sets BPM = 120
  - Sets playback to STOPPED

- `Sequencer_SetBPM()` 

  - Updates tempo value
  - Timer module will later use this to update timing

- `Sequencer_GetBPM()`

  - Returns the current tempo value (in BPM)
  - Used by display modules (7-segment) and timer module
  - Does not modify state (read-only accessor)

- `Sequencer_Start()` / `Stop()` / `TogglePlay()` 

  - control playback state

- `Sequencer_IsPlaying()`

  - Returns current playback state (true = playing, false = stopped)
  - Used by timer ISR and main loop to determine behavior

- `Sequencer_SelectTrack()` 

  - Changes active track (0–3)

- `Sequencer_GetCurrentTrack()`

  - Returns the currently selected track (0–3)
  - Used by UI modules (LCD, LEDs) and input handling

- `Sequencer_ToggleStep()` 

  - Toggles ON/OFF state of a step

- `Sequencer_GetStepState()`

  - Returns ON/OFF state of a specific step
  - Used by LCD display logic to render sequence
  -  Does not modify state

- `Sequencer_AdvanceStep()` 

  - Moves step forward (mod 16)
  - Sets `step_advanced_flag = true`

- `Sequencer_GetCurrentStep()`

  - Returns the current playback step index (0–15)
  - Used for display updates and timing logic
  - Updated only by `Sequencer_AdvanceStep()`

- `Sequencer_HasStepAdvanced()`

  - Returns true once per step advance
  - Clears the flag after reading
  - Used by main loop for UI updates

- `Sequencer_GetMidiNoteForTrack()` 

  - Maps track → MIDI note

  - Track 0 = 36 (Kick)

  - Track 1 = 38 (Snare)

  - Track 2 = 42 (Hi-hat)

  - Track 3 = 46 (Clap)

    

### **timer_seq.h / timer_seq.c**

**Purpose**

Provide real-time timing using hardware timer interrupts.



**Suggested Functions**

```c
void TimerSeq_Init(uint16_t bpm);
void TimerSeq_UpdatePeriod(uint16_t bpm);
void TimerSeq_Start(void);
void TimerSeq_Stop(void);
```



**Notes**

- `TimerSeq_Init(bpm)` 

  - Configures hardware timer
  - Sets initial period based on BPM
  - Enables interrupt

- `TimerSeq_UpdatePeriod(bpm)` 

  - Recalculates timer reload value
  - Called when BPM changes

- `TimerSeq_Start()` 

  - Enables timer counting

- `TimerSeq_Stop()` 

  - Disables timer counting

- ISR Behavior (inside .c file)

  - Clear interrupt flag

  - If playing:

    - call `Sequencer_AdvanceStep()`
    - optionally trigger MIDI events

    

### **midi_uart.h / midi_uart.c**

**Purpose**

Transmit MIDI messages over UART



**Suggested Functions**

```c
void MIDI_UART_Init(void);

void MIDI_UART_SendByte(uint8_t data);

void MIDI_UART_SendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
void MIDI_UART_SendNoteOff(uint8_t channel, uint8_t note, uint8_t velocity);
```



**Notes**

- `MIDI_UART_Init()` 
  - Configures UART peripheral and GPIO pins
- `MIDI_UART_SendByte()` 
  - Sends a single byte over UART
- `MIDI_UART_SendNoteOn()`
  - Sends MIDI Note ON message
  - Format: `0x90 | channel, note, velocity`
- `MIDI_UART_SendNoteOff()` 
  - Sends MIDI Note OFF message
  - Format: `0x80 | channel, note, velocity`



### **buttons.h / buttons.c**

**Purpose**

Read and debounce pushbuttons.



**Suggested Functions**

```c
void Buttons_Init(void);
void Buttons_Update(void);

bool Buttons_WasTivaSW1Pressed(void);
bool Buttons_WasTivaSW2Pressed(void);

bool Buttons_WasEduBaseSW2Pressed(void);
bool Buttons_WasEduBaseSW3Pressed(void);
bool Buttons_WasEduBaseSW4Pressed(void);
bool Buttons_WasEduBaseSW5Pressed(void);
```



**Notes**

- Module handles:
  - debouncing
  - edge detection (press events)
- `Buttons_Init()`
  - Configures GPIO pins connected to pushbuttons
  - Sets direction (input) and enables digital functionality
  - May enable pull-up or pull-down resistors depending on hardware
  - Initializes internal state variables used for debouncing

- `Buttons_Update()`

  - Reads GPIO inputs
  - Updates internal button states

- `Buttons_WasXPressed()`

  - Returns TRUE only once per press
  - Prevents repeated triggers from holding button

  

### **keypad.h / keypad.c**

**Purpose**

Scan 4x4 keypad and detect key presses



**Suggested Function**

```c
int8_t Keypad_Scan(void);
```



**Notes**

- Returns:
  - `-1` : no key pressed
  - `0-15` :  key index
- Module responsibilities:
  - drive row lines
  - read column lines
  - detect active key
  - apply basic debounce
- Each key corresponds to a sequencer step



### **lcd.h / lcd.c**

**Purpose**

Display sequencer state and step pattern.



**Suggested Function**

```c
void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_WriteChar(char c);
void LCD_WriteString(const char *str);

void LCD_DisplayTrackSteps(uint8_t track, uint8_t current_step);
```



**Notes**

- `LCD_Init()`
  - Initializes LCD hardware

- `LCD_Clear()`

  - Clears entire LCD display
  - Resets cursor to home position (row 0, column 0)
  - Typically used before writing a full new frame

- `LCD_SetCursor()`

  - Sets current cursor position on LCD
  - `row` selects line (0 or 1)
  - `col` selects column position (0–15)
  - Used before writing characters or strings

- `LCD_WriteChar()`

  - Writes a single character at current cursor position
  - Automatically advances cursor after write
  - Used for fine-grained display updates

- `LCD_WriteString()`
  - Writes text to display
- `LCD_DisplayTrackSteps()`
  - Main display function for MVP
  - Shows 16 steps across two rows
  - Uses:
    - `X` = active step
    - `.` = inactive step
  - Highlights or flashes current step



### **display7seg.h / display7seg.c**

**Purpose**

Display BPM on 7-segment display.



**Suggested functions**

```c
void Display7Seg_Init(void);
void Display7Seg_ShowNumber(uint16_t value);
```



**Notes**

- `Display7Seg_Init()`
  - Configures GPIO pins for display
- `Display7Seg_ShowNumber()`
  - Displays BPM value (e.g., 120)
  - Handles digit splitting and output
- May internally use multiplexing depending on hardware



### **gpio.h / gpio.c**

**Purpose**

Provide low-level GPIO initialization and helper functions.



**Suggested Functions**

```c
void GPIO_ProjectInit(void);

void GPIO_SetTrackLED(uint8_t track, bool on);
void GPIO_ClearAllTrackLEDs(void);
void GPIO_BlinkTrackLED(uint8_t track);
```



**Notes**

- `GPIO_ProjectInit()`

  - Enables all required GPIO ports
  - Configures pins for:
    - LEDs
    - buttons
    - keypad
    - display interfaces

- `GPIO_SetTrackLED()`

  - Turns specific track LED ON/OFF

- `GPIO_ClearAllTrackLEDs()`

  - Turns all track LEDs OFF

- `GPIO_BlinkTrackLED()`

  - Temporarily toggles LED to indicate note trigger

  
