#ifndef ROOM_SENSOR_H
#define ROOM_SENSOR_H

#include <Arduino.h>
#include <DHT.h>
#include "config/RodiConfig.h"

/**
 * @brief Room sensor class for DHT11 temperature and humidity monitoring
 * 
 * Provides room temperature and humidity readings with error handling
 * and validation. Non-critical failures allow continued operation.
 */
class RoomSensor {
public:
    /**
     * @brief Initialize the room sensor
     */
    void begin();
    
    /**
     * @brief Read current temperature and humidity
     * @return true if read successful, false on error
     */
    bool read();
    
    /**
     * @brief Get last valid temperature reading
     * @return Temperature in Celsius
     */
    float getTemperature() const { return temperature; }
    
    /**
     * @brief Get last valid humidity reading
     * @return Humidity percentage (0-100)
     */
    float getHumidity() const { return humidity; }
    
    /**
     * @brief Check if sensor is healthy
     * @return true if last read was successful
     */
    bool isHealthy() const { return healthy; }
    
    /**
     * @brief Get number of consecutive read failures
     * @return Failure count
     */
    uint8_t getFailureCount() const { return failureCount; }

private:
    DHT dht{DHT11_PIN, DHT_TYPE};
    
    float temperature = TEMP_DEFAULT_C;  // Last valid temperature
    float humidity = 50.0f;              // Last valid humidity
    bool healthy = false;                // Sensor health status
    uint8_t failureCount = 0;            // Consecutive failure count
    
    static constexpr float TEMP_MIN_VALID = -40.0f;  // DHT11 minimum
    static constexpr float TEMP_MAX_VALID = 80.0f;   // DHT11 maximum
    static constexpr float HUMIDITY_MIN_VALID = 0.0f;
    static constexpr float HUMIDITY_MAX_VALID = 100.0f;
    
    /**
     * @brief Validate sensor reading
     */
    bool isValidReading(float temp, float hum);
};

#endif // ROOM_SENSOR_H

