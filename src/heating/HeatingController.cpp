#include "HeatingController.h"

void HeatingController::begin() {
    // Configure relay pin as output
    pinMode(HEATING_RELAY_PIN, OUTPUT);
    
    // Ensure safe state (OFF) on initialization
    setRelay(false);
    heatingState = false;
    enabled = false;
    
    Serial.println("[HeatingController] Initialized - Heating OFF (safe state)");
}

void HeatingController::update(float currentTemp, float setpoint, bool sensorHealthy) {
    // Safety check: Disable heating if sensor is unhealthy
    if (!sensorHealthy) {
        if (heatingState) {
            Serial.println("[HeatingController] *** SAFETY SHUTOFF: Sensor unhealthy ***");
            forceOff();
        }
        return;
    }
    
    // Check if heating is enabled
    if (!enabled) {
        if (heatingState) {
            setRelay(false);
            heatingState = false;
        }
        return;
    }
    
    // Hysteresis control algorithm
    if (!heatingState) {
        // Heating is OFF - check if we should turn ON
        if (currentTemp < (setpoint - hysteresis)) {
            setRelay(true);
            heatingState = true;
            Serial.printf("[HeatingController] Heating ON - Temp: %.2f°C < (%.1f°C - %.1f°C)\n",
                         currentTemp, setpoint, hysteresis);
        }
    } else {
        // Heating is ON - check if we should turn OFF
        if (currentTemp >= setpoint) {
            setRelay(false);
            heatingState = false;
            Serial.printf("[HeatingController] Heating OFF - Temp: %.2f°C >= %.1f°C\n",
                         currentTemp, setpoint);
        }
    }
}

void HeatingController::enable() {
    if (!enabled) {
        enabled = true;
        Serial.println("[HeatingController] Heating control ENABLED");
    }
}

void HeatingController::disable() {
    if (enabled) {
        enabled = false;
        forceOff();
        Serial.println("[HeatingController] Heating control DISABLED");
    }
}

void HeatingController::forceOff() {
    if (heatingState) {
        setRelay(false);
        heatingState = false;
        Serial.println("[HeatingController] Heating FORCED OFF");
    }
}

void HeatingController::setSetpoint(float temp) {
    if (isValidSetpoint(temp)) {
        setpoint = temp;
        Serial.printf("[HeatingController] Setpoint changed to %.1f°C\n", temp);
    } else {
        Serial.printf("[HeatingController] ERROR: Invalid setpoint %.1f°C (range: %.0f-%.0f°C)\n",
                     temp, TEMP_MIN_C, TEMP_MAX_C);
    }
}

void HeatingController::setRelay(bool state) {
    digitalWrite(HEATING_RELAY_PIN, state ? HIGH : LOW);
    
    #if DEBUG_ENABLED
    Serial.printf("[HeatingController] Relay GPIO %d set to %s\n", 
                 HEATING_RELAY_PIN, state ? "HIGH" : "LOW");
    #endif
}

bool HeatingController::isValidSetpoint(float temp) {
    return (temp >= TEMP_MIN_C && temp <= TEMP_MAX_C);
}

