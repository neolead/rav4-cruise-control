#pragma once

#include <Arduino.h>

#include "consts.h"
#include "utils.h"

struct LedItem {
    uint8_t _pin;
    bool _handleClicks;
    bool _ledState;
    bool _controlState;
    uint32_t _stateChangedTime;
    uint32_t _clickSequenceStarted;
    bool _clicks;
    LedItem(uint8_t pin, bool handleClicks) : _pin{pin},
                                              _handleClicks{handleClicks},
                                              _controlState{false},
                                              _stateChangedTime{0},
                                              _clickSequenceStarted{0},
                                              _clicks{0}{};

    LedItem(uint8_t pin) : LedItem(pin, false) {};
    
    void init() {
        pinMode(_pin, OUTPUT);
    }

    void setHandleClicks(bool value = true) {
        _handleClicks = value;
    }

    void loop() {
        if (_ledState) {
            if (TimePassedSince(_stateChangedTime) > LED_LIGHT_TIME_ms) {
                _ledState = false;
                Serial.printf("\n%lu pin(%d): %s\n", millis(), _pin, "led off");
                refresh();
            }
        }
    }

    void onTrippleClickEvent() {
        Serial.printf("\n%lu pin(%d): %s\n", millis(), _pin, "tripple click");
    }

    void handleClicks() {
        // последовательность не начата
        if (!_clickSequenceStarted) {
            _clickSequenceStarted = millis();
            _clicks = 1;
            return;
        }
        // последовательность начата и прошло менее 1 сек
        if (_clickSequenceStarted && (TimePassedSince(_clickSequenceStarted) < ONE_SECOND)) {
            _clicks++;
            if (_clicks == 3) { //1 клик то
                onTrippleClickEvent();
            } else {
                Serial.printf("\n%lu pin(%d) clicked: %d\n", millis() - _clickSequenceStarted, _pin, _clicks);
            }
            return;
        }
        // Ничего из выше описанного
        _clicks = 0;
        _clickSequenceStarted = 0;
    }

    void onStateChange(bool newState) {
        if (_controlState != newState) {
            _controlState = newState;
            if (_controlState) {
                Serial.printf("\n%lu pin(%d): %s\n", millis(), _pin, "led on");
                _ledState = true;
            } else {
                if (_handleClicks) {
                    handleClicks();
                };
            }
            refresh();
            _stateChangedTime = millis();
        }
    }

    void refresh() {
        digitalWrite(_pin, _ledState ? HIGH : LOW);
    }
};
