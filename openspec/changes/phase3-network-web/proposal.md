# Phase 3: WiFi Network and Web Panel

## Why
Users need remote access to the RODI Smart Mattress for monitoring and control without physical proximity to the device. This phase enables WiFi connectivity (both direct access via SoftAP and local network via Station mode) and provides a web-based control panel accessible from any browser.

## What Changes
- Implement WiFi SoftAP (Access Point) mode for direct device connection
- Implement WiFi Station mode for home network integration (provisioning)
- Create WebSocket server for real-time data streaming (temperature, status updates)
- Build REST API for command and control operations
- Develop responsive web panel UI (HTML/CSS/JavaScript)
- Add WiFi credentials storage in NVS
- Implement SoftAP toggle via power button long press (Phase 2 integration)
- Create WiFi status indication on OLED display
- Support dual-mode operation (SoftAP + Station simultaneously)

## Impact
- **Affected specs**: 
  - `wifi-softap` (new)
  - `wifi-station` (new)
  - `websocket-realtime` (new)
  - `rest-api` (new)
  - `web-panel` (new)
  - `settings-storage` (modified - add WiFi credentials)
  - `button-interface` (modified - power button long press activates SoftAP toggle)
  - `display-interface` (modified - WiFi icon states)
- **Affected code**: 
  - `src/network/` - WiFi manager, SoftAP, Station modes
  - `src/server/` - HTTP server, WebSocket handler
  - `src/api/` - REST API endpoints
  - `data/` - Web panel static files (HTML/CSS/JS)
  - `src/storage/` - WiFi credentials persistence (extends Phase 1)
- **Hardware dependencies**: 
  - ESP32 WiFi radio
  - Network connectivity (router for Station mode)
- **Phase 1 & 2 integration**:
  - Web panel controls Phase 1 heating system
  - WebSocket streams Phase 1 sensor data
  - SoftAP toggle uses Phase 2 button long press
  - WiFi status shown on Phase 2 display

## Non-Goals (for this phase)
- Mobile application (deferred to Phase 4)
- Cloud connectivity or remote internet access
- OTA (Over-The-Air) firmware updates
- MQTT or other IoT protocols
- Complex user authentication (basic operation only)

## Prerequisites
- Phase 1 (Core Hardware Control) completed
- Phase 2 (Device Panel Interface) completed
- ESP-IDF HTTP server and WebSocket libraries available
- Basic understanding of HTML/CSS/JavaScript for web panel

## Breaking Changes
- **MODIFIED** `settings-storage`: Adds WiFi SSID and password keys to NVS
- **MODIFIED** `button-interface`: Power button long press (3s) now toggles SoftAP
- **MODIFIED** `display-interface`: WiFi icon now shows connection state

