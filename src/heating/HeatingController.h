#ifndef HEATING_CONTROLLER_H
#define HEATING_CONTROLLER_H

#include <Arduino.h>
#include "config/RodiConfig.h"

/**
 * @brief Heating controller class with hysteresis-based temperature control
 * 
 * Controls heating element relay based on mattress temperature with
 * hysteresis to prevent rapid on/off cycling. Implements safety
 * features for sensor failure and emergency shutoff.
 */
class HeatingController {
public:
    /**
     * @brief Initialize the heating controller
     */
    void begin();
    
    /**
     * @brief Update heating control based on current temperature
     * @param currentTemp Current mattress temperature
     * @param setpoint Target temperature setpoint
     * @param sensorHealthy Sensor health status
     */
    void update(float currentTemp, float setpoint, bool sensorHealthy);
    
    /**
     * @brief Enable heating control
     */
    void enable();
    
    /**
     * @brief Disable heating control (emergency shutoff)
     */
    void disable();
    
    /**
     * @brief Force heating element OFF immediately
     */
    void forceOff();
    
    /**
     * @brief Check if heating is currently enabled
     */
    bool isEnabled() const { return enabled; }
    
    /**
     * @brief Check if heating element is currently ON
     */
    bool isHeating() const { return heatingState; }
    
    /**
     * @brief Get current setpoint
     */
    float getSetpoint() const { return setpoint; }
    
    /**
     * @brief Set temperature setpoint
     */
    void setSetpoint(float temp);
    
    /**
     * @brief Get hysteresis value
     */
    float getHysteresis() const { return hysteresis; }

private:
    float setpoint = TEMP_DEFAULT_C;
    float hysteresis = TEMP_HYSTERESIS_C;
    bool enabled = false;
    bool heatingState = false;  // Current relay state
    
    /**
     * @brief Control relay GPIO
     */
    void setRelay(bool state);
    
    /**
     * @brief Validate temperature setpoint
     */
    bool isValidSetpoint(float temp);
};

#endif // HEATING_CONTROLLER_H

