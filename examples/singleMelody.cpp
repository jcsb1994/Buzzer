
#include <Arduino.h>
#include "Buzzer.h"

#define BUZZER_PIN 21

Buzzer::Melody_t bootupMelody{.nbNotes = 3,
                    .duration = {400, 400, 800},
                    .frequency = {D4_NOTE_FREQ, E4_NOTE_FREQ, G4_NOTE_FREQ}};

Buzzer myBuzzer;

void setup() {
  Serial.begin(9600);
  Serial.println("hi");
  myBuzzer.init(BUZZER_PIN, 0, 100);
  myBuzzer.setMelody(&bootupMelody); 
}

void loop() {
    delay(100);
    myBuzzer.step();
    if(myBuzzer.hasMelody()) {
      Serial.println("I print while buzzer plays!");
    }
}