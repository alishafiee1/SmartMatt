#include "RoomSensor.h"

void RoomSensor::begin() {
    dht.begin();
    delay(2000);  // DHT11 requires 2 second stabilization
    
    // Initial read to validate sensor presence
    if (read()) {
        Serial.println("[RoomSensor] DHT11 initialized successfully");
    } else {
        Serial.println("[RoomSensor] WARNING: DHT11 initialization failed - will retry");
    }
}

bool RoomSensor::read() {
    // Attempt to read with retries
    for (uint8_t attempt = 0; attempt < SENSOR_MAX_RETRIES; attempt++) {
        float newTemp = dht.readTemperature();
        float newHum = dht.readHumidity();
        
        // Check for read errors (NaN indicates sensor failure)
        if (isnan(newTemp) || isnan(newHum)) {
            if (attempt < SENSOR_MAX_RETRIES - 1) {
                delay(100);  // Brief delay before retry
                continue;
            } else {
                failureCount++;
                healthy = false;
                Serial.printf("[RoomSensor] ERROR: Failed to read DHT11 after %d attempts\n", 
                             SENSOR_MAX_RETRIES);
                return false;
            }
        }
        
        // Validate reading range
        if (isValidReading(newTemp, newHum)) {
            temperature = newTemp;
            humidity = newHum;
            failureCount = 0;
            healthy = true;
            
            #if DEBUG_ENABLED
            Serial.printf("[RoomSensor] Temp: %.1f°C, Humidity: %.1f%%\n", 
                         temperature, humidity);
            #endif
            
            return true;
        } else {
            Serial.printf("[RoomSensor] ERROR: Invalid reading - Temp: %.1f°C, Hum: %.1f%%\n",
                         newTemp, newHum);
            failureCount++;
            healthy = false;
            return false;
        }
    }
    
    return false;
}

bool RoomSensor::isValidReading(float temp, float hum) {
    return (temp >= TEMP_MIN_VALID && temp <= TEMP_MAX_VALID &&
            hum >= HUMIDITY_MIN_VALID && hum <= HUMIDITY_MAX_VALID);
}

