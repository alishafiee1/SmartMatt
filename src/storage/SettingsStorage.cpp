#include "SettingsStorage.h"

void SettingsStorage::begin() {
    preferences.begin(SETTINGS_NAMESPACE, false);  // Read-write mode
    Serial.println("[SettingsStorage] Preferences initialized");
    
    // Load existing settings or use defaults
    load();
}

void SettingsStorage::load() {
    // Load temperature setpoint
    float loadedTemp = preferences.getFloat(SETTINGS_KEY_TEMP, TEMP_DEFAULT_C);
    if (isValidTemperature(loadedTemp)) {
        temperatureSetpoint = loadedTemp;
    } else {
        Serial.printf("[SettingsStorage] Invalid stored temperature: %.1f°C - using default\n", 
                     loadedTemp);
        temperatureSetpoint = TEMP_DEFAULT_C;
    }
    
    // Load timer duration
    uint32_t loadedTimer = preferences.getUInt(SETTINGS_KEY_TIMER, TIMER_MIN_MIN);
    if (isValidTimer(loadedTimer)) {
        timerDuration = loadedTimer;
    } else {
        Serial.printf("[SettingsStorage] Invalid stored timer: %u min - using default\n", 
                     loadedTimer);
        timerDuration = TIMER_MIN_MIN;
    }
    
    // Load heating state (always OFF on boot for safety)
    heatingEnabled = SAFE_BOOT_HEATING_STATE;
    
    valid = true;
    
    Serial.printf("[SettingsStorage] Loaded settings - Temp: %.1f°C, Timer: %u min, Heating: %s\n",
                 temperatureSetpoint, timerDuration, heatingEnabled ? "ON" : "OFF");
}

bool SettingsStorage::save() {
    if (!valid) {
        Serial.println("[SettingsStorage] ERROR: Cannot save invalid settings");
        return false;
    }
    
    // Save temperature setpoint
    preferences.putFloat(SETTINGS_KEY_TEMP, temperatureSetpoint);
    
    // Save timer duration
    preferences.putUInt(SETTINGS_KEY_TIMER, timerDuration);
    
    // Save heating state
    preferences.putBool(SETTINGS_KEY_HEATING, heatingEnabled);
    
    Serial.printf("[SettingsStorage] Settings saved - Temp: %.1f°C, Timer: %u min, Heating: %s\n",
                 temperatureSetpoint, timerDuration, heatingEnabled ? "ON" : "OFF");
    
    return true;
}

void SettingsStorage::resetToDefaults() {
    temperatureSetpoint = TEMP_DEFAULT_C;
    timerDuration = TIMER_MIN_MIN;
    heatingEnabled = SAFE_BOOT_HEATING_STATE;
    valid = true;
    
    Serial.println("[SettingsStorage] Settings reset to defaults");
    
    save();
}

void SettingsStorage::setTemperatureSetpoint(float temp) {
    if (isValidTemperature(temp)) {
        temperatureSetpoint = temp;
        Serial.printf("[SettingsStorage] Temperature setpoint set to %.1f°C\n", temp);
    } else {
        Serial.printf("[SettingsStorage] ERROR: Invalid temperature %.1f°C (range: %.0f-%.0f°C)\n",
                     temp, TEMP_MIN_C, TEMP_MAX_C);
    }
}

void SettingsStorage::setTimerDuration(uint32_t minutes) {
    if (isValidTimer(minutes)) {
        timerDuration = minutes;
        Serial.printf("[SettingsStorage] Timer duration set to %u minutes\n", minutes);
    } else {
        Serial.printf("[SettingsStorage] ERROR: Invalid timer %u min (range: %d-%d min)\n",
                     minutes, TIMER_MIN_MIN, TIMER_MAX_MIN);
    }
}

void SettingsStorage::setHeatingEnabled(bool enabled) {
    heatingEnabled = enabled;
    Serial.printf("[SettingsStorage] Heating %s\n", enabled ? "ENABLED" : "DISABLED");
}

bool SettingsStorage::isValidTemperature(float temp) {
    return (temp >= TEMP_MIN_C && temp <= TEMP_MAX_C);
}

bool SettingsStorage::isValidTimer(uint32_t minutes) {
    return (minutes >= TIMER_MIN_MIN && minutes <= TIMER_MAX_MIN);
}

