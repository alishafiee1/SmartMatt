# Phase 1 Implementation Tasks

## 1. Project Structure Setup
- [ ] 1.1 Create feature-based folder structure (sensors, heating, timer, storage, safety)
- [ ] 1.2 Define pin assignments and constants in configuration header
- [ ] 1.3 Setup platformio.ini with required ESP-IDF libraries
- [ ] 1.4 Create base class headers for each module

## 2. Sensor Monitoring Implementation
- [ ] 2.1 Implement DHT22 driver class for room temperature/humidity
- [ ] 2.2 Implement DS18B20 driver class for mattress temperature
- [ ] 2.3 Add sensor validation and error detection logic
- [ ] 2.4 Create sensor data structure for readings
- [ ] 2.5 Implement periodic sensor reading (every 2-5 seconds)

## 3. Heating Control System
- [ ] 3.1 Create heating controller class with hysteresis logic (2°C default)
- [ ] 3.2 Implement GPIO control for heating element relay
- [ ] 3.3 Add temperature setpoint management (20-45°C range)
- [ ] 3.4 Implement heating on/off state machine
- [ ] 3.5 Add safety shutoff on sensor failure

## 4. Timer Management
- [ ] 4.1 Create software timer class using ESP32 timer API
- [ ] 4.2 Implement countdown timer (15 min - 8 hours range)
- [ ] 4.3 Add timer start/stop/reset functionality
- [ ] 4.4 Implement automatic heating shutoff on timer expiration
- [ ] 4.5 Add remaining time calculation

## 5. Settings Storage
- [ ] 5.1 Initialize ESP32 Preferences library
- [ ] 5.2 Implement temperature setpoint persistence
- [ ] 5.3 Implement timer duration persistence
- [ ] 5.4 Add default values (25°C, heating OFF)
- [ ] 5.5 Create load/save settings functions

## 6. Safety Features
- [ ] 6.1 Enable and configure watchdog timer
- [ ] 6.2 Implement sensor disconnection detection
- [ ] 6.3 Add heating element emergency shutoff
- [ ] 6.4 Create system health monitoring
- [ ] 6.5 Implement safe boot state (heating OFF)

## 7. Integration & Testing
- [ ] 7.1 Integrate all modules in main.cpp
- [ ] 7.2 Test temperature reading accuracy
- [ ] 7.3 Test heating control with hysteresis
- [ ] 7.4 Test timer countdown and auto-shutoff
- [ ] 7.5 Test settings persistence across reboots
- [ ] 7.6 Test safety features (sensor disconnect, watchdog)
- [ ] 7.7 Validate temperature and timer ranges
- [ ] 7.8 Serial logging for debugging and monitoring

