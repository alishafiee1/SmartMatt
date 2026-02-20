# Phase 1 API Reference

## Module Overview

Phase 1 provides 6 independent modules for hardware control:

| Module | Purpose | Header File |
|--------|---------|-------------|
| RoomSensor | DHT11 temperature & humidity | `sensors/RoomSensor.h` |
| MattressSensor | DS18B20 mattress temperature | `sensors/MattressSensor.h` |
| HeatingController | Heating element control | `heating/HeatingController.h` |
| HeatingTimer | Countdown timer | `timer/HeatingTimer.h` |
| SettingsStorage | NVS persistence | `storage/SettingsStorage.h` |
| SafetyMonitor | Watchdog & health checks | `safety/SafetyMonitor.h` |

---

## RoomSensor API

### Initialization
```cpp
RoomSensor roomSensor;
roomSensor.begin();
```

### Methods

#### `bool read()`
Read temperature and humidity from DHT11.
- **Returns**: `true` if successful, `false` on error
- **Retries**: Up to 3 attempts
- **Call frequency**: Every 2+ seconds (DHT11 limitation)

```cpp
if (roomSensor.read()) {
    Serial.println("Room sensor read OK");
}
```

#### `float getTemperature()`
Get last valid temperature reading.
- **Returns**: Temperature in Celsius
- **Range**: -40°C to 80°C (DHT11 spec)

```cpp
float temp = roomSensor.getTemperature();
Serial.printf("Room temp: %.1f°C\n", temp);
```

#### `float getHumidity()`
Get last valid humidity reading.
- **Returns**: Relative humidity (0-100%)

```cpp
float humidity = roomSensor.getHumidity();
Serial.printf("Humidity: %.1f%%\n", humidity);
```

#### `bool isHealthy()`
Check sensor health status.
- **Returns**: `true` if last read successful

```cpp
if (roomSensor.isHealthy()) {
    // Sensor OK
} else {
    // Sensor error - use last valid reading
}
```

---

## MattressSensor API

### Initialization
```cpp
MattressSensor mattressSensor;
mattressSensor.begin();
```

### Methods

#### `bool read()`
Read temperature from DS18B20 (critical sensor).
- **Returns**: `true` if successful, `false` on critical error
- **Retries**: Up to 3 attempts
- **Call frequency**: Any (async mode)

```cpp
bool ok = mattressSensor.read();
if (!ok) {
    // CRITICAL: Disable heating immediately
    heatingController.forceOff();
}
```

#### `float getTemperature()`
Get mattress temperature.
- **Returns**: Temperature in Celsius
- **Range**: -10°C to 85°C (validated range)

```cpp
float mattressTemp = mattressSensor.getTemperature();
```

#### `bool isHealthy()`
Check sensor health (critical for heating).
- **Returns**: `true` if sensor operational

```cpp
if (!mattressSensor.isHealthy()) {
    Serial.println("CRITICAL: Mattress sensor failed!");
}
```

#### `bool isConnected()`
Check physical sensor connection.
- **Returns**: `true` if DS18B20 detected on bus

---

## HeatingController API

### Initialization
```cpp
HeatingController heatingController;
heatingController.begin();  // Sets GPIO, heating OFF
```

### Methods

#### `void update(float currentTemp, float setpoint, bool sensorHealthy)`
Update heating control (call every sensor read).
- **currentTemp**: Mattress temperature
- **setpoint**: Target temperature
- **sensorHealthy**: Sensor health flag

**Hysteresis Logic**:
- Turn ON: `temp < (setpoint - hysteresis)`
- Turn OFF: `temp >= setpoint`

```cpp
heatingController.update(
    mattressSensor.getTemperature(),
    settings.getTemperatureSetpoint(),
    mattressSensor.isHealthy()
);
```

#### `void enable()` / `void disable()`
Enable or disable heating control.
```cpp
heatingController.enable();   // Allow heating
heatingController.disable();  // Prevent heating
```

#### `void forceOff()`
Emergency shutoff (immediate OFF).
```cpp
if (!mattressSensor.isHealthy()) {
    heatingController.forceOff();
}
```

#### `bool isEnabled()` / `bool isHeating()`
Check heating state.
```cpp
if (heatingController.isEnabled() && heatingController.isHeating()) {
    Serial.println("Heating element is ON");
}
```

#### `void setSetpoint(float temp)`
Change temperature setpoint.
- **Range**: 20-45°C (validated)

```cpp
heatingController.setSetpoint(30.0f);
```

---

## HeatingTimer API

