#include <Arduino.h>
#include <MIDI.h>

const int tempoPin = 13;  // Built-in LED on most Arduino boards

// Define which serial port to use for MIDI
#define MIDI_SERIAL Serial1

// Create the MIDI instance
MIDI_CREATE_INSTANCE(HardwareSerial, MIDI_SERIAL, MIDI);

const unsigned long pulseDuration = 50;  // LED on time in milliseconds
unsigned long lastPulseTime = 0;

void handleNoteOn(byte channel, byte note, byte velocity) {
  Serial.print("Note On: ch=");
  Serial.print(channel);
  Serial.print(", note=");
  Serial.print(note);
  Serial.print(", velocity=");
  Serial.println(velocity);
}

void handleNoteOff(byte channel, byte note, byte velocity) {
  Serial.print("Note Off: ch=");
  Serial.print(channel);
  Serial.print(", note=");
  Serial.print(note);
  Serial.print(", velocity=");
  Serial.println(velocity);
}

void handleControlChange(byte channel, byte number, byte value) {
  Serial.print("Control Change: ch=");
  Serial.print(channel);
  Serial.print(", number=");
  Serial.print(number);
  Serial.print(", value=");
  Serial.println(value);
}

void handleClock() {
  static byte clockCount = 0;
  clockCount++;

  // MIDI sends 24 clock messages per quarter note
  if (clockCount == 24) {  // Pulse LED every quarter note
    digitalWrite(tempoPin, HIGH);
    lastPulseTime = millis();
    // Serial.print(" 1/4 Note Pulse: ");
    clockCount = 0;
  }
}

void setup() {
  pinMode(tempoPin, OUTPUT);

  // Initialize MIDI communications
  MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all MIDI channels
  MIDI.turnThruOff();             // Disable automatic MIDI Thru

  // Set up callbacks for MIDI messages
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleClock(handleClock);

  // Initialize the default Serial for debugging
  Serial.begin(115200);  // Increased baud rate for faster printing
  while (!Serial) {
  }  // Wait for Serial to be ready
  Serial.println("MIDI LED Tempo Sync Started");
}

void loop() {
  MIDI.read();

  // Turn off LED after pulse duration
  if (millis() - lastPulseTime >= pulseDuration) {
    digitalWrite(tempoPin, LOW);
  }
}