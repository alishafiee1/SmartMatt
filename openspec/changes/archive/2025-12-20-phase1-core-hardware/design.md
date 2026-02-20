# Phase 1: Core Hardware Control - Design Document

## Context
This phase establishes the foundational hardware control system for the RODI Smart Mattress. The system must safely manage a heating element based on temperature feedback while ensuring user safety through multiple fail-safe mechanisms. The design uses ESP-IDF on ESP32 hardware with a feature-based architecture.

### Stakeholders
- End users requiring safe, reliable heating control
- Hardware team (H.Rostamizade) providing sensor integration requirements
- Software team (AliShafiee) implementing the control logic

### Constraints
- Must operate without user interface (Phase 2 dependency)
- Temperature control within 20-45°C safety range
- Timer limits: 15 minutes to 8 hours
- Memory-efficient storage using Preferences library
- Real-time temperature monitoring requirements

## Goals / Non-Goals

### Goals
- Safe heating element control with automatic shutoff mechanisms
- Accurate temperature monitoring from two independent sensors
- Persistent settings storage across power cycles
- Robust error handling for sensor failures
- Timer-based automatic heating termination
- System reliability through watchdog timer

### Non-Goals
- User interface (deferred to Phase 2)
- Network connectivity (deferred to Phase 3)
- Remote control features (deferred to Phase 4)
- Advanced scheduling or automation
- Energy usage tracking

## Technical Decisions

### 1. Architecture: Feature-Based Modules
**Decision**: Organize code into independent feature modules (sensors, heating, timer, storage, safety)

**Rationale**:
- Clear separation of concerns
- Easier testing and maintenance
- Aligns with project conventions (see project.md)
- Supports incremental development

**Alternatives considered**:
- Monolithic main.cpp: Rejected due to poor maintainability
- Object-oriented class hierarchy: Rejected as over-engineered for embedded system

### 2. Temperature Sensor Strategy
**Decision**: Use two sensors (DHT11 for room, DS18B20 for mattress) with independent validation

**Rationale**:
- DHT11 provides both temperature and humidity for room monitoring
- DS18B20 provides accurate mattress internal temperature
- Independent sensors allow cross-validation
- DS18B20 failure triggers immediate safety shutoff

**Alternatives considered**:
- Single sensor: Rejected due to inability to monitor both room and mattress
- Redundant DS18B20 sensors: Deferred as over-engineered for Phase 1

### 3. Hysteresis Control Algorithm
**Decision**: Implement 2°C hysteresis (configurable) to prevent rapid on/off cycling

**Algorithm**:
```
IF heating is OFF:
    Turn ON when mattress_temp < (setpoint - hysteresis)
ELSE IF heating is ON:
    Turn OFF when mattress_temp > setpoint
```

**Rationale**:
- Reduces relay wear and electrical noise
- Provides stable temperature control
- Industry-standard approach for thermal systems

**Trade-offs**:
- Slight temperature variation around setpoint
- Benefits outweigh precision loss for mattress heating application

### 4. Timer Implementation
**Decision**: Use ESP32 hardware timer with software countdown logic

**Rationale**:
- Hardware timer provides accurate timing
- Low overhead (no need for RTC precision)
- Countdown calculated in main loop for simplicity
- Timer state persisted to handle unexpected reboots

**Alternatives considered**:
- FreeRTOS software timers: Rejected as unnecessary complexity
- RTC-based timing: Rejected due to overkill for countdown needs

### 5. Settings Storage Strategy
**Decision**: Use ESP32 Preferences library (NVS wrapper) with key-value pairs

**Storage schema**:
```
Namespace: "rodi_settings"
Keys:
  - temp_setpoint (float, 20-45°C)
  - timer_duration (uint32, minutes)
  - heating_enabled (bool)
```

**Rationale**:
- Simple API for embedded key-value storage
- Automatic wear leveling on ESP32 flash
- Minimal memory footprint
- Fast read/write operations

**Alternatives considered**:
- SPIFFS file system: Rejected as overkill for small settings
- EEPROM emulation: Rejected due to Preferences library superiority

### 6. Safety Architecture
**Decision**: Multi-layer safety approach with watchdog + sensor validation

**Safety layers**:
1. **Watchdog Timer**: 10-second timeout, auto-reset on hang
2. **Sensor Validation**: Continuous DS18B20 health check
3. **Emergency Shutoff**: Immediate heating disable on sensor failure
4. **Safe Boot State**: Heating always OFF after boot/reset
5. **Range Validation**: Reject out-of-range temperature/timer values

**Rationale**:
- Heating element poses burn/fire risk if uncontrolled
- Multiple independent safety mechanisms provide defense in depth
- Fail-safe design: any error → heating OFF

## Data Flow

```
┌─────────────┐
│   DHT11     │──→ Room Temp/Humidity ──┐
└─────────────┘                          │
                                         ↓
┌─────────────┐                    ┌──────────────┐
│  DS18B20    │──→ Mattress Temp ─→│   Heating    │──→ Relay Control
└─────────────┘    (validated)     │  Controller  │    (GPIO)
                                   └──────────────┘
                                         ↑
                                         │
                                   ┌──────────────┐
                                   │    Timer     │
                                   │  Management  │
                                   └──────────────┘
                                         ↑
                                         │
                                   ┌──────────────┐
                                   │   Settings   │
                                   │   Storage    │
                                   └──────────────┘
```

## Pin Assignments

