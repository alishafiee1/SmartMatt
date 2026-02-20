# RODI Smart Mattress - Phase 1 Implementation

## Overview
Phase 1 implements the core hardware control system for the RODI Smart Mattress, providing foundational temperature monitoring, heating control, timer management, and persistent settings storage.

## Architecture

### Feature-Based Structure
```
src/
├── main.cpp                    # Main application integration
├── config/
│   └── RodiConfig.h           # Pin assignments and constants
├── sensors/
│   ├── RoomSensor.h/cpp       # DHT11 room temperature & humidity
│   └── MattressSensor.h/cpp   # DS18B20 mattress temperature
├── heating/
│   └── HeatingController.h/cpp # Heating element control with hysteresis
├── timer/
│   └── HeatingTimer.h/cpp     # Countdown timer for auto-shutoff
├── storage/
│   └── SettingsStorage.h/cpp  # NVS persistent settings
└── safety/
    └── SafetyMonitor.h/cpp    # Watchdog timer and health monitoring
```

## Hardware Configuration

### Pin Assignments
- **GPIO 4**: DHT11 (room temperature & humidity)
- **GPIO 5**: DS18B20 (mattress temperature)
- **GPIO 2**: Heating relay control
- **GPIO 21/22**: I2C (reserved for Phase 2 OLED)

### Components Required
- ESP32 DevKit
- DHT11 sensor (with pull-up resistor if needed)
- DS18B20 sensor (with 4.7kΩ pull-up resistor)
- Relay module (10A+ rating for heating element)
- Adequate power supply

## Features Implemented

### ✅ Temperature Monitoring
- **Room Sensor (DHT11)**: Temperature and humidity monitoring
- **Mattress Sensor (DS18B20)**: Critical temperature sensor
- Automatic retry on read failure (up to 3 attempts)
- Range validation
- Sensor health monitoring

### ✅ Heating Control
- **Hysteresis Algorithm**: 2°C hysteresis prevents rapid cycling
- **Safe Boot State**: Heating always OFF on startup
- **Temperature Range**: 20-45°C (configurable limits)
- **Emergency Shutoff**: Automatic disable on sensor failure

### ✅ Timer Management
- **Timer Range**: 15 minutes to 8 hours
- **Countdown Timer**: Accurate timing using millis()
- **Auto-shutoff**: Automatic heating disable on timer expiration
- **Real-time Updates**: Remaining time calculated on demand

### ✅ Settings Storage
- **Persistent Storage**: Uses ESP32 Preferences (NVS)
- **Stored Settings**:
  - Temperature setpoint
  - Timer duration
  - Heating enabled state
- **Default Values**: 25°C, heating OFF
- **Range Validation**: Ensures safe values on load

### ✅ Safety Features
- **Watchdog Timer**: 10-second timeout with auto-reset
- **Sensor Validation**: Continuous health monitoring
- **Critical Sensor Check**: Mattress sensor failure = immediate shutoff
- **Reset Reason Logging**: Diagnostics for debugging

## Configuration

### Key Constants (RodiConfig.h)
```cpp
// Temperature
#define TEMP_MIN_C        20.0f
#define TEMP_MAX_C        45.0f
#define TEMP_DEFAULT_C    25.0f
#define TEMP_HYSTERESIS_C 2.0f

// Timer
#define TIMER_MIN_MIN     15
#define TIMER_MAX_MIN     (8*60)  // 8 hours

// Safety
#define WATCHDOG_TIMEOUT_SEC 10
#define SENSOR_READ_INTERVAL_MS 2000
```

## Building and Uploading

### Prerequisites
1. Install PlatformIO IDE or PlatformIO Core
2. Install required libraries (automatically handled by platformio.ini)
3. Connect ESP32 via USB

### Build Commands
```bash
# Build project
pio run

# Upload to ESP32
pio run --target upload

# Monitor serial output
pio device monitor
```

### Serial Monitor
- **Baud Rate**: 115200
- **Debug Output**: Enabled (detailed logging)

## Usage

