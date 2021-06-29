#include "Buzzer.h"
#include <Arduino.h>

void Buzzer::_reset() { 
        _state = S_BUZZER_IDLE;
        _Melody = NULL;
        _currNote = 0;
        ledcWriteTone(_channel, 0); 
}

void Buzzer::setMelody(Melody_t *mel) {
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
    ledcWriteTone(_channel, 0); 
    _state = S_BUZZER_PAUSED;
}

void Buzzer::resume() {    
    ledcWriteTone(_channel, 0); 
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
            ledcWriteTone(_channel, sound);                                                                                                                                                     
            // tone(_pin, sound);
            _state = S_BUZZER_PLAYING;
        }
        break;
    case S_BUZZER_PLAYING:
    if ((++_stepCount * _stepPeriod) >= _Melody->duration[_currNote]) {
            // noTone(_pin);
            ledcWriteTone(_channel, 0); 
            _stepCount = 0;
            _state = S_BUZZER_ACTIVE;
            _currNote++;
    }
        break;
    default:
        break;
    }
}
