#pragma once

#include <Arduino.h>

inline  int32_t TimeDifference(uint32_t prev, uint32_t next) {
    return ((int32_t)(next - prev));
}

inline int32_t TimePassedSince(uint32_t timestamp) {
    // Compute the number of milliSeconds passed since timestamp given.
    // Note: value can be negative if the timestamp has not yet been reached.
    return TimeDifference(timestamp, millis());
}

inline int32_t TimePassedSinceUsec(uint32_t timestamp) {
    return TimeDifference(timestamp, micros());
}
