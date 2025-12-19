#ifndef RODI_CONFIG_H
#define RODI_CONFIG_H

#include <Arduino.h>

// ============================
// Pin Assignments
// ============================

// Temperature Sensors
#define DHT22_PIN         4      // GPIO4 - DHT22 room temp/humidity sensor
#define DS18B20_PIN       5      // GPIO5 - DS18B20 mattress temperature sensor

// Heating Control
#define HEATING_RELAY_PIN 2      // GPIO2 - Heating element relay control

// I2C for OLED (Phase 2 - reserved)
#define I2C_SDA_PIN       21     // GPIO21 - I2C SDA
#define I2C_SCL_PIN       22     // GPIO22 - I2C SCL

// ============================
// Temperature Limits & Defaults
// ============================

#define TEMP_MIN_C        20.0f  // Minimum safe temperature (°C)
#define TEMP_MAX_C        45.0f  // Maximum safe temperature (°C)
#define TEMP_DEFAULT_C    25.0f  // Default temperature setpoint (°C)
#define TEMP_HYSTERESIS_C 2.0f   // Hysteresis for heating control (°C)

// ============================
// Timer Limits
// ============================

#define TIMER_MIN_MIN     15      // Minimum timer duration (minutes)
#define TIMER_MAX_MIN     (8*60)  // Maximum timer duration (8 hours in minutes)

// ============================
// Sensor Configuration
// ============================

#define SENSOR_READ_INTERVAL_MS  2000   // Read sensors every 2 seconds
#define SENSOR_MAX_RETRIES       3      // Maximum retry attempts for sensor reads
#define SENSOR_TIMEOUT_MS        1000   // Timeout for sensor operations (ms)
#define DHT22_TYPE               DHT22  // DHT sensor type

// ============================
// Safety Configuration
// ============================

#define WATCHDOG_TIMEOUT_SEC     10     // Watchdog timeout in seconds
#define SAFE_BOOT_HEATING_STATE  false  // Heating OFF on boot

// ============================
// Settings Storage
// ============================

#define SETTINGS_NAMESPACE       "rodi_settings"  // NVS namespace for settings
#define SETTINGS_KEY_TEMP        "temp_setpoint"  // Temperature setpoint key
#define SETTINGS_KEY_TIMER       "timer_duration" // Timer duration key
#define SETTINGS_KEY_HEATING     "heating_enabled"// Heating state key

// ============================
// Serial Debug
// ============================

#define SERIAL_BAUD_RATE         115200  // Serial communication baud rate
#define DEBUG_ENABLED            true    // Enable debug logging

// ============================
// System Configuration
// ============================

#define MAIN_LOOP_DELAY_MS       100     // Main loop delay to prevent busy-wait

#endif // RODI_CONFIG_H

