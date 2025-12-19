#include "HeatingTimer.h"

void HeatingTimer::begin() {
    running = false;
    expired = false;
    Serial.println("[HeatingTimer] Timer initialized");
}

void HeatingTimer::start(uint32_t durationMin) {
    if (!isValidDuration(durationMin)) {
        Serial.printf("[HeatingTimer] ERROR: Invalid duration %u min (range: %d-%d min)\n",
                     durationMin, TIMER_MIN_MIN, TIMER_MAX_MIN);
        return;
    }
    
    durationMinutes = durationMin;
    startTimeMillis = millis();
    running = true;
    expired = false;
    
    Serial.printf("[HeatingTimer] Timer started - Duration: %u minutes (%u seconds)\n",
                 durationMinutes, durationMinutes * 60);
}

void HeatingTimer::setDuration(uint32_t durationMin) {
    if (!isValidDuration(durationMin)) {
        Serial.printf("[HeatingTimer] ERROR: Invalid duration %u min (range: %d-%d min)\n",
                     durationMin, TIMER_MIN_MIN, TIMER_MAX_MIN);
        return;
    }
    
    durationMinutes = durationMin;
    
    // If timer is running, restart with new duration
    if (running) {
        startTimeMillis = millis();
        expired = false;
        Serial.printf("[HeatingTimer] Timer duration updated and restarted - Duration: %u minutes\n",
                     durationMinutes);
    } else {
        Serial.printf("[HeatingTimer] Timer duration set - Duration: %u minutes (not started)\n",
                     durationMinutes);
    }
}

void HeatingTimer::stop() {
    if (running) {
        running = false;
        Serial.println("[HeatingTimer] Timer stopped");
    }
}

void HeatingTimer::reset() {
    if (running) {
        startTimeMillis = millis();
        expired = false;
        Serial.printf("[HeatingTimer] Timer reset - Duration: %u minutes\n", durationMinutes);
    }
}

void HeatingTimer::update() {
    if (!running || expired) {
        return;
    }
    
    // Calculate elapsed time
    uint32_t elapsedMillis = millis() - startTimeMillis;
    uint32_t durationMillis = durationMinutes * 60UL * 1000UL;
    
    // Check for expiration
    if (elapsedMillis >= durationMillis) {
        running = false;
        expired = true;
        Serial.println("[HeatingTimer] *** TIMER EXPIRED - AUTO SHUTOFF ***");
    }
}

uint32_t HeatingTimer::getRemainingSeconds() const {
    if (!running) {
        return 0;
    }
    
    uint32_t elapsedMillis = millis() - startTimeMillis;
    uint32_t durationMillis = durationMinutes * 60UL * 1000UL;
    
    if (elapsedMillis >= durationMillis) {
        return 0;
    }
    
    uint32_t remainingMillis = durationMillis - elapsedMillis;
    return remainingMillis / 1000;
}

uint32_t HeatingTimer::getRemainingMinutes() const {
    uint32_t remainingSeconds = getRemainingSeconds();
    
    // Round up to nearest minute
    return (remainingSeconds + 59) / 60;
}

bool HeatingTimer::isValidDuration(uint32_t minutes) {
    return (minutes >= TIMER_MIN_MIN && minutes <= TIMER_MAX_MIN);
}

