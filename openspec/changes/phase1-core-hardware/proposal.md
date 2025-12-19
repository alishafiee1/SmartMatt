# Phase 1: Core Hardware Control System

## Why
The RODI Smart Mattress requires a foundational hardware control system to safely manage mattress heating through temperature monitoring, timing control, and persistent settings. This phase establishes the critical safety infrastructure and core control logic that all subsequent features will depend on.

## What Changes
- Implement temperature monitoring with DHT22 (room) and DS18B20 (mattress) sensors
- Create heating element control with hysteresis-based temperature management
- Build software timer system for automatic shutoff after user-defined duration
- Implement persistent settings storage using ESP32 Preferences library
- Add watchdog timer for system reliability
- Create safety mechanisms for sensor failure detection

## Impact
- **Affected specs**: 
  - `heating-control` (new)
  - `sensor-monitoring` (new)
  - `timer-management` (new)
  - `settings-storage` (new)
- **Affected code**: 
  - `src/sensors/` - Temperature sensor drivers
  - `src/heating/` - Heating element control logic
  - `src/timer/` - Timer management
  - `src/storage/` - Settings persistence
  - `src/safety/` - Safety watchdog and sensor validation
- **Hardware dependencies**: 
  - DHT22 sensor
  - DS18B20 sensor
  - Heating element relay/control circuit
  - ESP32 GPIO pins

## Non-Goals (for this phase)
- User interface components (buttons, display)
- Network connectivity (WiFi, web panel)
- Mobile application integration
- Remote monitoring capabilities

## Prerequisites
- ESP32 development board configured
- ESP-IDF framework installed and working
- Hardware components connected and tested
- Pin assignments defined in configuration

