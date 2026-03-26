# MVP Specification



## **Project Title**

**Embedded MIDI Step Sequencer using the TM4C123G LaunchPad and EduBase V2**



## **Project Objective**

Design and implement a real-time embedded step sequencer on the TM4C123G LaunchPad and EduBase V2 board. The system allows a user to program 16-step rhythmic patterns across 4 tracks, control tempo, start and stop playback, and output MIDI note events to an external music system using UART. The project demonstrates GPIO, timer interrupts, UART communication, display interfacing, and real-time embedded state control. This aligns well with the course emphasis on embedded C, I/O programming, timer programming, serial communication, and interrupts.



## **MVP Features**

These are the features that define “project complete.”

### **Sequencer**

- 4 independent tracks
- 16 steps per track
- each step stores binary state: ON or OFF
- one current playback step shared across all tracks

### **User Input**

- EduBase SW2–SW5 select track 1–4
- Keypad K0–K15 toggles steps 1–16 for selected track
- Tiva SW2 starts and stops playback
- default tempo = 120 BPM

### **Playback Engine**

- playback driven by hardware timer interrupt
- each step represents a sixteenth note
- timer period derived from BPM
- current step advances cyclically from 0 to 15

### **MIDI Output**

- system sends MIDI note messages over UART
- one fixed MIDI note assigned to each track
- when current step is active for a given track, send Note On event
- optional Note Off handling may be immediate or short-duration depending on implementation detail

### **Visual Output**

- 7-segment display shows BPM
- LCD shows 16-step pattern for currently selected track using simple pattern view
- one of LED0–LED3 indicates selected track
- selected track LED briefly blinks when its note is triggered



## **Stretch Features**

Only after MVP is stable:

- Shift + SW2 tap tempo

- alternate LCD info mode

- mute / clear track

- numeric BPM entry

- PWM local drum sounds

- save/load patterns

- swing timing

  

## **Success Criteria**

The MVP is finished when the following live demo works:

- power on system
- BPM shows 120
- user selects a track
- user enters a pattern on keypad
- user presses Start
- playback advances in time
- MIDI note events are transmitted correctly for active steps
- LCD reflects programmed sequence
- LEDs indicate track selection and note trigger activity