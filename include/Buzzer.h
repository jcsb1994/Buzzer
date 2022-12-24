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
//    The user must call buzzer.step() in the main loop.
//    A melody is initialized with buzzer.setMelody(). The tones will ring, when stepping
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

#define C4_NOTE_FREQ (261)
#define D4_NOTE_FREQ (293)
#define E4_NOTE_FREQ (329)
#define F4_NOTE_FREQ (349)
#define G4_NOTE_FREQ (392)
#define A4_NOTE_FREQ (440)
#define B4_NOTE_FREQ (494)
#define C5_NOTE_FREQ (523)
#define E5_NOTE_FREQ (659)
#define F5_NOTE_FREQ (698)
#define G5_NOTE_FREQ (784)
#define A5_NOTE_FREQ (880)
#define B5_NOTE_FREQ (987)

#define MELODY_NOTE_MAX_NB (3)

class Buzzer {
  public:

    typedef struct {
        uint8_t nbNotes;
        uint16_t duration[MELODY_NOTE_MAX_NB];
        uint16_t frequency[MELODY_NOTE_MAX_NB];
    } Melody_t;

/*! @param pin GPIO the buzzer is connected to
    @param channel Only needed with ESP32 */
    Buzzer(uint8_t pin, uint8_t channel = 0) : _pin(pin), _channel(channel) {
        _init();
    }
    Buzzer() {}
    ~Buzzer() {}

/*! @brief  Initialization to use with the empty constructor */
    void init(uint8_t pin, uint8_t channel = 0);

/*! @brief  Sets a melody to start playing. will play as step() is called
    @param  mel melody_t struct containing the notes to be played */
    void setMelody(Melody_t *mel);
    bool hasMelody() { return (_Melody != NULL) ? true : false; }

/*! @brief  Steps through the state machine.
    @note This must be called in the main loop
    @attention Use setMelody() to add a melody to the state machine. */
    void step();

/*! @brief  Mutes the buzzer, but does not pause the melody queue */
    void mute();
    void unmute();
    bool isMuted();

/*! @brief  Stops playing but resume playing the same melody when resume() is called */
    void pause();
    void resume();

/***************************************************************************/
  private:
    uint8_t _pin;
    uint8_t _channel; // 0 TO 15
    
    typedef enum { S_BUZZER_IDLE, S_BUZZER_ACTIVE, S_BUZZER_PLAYING, S_BUZZER_PAUSED } BuzzerState_t;
    BuzzerState_t _state;
    Melody_t *_Melody;
    uint8_t _currNote;
    unsigned long _ringTimestamp;
    bool _muted = false;
  
    void _init();

/*! @brief  Resets the state machine, called in the stepping function when 
    melody is complete */
    void _reset();

    void _ring(uint16_t note);
    bool _isRingDone();
};

#endif