### Initialization
```cpp
HeatingTimer heatingTimer;
heatingTimer.begin();
```

### Methods

#### `void start(uint32_t durationMinutes)`
Start countdown timer.
- **Range**: 15 minutes to 480 minutes (8 hours)

```cpp
heatingTimer.start(60);  // 60 minutes
heatingController.enable();
```

#### `void stop()` / `void reset()`
Stop or reset timer.
```cpp
heatingTimer.stop();   // Stop countdown
heatingTimer.reset();  // Reset to initial duration
```

#### `void update()`
Update timer state (call in main loop).
```cpp
void loop() {
    heatingTimer.update();
    
    if (heatingTimer.hasExpired()) {
        heatingController.disable();
        heatingTimer.clearExpired();
    }
}
```

#### `bool isRunning()` / `bool hasExpired()`
Check timer state.
```cpp
if (heatingTimer.isRunning()) {
    uint32_t remaining = heatingTimer.getRemainingMinutes();
    Serial.printf("Time left: %u min\n", remaining);
}
```

#### `uint32_t getRemainingSeconds()` / `getRemainingMinutes()`
Get remaining time.
- **Seconds**: Exact remaining time
- **Minutes**: Rounded up to nearest minute

```cpp
uint32_t sec = heatingTimer.getRemainingSeconds();
uint32_t min = heatingTimer.getRemainingMinutes();
Serial.printf("Remaining: %u:%02u\n", min, sec % 60);
```

---

## SettingsStorage API

### Initialization
```cpp
SettingsStorage settings;
settings.begin();  // Loads from NVS
```

### Methods

#### `void load()` / `bool save()`
Load or save settings.
```cpp
settings.load();  // Read from NVS

settings.setTemperatureSetpoint(28.0f);
settings.save();  // Write to NVS
```

#### `float getTemperatureSetpoint()` / `void setTemperatureSetpoint(float temp)`
Temperature setpoint management.
- **Range**: 20-45°C
- **Default**: 25°C

```cpp
float setpoint = settings.getTemperatureSetpoint();
settings.setTemperatureSetpoint(30.0f);
```

#### `uint32_t getTimerDuration()` / `void setTimerDuration(uint32_t minutes)`
Timer duration management.
- **Range**: 15-480 minutes
- **Default**: 15 minutes

```cpp
uint32_t duration = settings.getTimerDuration();
settings.setTimerDuration(120);  // 2 hours
```

#### `bool isHeatingEnabled()` / `void setHeatingEnabled(bool enabled)`
Heating state management.
- **Boot Default**: Always `false` (safe state)

```cpp
settings.setHeatingEnabled(true);
settings.save();
```

#### `void resetToDefaults()`
Reset all settings to defaults.
```cpp
settings.resetToDefaults();  // Auto-saves
```

---

## SafetyMonitor API

### Initialization
```cpp
SafetyMonitor safetyMonitor;
safetyMonitor.begin();  // Enables watchdog
```

### Methods

#### `void feedWatchdog()`
Reset watchdog timer (call in main loop).
- **Timeout**: 10 seconds
- **Consequence**: System reset if not fed

```cpp
void loop() {
    safetyMonitor.feedWatchdog();
    // ... rest of code ...
}
```

#### `void update(bool mattressSensorHealthy, bool roomSensorHealthy)`
Update safety status.
```cpp
safetyMonitor.update(
    mattressSensor.isHealthy(),
    roomSensor.isHealthy()
);
```

#### `bool isSafe()`
Check overall safety state.
- **Returns**: `true` if critical sensor healthy

```cpp
if (!safetyMonitor.isSafe()) {
    heatingController.forceOff();
}
```

#### `bool isCriticalSensorHealthy()`
Check mattress sensor specifically.
```cpp
if (!safetyMonitor.isCriticalSensorHealthy()) {
    Serial.println("CRITICAL SENSOR FAILURE");
}
```

#### `uint32_t getUptimeSeconds()`
Get system uptime.
```cpp
uint32_t uptime = safetyMonitor.getUptimeSeconds();
Serial.printf("Uptime: %u seconds\n", uptime);
```

---

## Complete Example

