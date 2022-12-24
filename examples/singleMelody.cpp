/* This example was tested to work on the Arduino Uno and ESP32 boards */
#include <Arduino.h>
#include "Buzzer.h"

#define BUZZER_PIN 12

Buzzer::Melody_t myMelody{.nbNotes = 3,
                              .duration = {400, 400, 800},
                              .frequency = {D4_NOTE_FREQ, E4_NOTE_FREQ, G4_NOTE_FREQ}};

Buzzer myBuzzer;

void setup()
{
  Serial.begin(9600);
  Serial.println("hello, buzzer single melody example!");
  myBuzzer.init(BUZZER_PIN);
  myBuzzer.setMelody(&myMelody);
}

void loop()
{
  if (myBuzzer.hasMelody())
  {
    Serial.println("I print while buzzer plays!");
  }
  myBuzzer.step();
}
