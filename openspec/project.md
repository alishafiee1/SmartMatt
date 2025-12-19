# Project Context

## Purpose
RODI Smart Mattress is an IoT device that provides intelligent temperature control for mattresses using a heating element. The system allows users to control temperature and timing through multiple interfaces (device panel, web panel, Android app) with direct WiFi access and local network connectivity.

## Tech Stack
- **Microcontroller**: ESP32 (ESP-IDF framework)
- **Sensors**: 
  - DHT22 (External temperature & humidity)
  - DS18B20 (Mattress internal temperature)
- **Display**: OLED SSD1306
- **Communication**: WiFi (SoftAP + Station), WebSocket, REST API
- **Storage**: ESP32 Preferences library (NVS)
- **Frontend**: Web panel (HTML/CSS/JavaScript)
- **Mobile**: Android application

## Project Conventions

### Code Style
- **Architecture**: Feature-based folder structure
- **Language**: C++ for ESP-IDF firmware
- **Comments**: English language only
- **Naming Conventions**:
  - Classes: PascalCase
  - Variables: camelCase
  - Constants: UPPER_SNAKE_CASE
  - Pin definitions: Defined at program start
- **File Organization**: Header classes for microcontroller modules
- **Code Quality**: Clean, standard, and well-organized code

### Architecture Patterns
- **Feature-Based Structure**: Organized by functionality (sensors, display, heating, network, etc.)
- **Direct WiFi Access**: ESP32 as SoftAP for internet-independent operation
- **Hybrid Communication**:
  - WebSocket for real-time data streaming (push channel)
  - REST API for commands and settings
- **Data Push Strategy**: 
  - Send only on change (Option=CHANGE)
  - Full snapshot on initial connection (Option=ALL)
- **Client-Side Timer**: Timer countdown handled in UI to reduce network traffic
- **JSON Standard**: All messages and API responses use JSON format

### Testing Strategy
- **Safety First**: Sensor failure = heating element OFF
- **Watchdog Timer**: Enabled for system reliability
- **Default Safe State**: Heating system OFF on boot/reset
- **Range Validation**: 
  - Temperature: 20-45°C
  - Timer: 15 minutes - 8 hours
  - Hysteresis: 2°C (configurable)

### Git Workflow
- **Team Members**: 
  - HW Design: H.Rostamizade
  - SW Development: AliShafiee
  - Project Team: RODI
- **Commit Messages**: Clear and descriptive
- **Documentation**: Keep RTL Persian documentation in sync

## Domain Context
- **Smart Home IoT Device**: Temperature-controlled mattress heating system
- **User Interfaces**: 
  1. Physical panel with buttons (on device)
  2. Web panel (browser-based via SoftAP or LAN)
  3. Android application
- **Safety Critical**: Temperature sensor failure must immediately disable heating
- **Persistent Settings**: WiFi credentials, temperature setpoint, and timer stored in NVS
- **Default Values**: 25°C temperature, heating OFF on startup
- **Button Behavior**:
  - Short press: ±1°C or ±15 minutes
  - Long press: Accelerated change
  - Power button hold (3s): Toggle SoftAP on/off
- **Display Layout**: 
  - Center: Room temperature & humidity
  - Right: Timer remaining
  - Left: Temperature setpoint
  - Top-right: WiFi icon
  - Top-left: Heating element icon

## Important Constraints
- **Temperature Limits**: 20-45°C (safety range)
- **Timer Limits**: 15 minutes - 8 hours
- **Temperature Hysteresis**: 2°C (configurable in code)
- **Default Temperature**: 25°C
- **Timer Precision**: Internal ESP32 timer (high precision not required)
- **Safety Protocol**: Disconnected mattress sensor = element OFF + error message
- **Boot Sequence**: 
  1. Display Rodiset.ir animation
  2. Display "RODI" large font (3 seconds)
  3. Hardware setup and normal operation
- **Network**: Must work without internet (SoftAP mode)
- **Storage**: Use lightweight Preferences library for settings persistence

## External Dependencies
- **ESP-IDF Framework**: Core development framework
- **Sensors**:
  - DHT22 library for temperature/humidity
  - OneWire/DallasTemperature for DS18B20
- **Display**: 
  - SSD1306 OLED driver library
  - Custom animation for status changes
- **Networking**: 
  - ESP32 WiFi stack (SoftAP + Station modes)
  - WebSocket library for real-time communication
  - HTTP server for REST API
- **Storage**: Preferences library (NVS wrapper)
- **Android SDK**: For mobile application development

## Feature Capabilities
- Real-time temperature monitoring (room + mattress)
- Heating element control with hysteresis
- Timer-based automatic shutoff
- Multi-interface control (panel, web, mobile)
- Direct WiFi access (SoftAP)
- Local network integration (WiFi provisioning)
- Persistent settings storage
- Safety features (sensor monitoring, watchdog)
- Visual feedback (OLED animations, icons)
