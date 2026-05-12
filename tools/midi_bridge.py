"""
midi_bridge.py

Serial-to-MIDI bridge for the Embedded MIDI Sequencer project.

The TM4C123 sends raw MIDI-style bytes over UART0 through the LaunchPad
USB debug serial port. This script reads those bytes from the Mac serial
port and forwards them to the macOS IAC virtual MIDI bus so Ableton Live
can receive them.
"""

import serial
import mido

SERIAL_PORT = "/dev/cu.usbmodem0E234F551"
BAUD_RATE = 115200
MIDI_PORT = "IAC Driver Bus 1"
DEBUG_RAW_BYTES = True


def open_serial_port():
    """Open the LaunchPad serial port."""
    return serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.1)


def open_midi_output():
    """Open the macOS IAC virtual MIDI output port."""
    return mido.open_output(MIDI_PORT)


def parse_midi_stream(ser, outport):
    """
    Read UART bytes forever and rebuild 3-byte MIDI messages.

    UART is a byte stream. Not every read contains one
    complete MIDI message. This function buffers incoming bytes, looks for
    valid MIDI status bytes, then forwards complete Note On and Note Off
    messages to the virtual MIDI output.
    """
    buffer = bytearray()

    while True:
        incoming_byte = ser.read(1)

        if not incoming_byte:
            continue

        if DEBUG_RAW_BYTES:
            print(f"raw byte: 0x{incoming_byte[0]:02X}")

        buffer += incoming_byte

        while len(buffer) >= 3:
            status = buffer[0]

            # A valid MIDI status byte has bit 7 set.
            # If the first byte is not a status byte, discard it and resync.
            if (status & 0x80) == 0:
                buffer.pop(0)
                continue

            note = buffer[1]
            velocity = buffer[2]

            command = status & 0xF0
            channel = status & 0x0F

            if command == 0x90:
                msg = mido.Message(
                    "note_on",
                    channel=channel,
                    note=note,
                    velocity=velocity,
                )
            elif command == 0x80:
                msg = mido.Message(
                    "note_off",
                    channel=channel,
                    note=note,
                    velocity=velocity,
                )
            else:
                # Unknown MIDI message type for this project. Drop the
                # status byte and keep searching for the next message.
                buffer.pop(0)
                continue

            outport.send(msg)
            print(f"MIDI: {msg}")

            # Remove the three bytes that formed this complete message.
            buffer = buffer[3:]


def main():
    ser = open_serial_port()
    outport = open_midi_output()

    print(f"Listening on {SERIAL_PORT}")
    print(f"Sending MIDI to {MIDI_PORT}")
    print("Press Ctrl+C to stop.")

    try:
        parse_midi_stream(ser, outport)
    except KeyboardInterrupt:
        print("\nStopping MIDI bridge.")
    finally:
        ser.close()
        outport.close()


if __name__ == "__main__":
    main()
