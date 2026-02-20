# Phase 2: Device Panel Interface

## Why
Users need direct physical interaction with the RODI Smart Mattress device to control temperature, timer, and heating without requiring connection to mobile app or web panel. The device panel provides immediate access to core functions through buttons and visual feedback through OLED display.

## What Changes
- Implement physical button interface with debouncing for temperature, timer, and power control
- Create OLED display driver with animated UI showing temperature, humidity, timer, and status icons
- Add boot sequence with animated branding (Rodiset.ir, RODI logo)
- Implement display animations for setting changes (zoom effect)
- Add button press patterns (short press, long press, hold for acceleration)
- Display WiFi and heating element status icons
- Create button-triggered SoftAP toggle (power button 3-second hold)

## Impact
- **Affected specs**: 
  - `button-interface` (new)
  - `display-interface` (new)
  - `boot-sequence` (new)
- **Affected code**: 
  - `src/buttons/` - Button debouncing and event handling
  - `src/display/` - OLED driver and UI rendering
  - `src/ui/` - Animation engine and layout management
  - `src/main.cpp` - Boot sequence integration
- **Hardware dependencies**: 
  - SSD1306 OLED display (I2C)
  - 4 physical buttons (temperature up/down, timer up/down, power)
  - I2C bus configuration (SDA, SCL pins)
- **Phase 1 integration**:
  - Buttons control Phase 1 heating controller
  - Display shows Phase 1 sensor readings and timer state

## Non-Goals (for this phase)
- Network connectivity (deferred to Phase 3)
- Remote control (deferred to Phase 3 & 4)
- Configuration menus (basic operation only)
- Multi-language support (English only for Phase 2)

## Prerequisites
- Phase 1 (Core Hardware Control) completed and tested
- OLED display connected and responding on I2C bus
- Buttons wired to GPIO pins with pull-up/down configuration
- Button hardware debouncing (optional, handled in software)

## Dependencies on Phase 1
- Temperature setpoint control APIs
- Timer duration control APIs
- Heating system enable/disable APIs
- Sensor reading access (DHT11, DS18B20)
- Timer remaining time access

