#pragma once

#include <Arduino.h>

#define LED_LIGHT_TIME_ms 150

struct LedItem {
    uint8_t pin;
    bool state;
    uint32_t stateOnTime;
    LedItem(uint8_t pin) : state{false}, stateOnTime{0} {};

    void init() {
        pinMode(pin, OUTPUT);
    }

    void loop() {
        if (state && (stateOnTime + LED_LIGHT_TIME_ms) < millis()) {
            setState(false);
        }
    }

    void setState(bool newState) {
        bool oldState = state;
        if (oldState != newState) {
            state = newState;
            stateOnTime = state ? millis() : 0;
            refresh();
        }
    }

    void refresh() {
        Serial.printf("pin(%d): %d", pin, state);
        digitalWrite(pin, state ? HIGH : LOW);
    }
};
