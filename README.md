# Embedded MIDI Step Sequencer

Setup to run the hardware sequencer and trigger sounds in DAW via a serial to MIDI bridge.

---



## Overview

- TM4C123 LaunchPad + EduBase V2
- 4-track, 16-step sequencer
- UART0 TX (PA1) sends MIDI bytes over USB serial
- Python script converts serial bytes to virtual MIDI (macOS IAC)
- DAW (Ex. Ableton) receives MIDI and triggers samples



## Hardware

- TM4C123G LaunchPad (USB to Mac)
- EduBase V2 board (buttons, keypad, LCD, LEDs)



## Firmware (Keil)

1. Open project in Keil
3. Build + Flash

Key points:
- UART0 TX on **PA1**
- Baud rate: **115200**
- System Clock: 50 MHz

Run on board:
- SW1: Play/Stop
- Keypad: Toggle steps
- SW2–SW5: Select track



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
pip install pyserial mido python-rtmidi
```



### 3) Run bridge

Close any serial monitors first.

```bash
python tools/midi_bridge.py
```

You should see lines like:

```text
MIDI: note_on channel=0 note=46 velocity=100
```



## Ableton Setup

1. Preferences/Temp & MIDI
   - Enable **Track** for `IAC Driver Bus 1`
2. Create MIDI track
3. Set:
   - **MIDI From**: `IAC Driver Bus 1`
   - Monitor: **IN**
4. Add **Drum Rack**
5. Place samples on incoming notes (e.g., 46 = A#1)

Press play on the sequencer and should hear audio.



## Notes / Tips

- Opening the serial port can reset the board so press SW1 again
- If you see raw bytes but no MIDI, check system clock vs UART baud
- Only one app can use the serial port at a time



## Optional Mapping

Map tracks to notes in firmware:

- Track 0 (Kick): A#1 (46)
- Track 1 (Snare): F#1 (42)
- Track 2 (Closed Hat): D1 (38)
- Track 3 (Open Hat): C1 (36)



