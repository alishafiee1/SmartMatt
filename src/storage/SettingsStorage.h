#ifndef SETTINGS_STORAGE_H
#define SETTINGS_STORAGE_H

#include <Arduino.h>
#include <Preferences.h>
#include "config/RodiConfig.h"

/**
 * @brief Settings storage class using ESP32 Preferences (NVS)
 * 
 * Manages persistent storage of temperature setpoint, timer duration,
 * and heating state. Provides default values on first boot or corruption.
 */
class SettingsStorage {
public:
    /**
     * @brief Initialize settings storage
     */
    void begin();
    
    /**
     * @brief Load all settings from NVS
     */
    void load();
    
    /**
     * @brief Save all settings to NVS
     * @return true if save successful
     */
    bool save();
    
    /**
     * @brief Reset all settings to defaults
     */
    void resetToDefaults();
    
    // Temperature setpoint
    float getTemperatureSetpoint() const { return temperatureSetpoint; }
    void setTemperatureSetpoint(float temp);
    
    // Timer duration (in minutes)
    uint32_t getTimerDuration() const { return timerDuration; }
    void setTimerDuration(uint32_t minutes);
    
    // Heating enabled state
    bool isHeatingEnabled() const { return heatingEnabled; }
    void setHeatingEnabled(bool enabled);
    
    /**
     * @brief Check if settings are valid
     */
    bool isValid() const { return valid; }

private:
    Preferences preferences;
    
    float temperatureSetpoint = TEMP_DEFAULT_C;
    uint32_t timerDuration = TIMER_MIN_MIN;  // Default to minimum
    bool heatingEnabled = SAFE_BOOT_HEATING_STATE;
    bool valid = false;
    
    /**
     * @brief Validate temperature setpoint range
     */
    bool isValidTemperature(float temp);
    
    /**
     * @brief Validate timer duration range
     */
    bool isValidTimer(uint32_t minutes);
};

#endif // SETTINGS_STORAGE_H

