#ifndef SAFETY_MONITOR_H
#define SAFETY_MONITOR_H

#include <Arduino.h>
#include <esp_task_wdt.h>
#include "config/RodiConfig.h"

/**
 * @brief Safety monitor class with watchdog timer and system health checks
 * 
 * Provides system-level safety monitoring including watchdog timer,
 * sensor health validation, and emergency shutoff coordination.
 */
class SafetyMonitor {
public:
    /**
     * @brief Initialize safety monitor and watchdog
     */
    void begin();
    
    /**
     * @brief Reset watchdog timer (call regularly in main loop)
     */
    void feedWatchdog();
    
    /**
     * @brief Update safety status
     * @param mattressSensorHealthy Mattress sensor health status
     * @param roomSensorHealthy Room sensor health status
     */
    void update(bool mattressSensorHealthy, bool roomSensorHealthy);
    
    /**
     * @brief Check if system is in safe state
     * @return true if all safety checks pass
     */
    bool isSafe() const { return safe; }
    
    /**
     * @brief Check if critical sensor (mattress) is healthy
     */
    bool isCriticalSensorHealthy() const { return criticalSensorHealthy; }
    
    /**
     * @brief Get system uptime in seconds
     */
    uint32_t getUptimeSeconds() const;
    
    /**
     * @brief Get last reset reason
     */
    void printResetReason();

private:
    bool safe = true;
    bool criticalSensorHealthy = true;
    bool watchdogEnabled = false;
    uint32_t bootTimeMillis = 0;
};

#endif // SAFETY_MONITOR_H

