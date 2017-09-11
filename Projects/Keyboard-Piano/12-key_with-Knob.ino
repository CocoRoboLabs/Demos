#include <SPI.h>
#include <SD.h>
#include <MusicPlayer.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define potPin A7

Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

MusicPlayer player;

int inst = 27;            // Instrument selection
int durTime = 60;         // Duration when playing a sound
int valVolume = 100;      // Global Volume
int initSound = 60;       // Init note, 60 is C4

int soundA = initSound;
int soundB = initSound + 1;
int soundC = initSound + 2;
int soundD = initSound + 3;
int soundE = initSound + 4;
int soundF = initSound + 5;
int soundG = initSound + 6;
int soundH = initSound + 7;
int soundI = initSound + 8;
int soundJ = initSound + 9;
int soundK = initSound + 10;
int soundL = initSound + 11;

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(5, 5, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

void setup() {
  Serial.begin(9600);

  pinMode(potPin, INPUT);

  // MIDI Setup
  player.beginMidi();      

  Serial.println("Adafruit MPR121 Capacitive Touch sensor test");
  if (!cap.begin(0x5A)) { Serial.println("MPR121 not found, check wiring?"); while (1); }
  Serial.println("MPR121 found!");

  // LED Board setup
  matrix.begin();         
  matrix.setBrightness(20);
}

void loop() {
  currtouched = cap.touched();

  uint8_t i;
  int readVal = analogRead(potPin);
  int realVal = map(readVal, 0, 1023, 1, 6);

  if ( realVal == 1 ) {
    inst = 3;
    Serial.println(realVal);
    matrix.fillScreen(0);
    matrix.fillRect(0, 0, 1, 5, matrix.Color(0, 255, 255));
    matrix.show();
  } else if (realVal == 2) {
    inst = 38;
    Serial.println(realVal);
    matrix.fillScreen(0);
    matrix.fillRect(1, 0, 1, 5, matrix.Color(0, 255, 255));
    matrix.show();
  } else if (realVal == 3) {
    inst = 117;
    Serial.println(realVal);
    matrix.fillScreen(0);
    matrix.fillRect(2, 0, 1, 5, matrix.Color(0, 255, 255));
    matrix.show();
  } else if (realVal == 4) {
    inst = 107;
    Serial.println(realVal);
    matrix.fillScreen(0);
    matrix.fillRect(3, 0, 1, 5, matrix.Color(0, 255, 255));
    matrix.show();
  } else if (realVal == 5) {
    inst = 107;
    Serial.println(realVal);
    matrix.fillScreen(0);
    matrix.fillRect(4, 0, 1, 5, matrix.Color(0, 255, 255));
    matrix.show();
  }

  for (uint8_t i = 0; i < 12; i++) {
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      switch (i) {
        case 6:
          MidiSoundA();
          Serial.print(i); Serial.println(" touched");
          break;
        case 7:
          MidiSoundB();
          Serial.print(i); Serial.println(" touched");
          break;
        case 8:
          MidiSoundC();
          Serial.print(i); Serial.println(" touched");
          break;
        case 9:
          MidiSoundD();
          Serial.print(i); Serial.println(" touched");
          break;
        case 10:
          MidiSoundE();
          Serial.print(i); Serial.println(" touched");
          break;
        case 11:
          MidiSoundF();
          Serial.print(i); Serial.println(" touched");
          break;
        case 0:
          MidiSoundG();
          Serial.print(i); Serial.println(" touched");
          break;
        case 1:
          MidiSoundH();
          Serial.print(i); Serial.println(" touched");
          break;
        case 2:
          MidiSoundI();
          Serial.print(i); Serial.println(" touched");
          break;
        case 3:
          MidiSoundJ();
          Serial.print(i); Serial.println(" touched");
          break;
        case 4:
          MidiSoundK();
          Serial.print(i); Serial.println(" touched");
          break;
        case 5:
          MidiSoundL();
          Serial.print(i); Serial.println(" touched");
          break;
      }
    }
  }

  // reset state
  lasttouched = currtouched;
}


/*----- MIDI DEFINITIONS ----*/

void midiSendByte(char data)
{
  SPI.transfer(0x00);
  SPI.transfer(data);
}

void midiWriteData(char cmd, char high, char low)
{
  while (!digitalRead(VS_DREQ));
  digitalWrite(VS_XDCS, LOW);
  midiSendByte(cmd);
  if ((cmd & 0xF0) <= 0xB0 || (cmd & 0xF0) >= 0xE0)
  {
    midiSendByte(high);
    midiSendByte(low);
  }
  else
  {
    midiSendByte(high);
  }
  digitalWrite(VS_XDCS, HIGH);
}

void midiNoteOff(char channel, char note, char rate)
{
  midiWriteData((0x80 | channel), note, rate);
}

void midiNoteOn(char channel, char note, char rate)
{
  midiWriteData((0x90 | channel), note, rate);
}

/* separate notes */

void MidiSound(int parInst, int parNote, int parTime) {
  midiWriteData(0xB0, 0x07, valVolume); // Set MIDI channel volume
  midiWriteData(0xB0, 0, 0x00);  // Select bank GM1
  midiWriteData(0xC0, parInst, 0); // Select instrument
  midiNoteOn(0, parNote, 127); // Play note
  delay(parTime);
  midiNoteOff(0, parNote, 127); // Stop playing note
}

