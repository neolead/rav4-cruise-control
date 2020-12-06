#pragma once

#include <Arduino.h>

#include "consts.h"
#include "utils.h"

struct LedItem {
    uint8_t _pin;
    bool _state;
    uint32_t _stateChangedTime;
    uint32_t _clickSequenceStarted;
    bool _clicks;
    LedItem(uint8_t pin) : _state{false}, _stateChangedTime{0},  _clickSequenceStarted{0}, _clicks{0} {};

    void init() {
        pinMode(_pin, OUTPUT);
    }

    void loop() {
        if (_state) {
            if (TimePassedSince(_stateChangedTime) > LED_LIGHT_TIME_ms) {                
                _state = false;
                refresh();
            }
        }
    }

    void onTrippleClickEvent() {
        Serial.printf("%lu pin(%d): %s\n", millis(), _pin, "tripple click");        
    }

    void handleClicks() {
        if (!_clickSequenceStarted) {
            _clickSequenceStarted = millis();                    
            _clicks = 1;
        } else {
            if (TimePassedSince(_clickSequenceStarted) < ONE_SECOND) {
                _clicks++;                
                if(_clicks >= 3)  {
                    onTrippleClickEvent();                            
                } else {
                    Serial.printf("%lu pin(%d) clicked: %d\n", millis(), _pin, _clicks);        
                    return;
                }
            }
        }                  
        _clicks = 0;
        _clickSequenceStarted = 0;
    }

    void onPress(bool newState) {
        if (_state != newState) {
            _state = newState;
            if (!_state) {
               handleClicks();
            }
            _stateChangedTime = millis();
            refresh();
        }
    }

    void refresh() {
        Serial.printf("%lu pin(%d): %s\n", millis(), _pin, _state ? "ON": "OFF");        
        digitalWrite(_pin, _state ? HIGH : LOW);
    }
};