```c
// Temperature Sensors
#define DHT11_PIN         GPIO_NUM_4
#define DS18B20_PIN       GPIO_NUM_5

// Heating Control
#define HEATING_RELAY_PIN GPIO_NUM_2

// I2C for OLED (Phase 2)
#define I2C_SDA_PIN       GPIO_NUM_21
#define I2C_SCL_PIN       GPIO_NUM_22
```

## Constants and Thresholds

```c
// Temperature Limits
#define TEMP_MIN_C        20.0f
#define TEMP_MAX_C        45.0f
#define TEMP_DEFAULT_C    25.0f
#define TEMP_HYSTERESIS_C 2.0f

// Timer Limits
#define TIMER_MIN_MIN     15
#define TIMER_MAX_MIN     (8 * 60)  // 8 hours

// Sensor Reading
#define SENSOR_READ_INTERVAL_MS  2000
#define SENSOR_MAX_RETRIES       3
#define SENSOR_TIMEOUT_MS        1000

// Watchdog
#define WATCHDOG_TIMEOUT_SEC     10
```

## Error Handling Strategy

### Sensor Errors
- **DS18B20 read failure**: 
  - Retry up to 3 times
  - If persistent: Disable heating, set error flag
  - Log error to serial
- **DHT11 read failure**: 
  - Retry up to 3 times
  - Non-critical: Continue operation, use last valid reading
  - Display error indicator (Phase 2)

### Timer Errors
- **Timer expiration**: Normal flow, disable heating
- **Timer overflow**: Not possible with uint32 + range validation
- **Settings corruption**: Restore to defaults

### Storage Errors
- **NVS read failure**: Use default values, attempt repair
- **NVS write failure**: Log error, continue with in-memory values
- **Corrupted data**: Validate ranges, restore defaults if invalid

## Performance Considerations

### CPU Usage
- Sensor polling: ~2% CPU @ 2-second intervals
- Heating control: <1% CPU (state machine)
- Timer management: <1% CPU
- Total: <5% CPU, leaving headroom for Phase 2+

### Memory Footprint
- Sensor drivers: ~8 KB
- Heating control: ~4 KB
- Timer: ~2 KB
- Settings: ~1 KB
- Total: ~15 KB RAM, ~50 KB flash

### Response Times
- Sensor reading: <100ms
- Heating control decision: <10ms
- Settings save: <50ms
- Watchdog refresh: <1ms

## Testing Strategy

### Unit Testing (Manual)
Each module tested independently via serial commands:
- Sensor reading accuracy (compare to reference thermometer)
- Hysteresis algorithm (log state transitions)
- Timer countdown (verify accuracy over 30 minutes)
- Settings persistence (reboot test)

### Integration Testing
Complete system tests:
1. Cold start: Verify safe boot state
2. Heating cycle: Monitor temperature rise to setpoint
3. Timer expiration: Verify automatic shutoff
4. Sensor failure simulation: Verify emergency shutoff
5. Power cycle: Verify settings persistence

### Safety Testing
Critical safety scenarios:
1. Disconnect DS18B20 during heating → Immediate shutoff
2. Watchdog timeout simulation → System reset
3. Out-of-range temperature requests → Rejection
4. Multiple rapid on/off commands → Stable operation

## Migration Plan

### Phase 1 → Phase 2 Integration
- No breaking changes expected
- Phase 2 adds UI layer on top of Phase 1 APIs
- Settings structure may expand (WiFi credentials)
- Heating control API remains stable

### Rollback Strategy
- Phase 1 has no previous version to roll back to
- Future phases can disable Phase 1 features but not remove them
- Settings namespace versioning for future compatibility

## Risks / Trade-offs

### Risk: Temperature Sensor Accuracy
- **Impact**: Inaccurate heating control
- **Mitigation**: Use calibrated sensors, validate against reference
- **Trade-off**: Accepting ±0.5°C sensor tolerance

### Risk: Flash Wear from Frequent Settings Writes
- **Impact**: NVS failure after ~100K write cycles
- **Mitigation**: Only write on actual setting changes, not periodic
- **Trade-off**: Settings may be lost if power fails during write

### Risk: Watchdog False Triggers
- **Impact**: Unexpected system resets
- **Mitigation**: 10-second timeout provides ample margin
- **Trade-off**: May not catch some soft-lock conditions

### Risk: Relay Failure
- **Impact**: Heating element stuck on or off
- **Mitigation**: Use quality relay, future phase adds current sensing
- **Trade-off**: Hardware reliability depends on component quality

## Open Questions

1. **Q**: Should we add overcurrent protection in Phase 1?
   **A**: Deferred - requires additional hardware (current sensor)

2. **Q**: Do we need temperature trend analysis (rising/falling rate)?
   **A**: Deferred to Phase 3 - useful for web/app analytics

3. **Q**: Should settings include last heating timestamp?
   **A**: No - timer resets on power cycle, timestamp not needed

4. **Q**: What happens if both sensors fail simultaneously?
   **A**: Heating disabled, error state, requires manual reset

## Dependencies

### Hardware Dependencies
- ESP32 DevKit with sufficient GPIO pins
- DHT11 sensor properly powered (3.3V or 5V depending on module)
- DS18B20 with 4.7kΩ pull-up resistor on data line
- Relay module for heating element control (isolated, 10A+ rating)
- Adequate power supply for ESP32 + peripherals

### Software Dependencies
- ESP-IDF v4.4+ (LTS version recommended)
- DHT sensor library (Adafruit or equivalent)
- OneWire + DallasTemperature libraries for DS18B20
- Preferences library (included in ESP-IDF)

### External Dependencies
- Serial console for debugging (USB connection)
- Development machine with ESP-IDF toolchain

