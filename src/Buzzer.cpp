#include "Buzzer.h"
#include <Arduino.h>


void Buzzer::_init() {
#if USING_ESP32
    ledcSetup(_channel, 2000, 8); // setup beeper at 2000, maybe lower?
    ledcAttachPin(_pin, _channel); 
#endif
}

void Buzzer::_ring(uint16_t note) {
#if USING_ESP32
    ledcWriteTone(_channel, note);
#else
    tone(_pin, sound);
#endif
}

void Buzzer::_reset() { 
    _state = S_BUZZER_IDLE;
    _Melody = NULL;
    _currNote = 0;
    _ring(0); 
}

void Buzzer::setMelody(Buzzer::Melody_t *mel) {
    _reset();
    _Melody = mel;
}

void Buzzer::mute() {
    _muted = true;
}

void Buzzer::unmute() {
    _muted = false;
}

bool Buzzer::isMuted() {
    return _muted;
}

void Buzzer::pause() {    
    _ring(0);
    _state = S_BUZZER_PAUSED;
}

void Buzzer::resume() {    
    _state = S_BUZZER_PLAYING;
}

void Buzzer::step() {
    if (_muted)
        return;
        
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
            // tone(_pin, sound);
            _state = S_BUZZER_PLAYING;
        }
        break;
    case S_BUZZER_PLAYING:
    if ((++_stepCount * _stepPeriod) >= _Melody->duration[_currNote]) {
            // noTone(_pin);
            _ring(0); 
            _stepCount = 0;
            _state = S_BUZZER_ACTIVE;
            _currNote++;
    }
        break;
    default:
        break;
    }
}
