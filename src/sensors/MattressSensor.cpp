#include "MattressSensor.h"

void MattressSensor::begin() {
    sensor.begin();
    
    // Check if sensor is physically connected
    uint8_t deviceCount = sensor.getDeviceCount();
    
    if (deviceCount > 0) {
        connected = true;
        sensor.setResolution(12);  // 12-bit resolution (0.0625°C precision)
        sensor.setWaitForConversion(false);  // Async mode
        
        Serial.printf("[MattressSensor] DS18B20 initialized successfully (%d device(s))\n", 
                     deviceCount);
        
        // Initial read to validate
        sensor.requestTemperatures();
        delay(750);  // Wait for 12-bit conversion (750ms max)
        read();
    } else {
        connected = false;
        healthy = false;
        Serial.println("[MattressSensor] CRITICAL: No DS18B20 sensor detected!");
    }
}

bool MattressSensor::read() {
    // Check if sensor is connected
    if (!connected) {
        uint8_t deviceCount = sensor.getDeviceCount();
        if (deviceCount > 0) {
            connected = true;
            Serial.println("[MattressSensor] Sensor reconnected");
        } else {
            failureCount++;
            healthy = false;
            Serial.println("[MattressSensor] ERROR: Sensor disconnected");
            return false;
        }
    }
    
    // Request temperature reading
    sensor.requestTemperatures();
    
    // Attempt to read with retries
    for (uint8_t attempt = 0; attempt < SENSOR_MAX_RETRIES; attempt++) {
        float newTemp = sensor.getTempCByIndex(0);
        
        // Check for error value
        if (newTemp == TEMP_ERROR_VALUE) {
            if (attempt < SENSOR_MAX_RETRIES - 1) {
                delay(100);  // Brief delay before retry
                sensor.requestTemperatures();
                continue;
            } else {
                failureCount++;
                healthy = false;
                connected = false;
                Serial.printf("[MattressSensor] CRITICAL: Failed to read DS18B20 after %d attempts\n", 
                             SENSOR_MAX_RETRIES);
                return false;
            }
        }
        
        // Validate reading range
        if (isValidReading(newTemp)) {
            temperature = newTemp;
            failureCount = 0;
            healthy = true;
            
            #if DEBUG_ENABLED
            Serial.printf("[MattressSensor] Mattress Temp: %.2f°C\n", temperature);
            #endif
            
            return true;
        } else {
            Serial.printf("[MattressSensor] ERROR: Invalid reading - Temp: %.2f°C\n", newTemp);
            failureCount++;
            healthy = false;
            return false;
        }
    }
    
    return false;
}

bool MattressSensor::isValidReading(float temp) {
    return (temp >= TEMP_MIN_VALID && temp <= TEMP_MAX_VALID);
}

