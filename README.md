# Embedded MIDI Step Sequencer

Setup to run the hardware sequencer and trigger sounds in DAW via a serial to MIDI bridge.



## Overview

- TM4C123 LaunchPad + EduBase V2
- 4-track, 16-step sequencer
- UART0 TX (PA1) sends MIDI bytes over USB serial
- Python script converts serial bytes to virtual MIDI (macOS IAC)
- DAW (Ex. Ableton) receives MIDI and triggers samples
- `SW1` controls playback
- `SW2` is reserved for future shift/alternate behavior



## Hardware

- TM4C123G LaunchPad (USB to Mac)
- EduBase V2 board (buttons, keypad, LCD, LEDs)



## Firmware (Keil)

1. Open project in Keil
2. Build + Flash

Key points:
- UART0 TX on **PA1**
- Baud rate: **115200**
- System Clock: 50 MHz

Run on board:
- SW1: Play/Stop
- SW2: Reserved for future shift functions
- Keypad: Toggle steps
- EduBase SW2: Track 1
- EduBase SW3: Track 2
- EduBase SW4: Track 3
- EduBase SW5: Track 4
- Track LEDs are shown as 
  - Track 1: LED3
  - Track 2: LED2
  - Track 3: LED1
  - Track 4: LED0


LCD legend:
- `X`: active step
- `.`: inactive step
- `*`: current step and active
- `-`: current step and inactive

Timing model:
- Timer0A runs as a 1 ms time base
- Step timing is derived from BPM as a sixteenth-note interval
- At 120 BPM, each step lasts 125 ms



## Mac Setup

### 1) Enable virtual MIDI

- Open **Audio MIDI Setup**
  - Window  **Show MIDI Studio**
- Double-click **IAC Driver**
- Check **Device is online**



### 2) Python environment

```bash
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```



### 3) Run bridge

Close any serial monitors first.

The bridge script currently uses hard-coded values for:
- serial port: `SERIAL_PORT`
- MIDI destination: `MIDI_PORT`

If your LaunchPad or virtual MIDI device uses different names, edit `tools/midi_bridge.py` before running it.

```bash
python3 tools/midi_bridge.py
```

You should see lines like:

```text
MIDI: note_on channel=0 note=46 velocity=100
```



## Ableton Setup

1. Preferences/Tempo & MIDI
   - Enable **Track** for `IAC Driver Bus 1`
2. Create MIDI track
3. Set:
   - **MIDI From**: `IAC Driver Bus 1`
   - Monitor: **IN**
4. Add **Drum Rack**
5. Place samples on incoming notes (e.g., 36 = C1)

Press play on the sequencer and should hear audio.



## Mapping

Map tracks to notes in firmware:

- Track 0: C1 (36)
- Track 1: C#1 (37)
- Track 2: D1 (38)
- Track 3: D#1 (39)



## Notes

- Opening the serial port can reset the board so press SW1 again
- If you see raw bytes but no MIDI, check system clock vs UART baud
- Only one app can use the serial port at a time
- EduBase buttons and keypad rows share `PD0-PD3`
- LCD data lines and keypad columns share `PA2-PA5`
- `SW2` shift behavior, tap tempo, and related alternate controls are future work and are not implemented in the current firmware


