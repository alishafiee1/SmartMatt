#ifndef MATTRESS_SENSOR_H
#define MATTRESS_SENSOR_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "config/RodiConfig.h"

/**
 * @brief Mattress sensor class for DS18B20 temperature monitoring
 * 
 * Critical sensor for heating control. Failures trigger immediate
 * safety shutoff of heating element.
 */
class MattressSensor {
public:
    /**
     * @brief Initialize the mattress sensor
     */
    void begin();
    
    /**
     * @brief Read current mattress temperature
     * @return true if read successful, false on critical error
     */
    bool read();
    
    /**
     * @brief Get last valid temperature reading
     * @return Temperature in Celsius
     */
    float getTemperature() const { return temperature; }
    
    /**
     * @brief Check if sensor is healthy and connected
     * @return true if sensor is operational
     */
    bool isHealthy() const { return healthy; }
    
    /**
     * @brief Get number of consecutive read failures
     * @return Failure count
     */
    uint8_t getFailureCount() const { return failureCount; }
    
    /**
     * @brief Check if sensor is connected
     * @return true if DS18B20 device is detected
     */
    bool isConnected() const { return connected; }

private:
    OneWire oneWire{DS18B20_PIN};
    DallasTemperature sensor{&oneWire};
    
    float temperature = TEMP_DEFAULT_C;  // Last valid temperature
    bool healthy = false;                // Sensor health status
    bool connected = false;              // Physical connection status
    uint8_t failureCount = 0;            // Consecutive failure count
    
    static constexpr float TEMP_MIN_VALID = -10.0f;  // DS18B20 reasonable minimum
    static constexpr float TEMP_MAX_VALID = 85.0f;   // DS18B20 maximum safe reading
    static constexpr float TEMP_ERROR_VALUE = -127.0f; // DS18B20 error indicator
    
    /**
     * @brief Validate sensor reading
     */
    bool isValidReading(float temp);
};

#endif // MATTRESS_SENSOR_H