void MidiSoundA()
{
  midiWriteData(0xB0, 0x07, valVolume); // Set MIDI channel volume
  midiWriteData(0xB0, 0, 0x00);  // Select bank GM1
  midiWriteData(0xC0, inst, 0); // Select instrument (Piano)
  midiNoteOn(0, soundA, 127); // Play note (C4)
  delay(durTime);
  midiNoteOff(0, soundA, 127); // Stop playing note
}

void MidiSoundB()
{
  midiWriteData(0xB0, 0x07, valVolume); // Set MIDI channel volume
  midiWriteData(0xB0, 0, 0x00);  // Select bank GM1
  midiWriteData(0xC0, inst, 0); // Select instrument (Piano)
  midiNoteOn(0, soundB, 127); // Play note (C4)
  delay(durTime);
  midiNoteOff(0, soundB, 127); // Stop playing note
}

void MidiSoundC()
{
  midiWriteData(0xB0, 0x07, valVolume); // Set MIDI channel volume
  midiWriteData(0xB0, 0, 0x00);  // Select bank GM1
  midiWriteData(0xC0, inst, 0); // Select instrument (Piano)
  midiNoteOn(0, soundC, 127); // Play note (C4)
  delay(durTime);
  midiNoteOff(0, soundC, 127); // Stop playing note
}

void MidiSoundD()
{
  midiWriteData(0xB0, 0x07, valVolume); // Set MIDI channel volume
  midiWriteData(0xB0, 0, 0x00);  // Select bank GM1
  midiWriteData(0xC0, inst, 0); // Select instrument (Piano)
  midiNoteOn(0, soundD, 127); // Play note (C4)
  delay(durTime);
  midiNoteOff(0, soundD, 127); // Stop playing note
}

void MidiSoundE()
{
  midiWriteData(0xB0, 0x07, valVolume); // Set MIDI channel volume
  midiWriteData(0xB0, 0, 0x00);  // Select bank GM1
  midiWriteData(0xC0, inst, 0); // Select instrument (Piano)
  midiNoteOn(0, soundE, 127); // Play note (C4)
  delay(durTime);
  midiNoteOff(0, soundE, 127); // Stop playing note
}

void MidiSoundF()
{
  midiWriteData(0xB0, 0x07, valVolume); // Set MIDI channel volume
  midiWriteData(0xB0, 0, 0x00);  // Select bank GM1
  midiWriteData(0xC0, inst, 0); // Select instrument (Piano)
  midiNoteOn(0, soundF, 127); // Play note (C4)
  delay(durTime);
  midiNoteOff(0, soundF, 127); // Stop playing note
}

void MidiSoundG()
{
  midiWriteData(0xB0, 0x07, valVolume); // Set MIDI channel volume
  midiWriteData(0xB0, 0, 0x00);  // Select bank GM1
  midiWriteData(0xC0, inst, 0); // Select instrument (Piano)
  midiNoteOn(0, soundG, 127); // Play note (C4)
  delay(durTime);
  midiNoteOff(0, soundG, 127); // Stop playing note
}

void MidiSoundH()
{
  midiWriteData(0xB0, 0x07, valVolume); // Set MIDI channel volume
  midiWriteData(0xB0, 0, 0x00);  // Select bank GM1
  midiWriteData(0xC0, inst, 0); // Select instrument (Piano)
  midiNoteOn(0, soundH, 127); // Play note (C4)
  delay(durTime);
  midiNoteOff(0, soundH, 127); // Stop playing note
}

void MidiSoundI()
{
  midiWriteData(0xB0, 0x07, valVolume); // Set MIDI channel volume
  midiWriteData(0xB0, 0, 0x00);  // Select bank GM1
  midiWriteData(0xC0, inst, 0); // Select instrument (Piano)
  midiNoteOn(0, soundI, 127); // Play note (C4)
  delay(durTime);
  midiNoteOff(0, soundI, 127); // Stop playing note
}

void MidiSoundJ()
{
  midiWriteData(0xB0, 0x07, valVolume); // Set MIDI channel volume
  midiWriteData(0xB0, 0, 0x00);  // Select bank GM1
  midiWriteData(0xC0, inst, 0); // Select instrument (Piano)
  midiNoteOn(0, soundJ, 127); // Play note (C4)
  delay(durTime);
  midiNoteOff(0, soundJ, 127); // Stop playing note
}

void MidiSoundK()
{
  midiWriteData(0xB0, 0x07, valVolume); // Set MIDI channel volume
  midiWriteData(0xB0, 0, 0x00);  // Select bank GM1
  midiWriteData(0xC0, inst, 0); // Select instrument (Piano)
  midiNoteOn(0, soundK, 127); // Play note (C4)
  delay(durTime);
  midiNoteOff(0, soundK, 127); // Stop playing note
}

void MidiSoundL()
{
  midiWriteData(0xB0, 0x07, valVolume); // Set MIDI channel volume
  midiWriteData(0xB0, 0, 0x00);  // Select bank GM1
  midiWriteData(0xC0, inst, 0); // Select instrument (Piano)
  midiNoteOn(0, soundL, 127); // Play note (C4)
  delay(durTime);
  midiNoteOff(0, soundL, 127); // Stop playing note
}