### Serial Monitor Output
The system provides comprehensive status updates:
- Initialization sequence with module status
- Sensor readings every 2 seconds
- System status summary every 10 seconds
- Heating state changes (ON/OFF with temperature)
- Timer expiration notifications
- Safety alerts and warnings

### Example Output
```
=====================================
     RODI Smart Mattress System     
    Phase 1: Core Hardware Control   
=====================================

[SafetyMonitor] Last reset reason: Power-on reset
[RoomSensor] DHT11 initialized successfully
[MattressSensor] DS18B20 initialized successfully (1 device(s))
[HeatingController] Initialized - Heating OFF (safe state)
[HeatingTimer] Timer initialized

╔════════════════════════════════════════════════════════════╗
║                    SYSTEM STATUS                           ║
╠════════════════════════════════════════════════════════════╣
║ Uptime: 10 seconds                                         ║
║ Safety: ✓ SAFE                                            ║
╠════════════════════════════════════════════════════════════╣
║ Room Temp: 23.5°C | Humidity: 45.2% | Status: OK          ║
║ Mattress Temp: 22.35°C | Status: OK                        ║
╠════════════════════════════════════════════════════════════╣
║ Heating: DISABLED | Setpoint: 25.0°C | Element: OFF       ║
║ Timer: STOPPED | Duration: 15 minutes                      ║
╚════════════════════════════════════════════════════════════╝
```

## Testing

### Manual Testing Checklist
- [ ] Verify sensor readings (compare to reference thermometer)
- [ ] Test heating control (observe relay state changes)
- [ ] Test timer countdown accuracy
- [ ] Test settings persistence (reboot device)
- [ ] Test sensor disconnection (remove DS18B20, verify shutoff)
- [ ] Test watchdog (introduce infinite loop, verify reset)
- [ ] Validate temperature range limits
- [ ] Validate timer range limits

### Safety Tests
- [ ] Disconnect DS18B20 during heating → Immediate shutoff
- [ ] Watchdog timeout → System reset and safe state
- [ ] Power cycle → Settings restored, heating OFF
- [ ] Invalid settings in NVS → Defaults restored

## Hysteresis Control Algorithm

The heating controller uses a hysteresis algorithm to prevent rapid on/off cycling:

```
Current State: OFF
├─ IF mattress_temp < (setpoint - hysteresis)
│  └─ Turn heating ON
└─ ELSE
   └─ Stay OFF

Current State: ON
├─ IF mattress_temp >= setpoint
│  └─ Turn heating OFF
└─ ELSE
   └─ Stay ON
```

**Example** (Setpoint: 25°C, Hysteresis: 2°C):
- Heating turns ON at: 23°C (25 - 2)
- Heating turns OFF at: 25°C

This creates a comfortable temperature range while minimizing relay wear.

## Known Limitations (Phase 1)

- No user interface (buttons/display) - Phase 2
- No network connectivity - Phase 3
- No mobile app - Phase 4
- Manual control requires code modification
- No persistent timer state across reboots

## Safety Considerations

### Critical Safety Features
1. **Safe Boot State**: Heating always OFF on startup
2. **Sensor Health**: Continuous monitoring of DS18B20
3. **Emergency Shutoff**: Immediate disable on sensor failure
4. **Watchdog Timer**: Automatic reset on system hang
5. **Temperature Limits**: Hardware-enforced safe range (20-45°C)

### Important Notes
- **Never** bypass sensor health checks
- **Never** modify temperature limits without safety review
- **Always** test heating control with actual hardware before prolonged use
- **Monitor** serial output for warning messages

## Next Steps (Phase 2)

Phase 2 will add:
- Physical button interface (5 buttons)
- OLED display (128x64 SSD1306)
- Boot sequence animation
- Real-time visual feedback
- Manual control via device panel

## Support

**Project**: RODI Smart Mattress  
**Team**: RODI  
**Developer**: AliShafiee  
**HW Designer**: H.Rostamizade  
**Date**: 2025-12-19

For OpenSpec documentation, see: `openspec/changes/phase1-core-hardware/`

