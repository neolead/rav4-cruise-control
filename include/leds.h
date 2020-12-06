#pragma once

#include <Arduino.h>

#include "consts.h"
#include "utils.h"

struct LedItem {
    uint8_t _pin;
    bool _ledState;
    bool _controlState;
    uint32_t _stateChangedTime;
    uint32_t _clickSequenceStarted;
    bool _clicks;
    LedItem(uint8_t pin) : _ledState{false}, _controlState{false}, _stateChangedTime{0},  _clickSequenceStarted{0}, _clicks{0} {};

    void init() {
        pinMode(_pin, OUTPUT);
    }

    void loop() {
        if (_ledState) {
            if (TimePassedSince(_stateChangedTime) > LED_LIGHT_TIME_ms) {                
                _ledState = false;
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
                return;
            }
        }                  
        _clicks = 0;
        _clickSequenceStarted = 0;
    }

    void onStateChange(bool newState) {
        if (_controlState != newState) {
            _controlState = newState;
            if (_controlState) { 
                _ledState = true;                            
            } else {
                handleClicks();
            }
            refresh();    
            _stateChangedTime = millis();
        }
    }

    void refresh() {
        Serial.printf("%lu pin(%d): %s\n", millis(), _pin, _ledState ? "ON": "OFF");        
        digitalWrite(_pin, _ledState ? HIGH : LOW);
    }
};
