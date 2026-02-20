#ifndef RODI_CONFIG_H
#define RODI_CONFIG_H

#include <Arduino.h>

// ============================
// Pin Assignments
// ============================

// Temperature Sensors
#define DHT11_PIN         32     // GPIO32 - DHT11 room temp/humidity sensor
#define DS18B20_PIN       4      // GPIO4 - DS18B20 mattress temperature sensor

// Heating Control
#define HEATING_RELAY_PIN 2      // GPIO2 - Heating element relay control

// I2C for OLED Display (Phase 2)
#define I2C_SDA_PIN       21     // GPIO21 - I2C SDA
#define I2C_SCL_PIN       22     // GPIO22 - I2C SCL
#define I2C_FREQ_HZ       400000 // I2C frequency 400kHz (fast mode)

// Physical Buttons (Phase 2)
#define BTN_TEMP_UP_PIN   14     // GPIO14 - Temperature increase button
#define BTN_TEMP_DOWN_PIN 27     // GPIO27 - Temperature decrease button
#define BTN_TIMER_UP_PIN  13     // GPIO13 - Timer increase button
#define BTN_TIMER_DOWN_PIN 12    // GPIO12 - Timer decrease button
#define BTN_POWER_PIN     26    // GPIO26 - Power/heating toggle button

// ============================
// Temperature Limits & Defaults
// ============================

#define TEMP_MIN_C        10.0f  // Minimum safe temperature (°C)
#define TEMP_MAX_C        60.0f  // Maximum safe temperature (°C)
#define TEMP_DEFAULT_C    25.0f  // Default temperature setpoint (°C)
#define TEMP_HYSTERESIS_C 2.0f   // Hysteresis for heating control (°C)

// ============================
// Timer Limits
// ============================

#define TIMER_MIN_MIN     10      // Minimum timer duration (minutes)
#define TIMER_MAX_MIN     (8*60)  // Maximum timer duration (8 hours in minutes)

// ============================
// Sensor Configuration
// ============================

#define SENSOR_READ_INTERVAL_MS  2000   // Read sensors every 2 seconds
#define SENSOR_MAX_RETRIES       3      // Maximum retry attempts for sensor reads
#define SENSOR_TIMEOUT_MS        1000   // Timeout for sensor operations (ms)
#define DHT_TYPE               DHT11  // DHT sensor type

// ============================
// Safety Configuration
// ============================

#define WATCHDOG_TIMEOUT_SEC     20     // Watchdog timeout in seconds (increased for SPIFFS init)
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
// Button Configuration 
// ============================

#define BTN_DEBOUNCE_MS          20      // Button debounce time (ms) - reduced for faster response
#define BTN_LONG_PRESS_MS        3000    // Long press threshold (3 seconds)
#define BTN_REPEAT_START_MS      500     // Initial repeat delay when held (ms)
#define BTN_REPEAT_FAST_MS       100     // Accelerated repeat rate (ms)
#define BTN_REPEAT_ACCEL_TIME_MS 3000    // Time to reach fast repeat (ms)
#define BTN_TEMP_INCREMENT       1       // Temperature change per press (°C)
#define BTN_TIMER_INCREMENT      5       // Timer change per press (minutes)

// ============================
// Display Configuration 
// ============================

#define DISPLAY_WIDTH            128     // OLED display width in pixels
#define DISPLAY_HEIGHT           64      // OLED display height in pixels
#define SH1106_I2C_ADDR          0x78    // SH1106 I2C address (0x78 default, 0x7A with jumper)
#define DISPLAY_REFRESH_MS       200     // Periodic display refresh (5 Hz for smoother updates)
#define ICON_SIZE                12      // Status icon size (pixels)

// Animation Configuration
#define ANIM_ZOOM_DURATION_MS    300     // Zoom animation duration (ms)
#define ANIM_IDLE_TIMEOUT_MS     3000    // Idle time before zoom out (ms)
#define ANIM_TARGET_FPS          30      // Target animation frame rate
#define ANIM_ZOOM_SCALE          1.5f    // Zoom scale factor (150%)

// Boot Sequence Configuration
#define BOOT_STAGE_DURATION_MS  3000    // Rodiset.ir stage duration (ms)

// ============================
// WiFi Configuration 
// ============================

// SoftAP Configuration
#define WIFI_AP_SSID_PREFIX      "RODI_"           // SoftAP SSID prefix (suffix = MAC last 4 digits)
#define WIFI_AP_PASSWORD         ""                 // SoftAP password (empty = open network, min 8 chars for WPA2)
#define WIFI_AP_CHANNEL          1                 // WiFi channel (1-13)
#define WIFI_AP_MAX_CONNECTIONS  4                 // Maximum concurrent connections
#define WIFI_AP_HIDDEN           false             // Broadcast SSID
#define WIFI_AP_IP               "192.168.4.1"     // SoftAP IP address
#define WIFI_AP_GATEWAY          "192.168.4.1"     // SoftAP gateway
#define WIFI_AP_SUBNET           "255.255.255.0"   // SoftAP subnet mask

// Station Mode Configuration
#define WIFI_STA_CONNECT_TIMEOUT_MS  30000         // Station connection timeout (30 seconds)
#define WIFI_STA_MAX_RETRIES         3             // Maximum connection retry attempts
#define WIFI_STA_RETRY_DELAY_MS      5000          // Delay between retries (5 seconds)

// WiFi Settings Storage Keys
#define SETTINGS_KEY_WIFI_SSID       "wifi_ssid"   // WiFi SSID key in NVS
#define SETTINGS_KEY_WIFI_PASS       "wifi_pass"   // WiFi password key in NVS
#define SETTINGS_KEY_SOFTAP_ENABLED  "softap_en"   // SoftAP enabled state key

// ============================
// Web Server Configuration (Phase 3)
// ============================

#define HTTP_SERVER_PORT         80                // HTTP server port
#define HTTP_MAX_URI_HANDLERS    20                // Maximum URI handlers
#define HTTP_STACK_SIZE          8192              // HTTP server stack size
#define WEBSOCKET_MAX_CLIENTS    4                 // Maximum WebSocket connections
#define WEBSOCKET_PING_INTERVAL_MS  30000          // WebSocket ping interval (30 seconds)

// ============================
// System Configuration
// ============================

#define MAIN_LOOP_DELAY_MS       100     // Main loop delay to prevent busy-wait

#endif // RODI_CONFIG_H

