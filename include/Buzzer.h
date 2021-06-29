//***********************************************************************************
// Copyright 2021 jcsb1994
// Written by jcsb1994
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//***********************************************************************************
//
// Description:
//    This file contains a high level interface for a buzzer. It lets the
//    user create melodies and feed them into the object without blocking operations.
//    The instance steps every ISR. step delay is specified in the ctor.
//    a melody is initialized with setMelody(). the tone will ring, when stepping
//    is complete, melody is cleared
//    https://github.com/espressif/arduino-esp32/issues/1720
//      
// Implementation:
//    Implemented with Arduino.h and tone() to control note frequencies
//    On the ESP32, reimplemented with the ledcWriteTone() function
//
//***********************************************************************************

#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

#define D4_NOTE_FREQ (293)
#define E4_NOTE_FREQ (329)
#define F4_NOTE_FREQ (349)
#define G4_NOTE_FREQ (392)
#define A4_NOTE_FREQ (440)
#define B4_NOTE_FREQ (494)
#define C5_NOTE_FREQ (523)
#define E5_NOTE_FREQ (659)

#define MELODY_NOTE_MAX_NB (3)

#if USING_ESP32 0

class Buzzer {
  public:

    typedef struct {
        uint8_t nbNotes;
        uint16_t duration[MELODY_NOTE_MAX_NB];
        uint16_t frequency[MELODY_NOTE_MAX_NB];
    } Melody_t;

    Buzzer(uint8_t pin, uint8_t channel, uint16_t stepPeriod)
        : _pin(pin), _stepPeriod(stepPeriod), _channel(channel) {
          _init();
        }

    ~Buzzer() {}

/***************************************************************************/
  /*!
    @brief  Sets a melody to start playing. will play as step() is called
    @param  mel melody_t struct containing the notes to be played
  */
/***************************************************************************/
    void setMelody(Melody_t *mel);

/***************************************************************************/
  /*!
    @brief  Mutes the buzzer, but does not pause the melodie queue
    @param  mel melody_t struct containing the notes to be played
  */
/***************************************************************************/
    void mute();

    void unmute();

    bool isMuted();

/***************************************************************************/
  /*!
    @brief  Stops playing but resume playing the same melody when 
    @param  mel melody_t struct containing the notes to be played
  */
/***************************************************************************/
    void pause();
    
    void resume();

/***************************************************************************/
  /*!
    @brief  Steps through the state machine. Must be called in an ISR or recurring 
    handler with the specified stepPeriod in the constructor. 
    Use setMelody() to add a melody to the state machine. 
    @param  none
  */
/***************************************************************************/
    void step();

  private:
    typedef enum { S_BUZZER_IDLE, S_BUZZER_ACTIVE, S_BUZZER_PLAYING, S_BUZZER_PAUSED } BuzzerState_t;
    const uint8_t _pin;
    const uint16_t _stepPeriod;
    const uint8_t _channel; // 0 TO 15
    Melody_t *_Melody;

    bool _muted = false;
    uint16_t _stepCount;
    uint8_t _currNote;
    BuzzerState_t _state;

    void _init();

    void _ring(uint16_t note);

/***************************************************************************/
  /*!
    @brief  Resets the state machine, called in the stepping function when 
    melody is complete
    @param  none
  */
/***************************************************************************/
    void _reset();  

};

#endif