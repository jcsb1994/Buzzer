
#include <Arduino.h>
#include "Buzzer.h"

#define BUZZER_PIN 7

Buzzer::Melody_t bootupMelody{.nbNotes = 3,
                              .duration = {400, 400, 800},
                              .frequency = {D4_NOTE_FREQ, E4_NOTE_FREQ, G4_NOTE_FREQ}};

Buzzer myBuzzer;

void setup()
{
  Serial.begin(9600);
  Serial.println("hi");
  myBuzzer.init(BUZZER_PIN, 0, 100);
  myBuzzer.setMelody(&bootupMelody);


  // TIMER INTERRUPT CODE GENERATED WITH http://www.8bit-era.cz/arduino-timer-interrupts-calculator.html
  ///////////////////////////////////////////////////////////////////
  // TIMER 1 for interrupt frequency 10 Hz:
  cli();      // stop interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1 = 0;  // initialize counter value to 0
  // set compare match register for 10 Hz increments
  OCR1A = 24999; // = 16000000 / (64 * 10) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12, CS11 and CS10 bits for 64 prescaler
  TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei(); // allow interrupts
}

void loop()
{
  if (myBuzzer.hasMelody())
  {
    Serial.println("I print while buzzer plays!");
  }
}

ISR(TIMER1_COMPA_vect){
  TCNT1  = 0;                  
  myBuzzer.step();
}