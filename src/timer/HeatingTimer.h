#ifndef HEATING_TIMER_H
#define HEATING_TIMER_H

#include <Arduino.h>
#include "config/RodiConfig.h"

/**
 * @brief Heating timer class for automatic shutoff control
 * 
 * Manages countdown timer for heating element with automatic shutoff
 * when timer expires. Supports start, stop, and reset operations.
 */
class HeatingTimer {
public:
    /**
     * @brief Initialize the timer
     */
    void begin();
    
    /**
     * @brief Start timer with specified duration
     * @param durationMinutes Timer duration in minutes
     */
    void start(uint32_t durationMinutes);
    
    /**
     * @brief Set timer duration (without starting)
     * @param durationMinutes Timer duration in minutes
     */
    void setDuration(uint32_t durationMinutes);
    
    /**
     * @brief Stop the timer
     */
    void stop();
    
    /**
     * @brief Reset timer to initial duration
     */
    void reset();
    
    /**
     * @brief Update timer state (call in main loop)
     */
    void update();
    
    /**
     * @brief Check if timer is running
     */
    bool isRunning() const { return running; }
    
    /**
     * @brief Check if timer has expired
     */
    bool hasExpired() const { return expired; }
    
    /**
     * @brief Get remaining time in seconds
     */
    uint32_t getRemainingSeconds() const;
    
    /**
     * @brief Get remaining time in minutes (rounded up)
     */
    uint32_t getRemainingMinutes() const;
    
    /**
     * @brief Get total duration in minutes
     */
    uint32_t getDurationMinutes() const { return durationMinutes; }
    
    /**
     * @brief Clear expired flag (after handling expiration)
     */
    void clearExpired() { expired = false; }

private:
    uint32_t durationMinutes = TIMER_MIN_MIN;  // Timer duration in minutes
    uint32_t startTimeMillis = 0;              // Timer start time
    bool running = false;                       // Timer running state
    bool expired = false;                       // Timer expired flag
    
    /**
     * @brief Validate timer duration
     */
    bool isValidDuration(uint32_t minutes);
};

#endif // HEATING_TIMER_H