### Basic Heating Control
```cpp
#include <Arduino.h>
#include "config/RodiConfig.h"
#include "sensors/RoomSensor.h"
#include "sensors/MattressSensor.h"
#include "heating/HeatingController.h"
#include "timer/HeatingTimer.h"
#include "storage/SettingsStorage.h"
#include "safety/SafetyMonitor.h"

// Global objects
RoomSensor roomSensor;
MattressSensor mattressSensor;
HeatingController heatingController;
HeatingTimer heatingTimer;
SettingsStorage settings;
SafetyMonitor safetyMonitor;

void setup() {
    Serial.begin(115200);
    
    // Initialize all modules
    safetyMonitor.begin();
    settings.begin();
    roomSensor.begin();
    mattressSensor.begin();
    heatingController.begin();
    heatingTimer.begin();
    
    // Configure heating
    heatingController.setSetpoint(30.0f);
    heatingController.enable();
    
    // Start 2-hour timer
    heatingTimer.start(120);
    
    Serial.println("System ready");
}

void loop() {
    // Feed watchdog
    safetyMonitor.feedWatchdog();
    
    // Read sensors
    roomSensor.read();
    bool mattressOk = mattressSensor.read();
    
    // Update safety
    safetyMonitor.update(mattressSensor.isHealthy(), roomSensor.isHealthy());
    
    // Update heating control
    if (mattressOk) {
        heatingController.update(
            mattressSensor.getTemperature(),
            30.0f,
            mattressSensor.isHealthy()
        );
    } else {
        heatingController.forceOff();
    }
    
    // Update timer
    heatingTimer.update();
    if (heatingTimer.hasExpired()) {
        heatingController.disable();
        heatingTimer.clearExpired();
        Serial.println("Timer expired - heating OFF");
    }
    
    // Print status
    Serial.printf("Mattress: %.1f°C | Heating: %s | Timer: %u min\n",
                 mattressSensor.getTemperature(),
                 heatingController.isHeating() ? "ON" : "OFF",
                 heatingTimer.getRemainingMinutes());
    
    delay(2000);
}
```

---

## Error Handling

### Sensor Errors
```cpp
// Non-critical (room sensor)
if (!roomSensor.isHealthy()) {
    Serial.println("Warning: Room sensor error (using last valid reading)");
    // Continue operation
}

// Critical (mattress sensor)
if (!mattressSensor.isHealthy()) {
    Serial.println("CRITICAL: Mattress sensor failed!");
    heatingController.forceOff();
    // Do NOT enable heating until sensor recovers
}
```

### Watchdog Protection
```cpp
void loop() {
    // ALWAYS feed watchdog at start of loop
    safetyMonitor.feedWatchdog();
    
    // Avoid blocking operations >10 seconds
    // Break long tasks into chunks
}
```

### Safe Shutdown
```cpp
void emergencyShutdown() {
    heatingController.forceOff();
    heatingTimer.stop();
    settings.setHeatingEnabled(false);
    settings.save();
    
    Serial.println("EMERGENCY SHUTDOWN COMPLETE");
}
```

---

## Constants Reference

All constants defined in `config/RodiConfig.h`:

### Temperature
```cpp
TEMP_MIN_C        20.0f   // Minimum safe temperature
TEMP_MAX_C        45.0f   // Maximum safe temperature
TEMP_DEFAULT_C    25.0f   // Default setpoint
TEMP_HYSTERESIS_C 2.0f    // Hysteresis for control
```

### Timer
```cpp
TIMER_MIN_MIN     15      // Minimum timer (minutes)
TIMER_MAX_MIN     480     // Maximum timer (8 hours)
```

### Sensors
```cpp
SENSOR_READ_INTERVAL_MS  2000   // Read every 2 seconds
SENSOR_MAX_RETRIES       3      // Retry attempts
```

### Safety
```cpp
WATCHDOG_TIMEOUT_SEC     10     // Watchdog timeout
SAFE_BOOT_HEATING_STATE  false  // Heating OFF on boot
```

---

## Thread Safety

**Note**: Phase 1 uses single-threaded Arduino framework.

- All APIs are **NOT** thread-safe
- Do **NOT** call from ISRs
- Do **NOT** use FreeRTOS tasks without proper locking

Future phases may add multi-threading with appropriate mutexes.

---

## Performance Notes

### Timing Requirements
- Feed watchdog: Every <10 seconds
- Read sensors: Every 2-5 seconds
- Update heating: On every sensor read
- Update timer: Every loop iteration

### Blocking Operations
- DHT11 read: ~250ms
- DS18B20 read: ~750ms (12-bit resolution)
- Settings save: <50ms

**Recommendation**: Keep main loop frequency >1 Hz (avoid delays >1000ms)

---

## Next Phase (Phase 2)

Phase 2 will add:
- Button interface API
- Display interface API
- Boot sequence API
- User interaction handling

All Phase 1 APIs remain stable and unchanged.


