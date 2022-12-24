#include "Buzzer.h"
#include <stdint.h>

#if DEBUG_BUZZER
#define PRINT(msg) Serial.println(msg)
#else
#define PRINT(msg) 
#endif

#if DEBUG_BUZZER > 1
#define PRINT_VERBOSE(msg) Serial.println(msg)
#else
#define PRINT_VERBOSE(msg) 
#endif

void Buzzer::init(uint8_t pin, uint8_t channel)
{
    _pin = pin;
    _channel = channel;
    _init();
}

void Buzzer::setMelody(Buzzer::Melody_t *mel)
{
    _reset();
    _Melody = mel;
}

void Buzzer::mute()
{
    _muted = true;
}

void Buzzer::unmute()
{
    _muted = false;
}

bool Buzzer::isMuted()
{
    return _muted;
}

void Buzzer::pause()
{
    _ring(0);
    _state = S_BUZZER_PAUSED;
}

void Buzzer::resume()
{
    _state = S_BUZZER_PLAYING;
}

// Private definitions

void Buzzer::_init()
{
#ifdef ESP32
    ledcSetup(_channel, 2000, 8); // setup beeper at 2000, maybe lower?
    ledcAttachPin(_pin, _channel); 
#endif
}

void Buzzer::_reset()
{
    _state = S_BUZZER_IDLE;
    _Melody = NULL;
    _currNote = 0;
    _ring(0);
}

void Buzzer::_ring(uint16_t note)
{
#ifdef ESP32
    ledcWriteTone(_channel, note);
#else
    if(!note) {
        noTone(_pin);
    } else {
        tone(_pin, note);   
    }    
#endif
}

bool Buzzer::_isRingDone()
{
    unsigned long gap;
    PRINT_VERBOSE("Start time: " + String(_ringTimestamp));
    if (millis() < _ringTimestamp) {   // in case of overflow
      PRINT("Millis Overflow");
      gap = (UINT32_MAX - _ringTimestamp) + millis() + 1;
    } else {
      PRINT_VERBOSE("Calculating time between now and timer start: " + String(millis()) + " " + String(_ringTimestamp));
      gap = millis() - _ringTimestamp;
    }  
    if (gap >= _Melody->duration[_currNote]) {
      PRINT_VERBOSE(String(gap) + " is bigger than timeout period " + String(_Melody->duration[_currNote]));
      return true;
    } else {
      PRINT_VERBOSE("Not reached");
      return false;
    }
}


void Buzzer::step()
{
    if (_muted) { return; }
        
    switch (_state) {
    case S_BUZZER_PAUSED:
        break;
    case S_BUZZER_IDLE:
        if (_Melody != NULL) {
            _state = S_BUZZER_ACTIVE;
        }
        break;
    case S_BUZZER_ACTIVE:
        if(_currNote >= _Melody->nbNotes) {
            _reset();
            
        } else { 
            uint16_t sound = _Melody->frequency[_currNote];
            _ring(sound);
            _ringTimestamp = millis();
            _state = S_BUZZER_PLAYING;
        }
        break;
    case S_BUZZER_PLAYING:
    if (_isRingDone()) {
            _ring(0);
            _state = S_BUZZER_ACTIVE;
            _currNote++;
    }
        break;
    default:
        break;
    }
}
