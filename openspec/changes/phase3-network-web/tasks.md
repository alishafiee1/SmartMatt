# Phase 3 Implementation Tasks

## 1. WiFi Manager Foundation
- [ ] 1.1 Create WiFiManager class for mode management
- [ ] 1.2 Initialize ESP32 WiFi radio on boot
- [ ] 1.3 Implement WiFi event handler for connection events
- [ ] 1.4 Add WiFi mode state machine (OFF, AP, STA, AP+STA)
- [ ] 1.5 Test basic WiFi initialization

## 2. SoftAP (Access Point) Mode
- [ ] 2.1 Configure SoftAP with SSID "RODI_XXXX" (XXXX = device MAC suffix)
- [ ] 2.2 Set SoftAP password or open mode (configurable)
- [ ] 2.3 Configure IP address (192.168.4.1) and DHCP server
- [ ] 2.4 Implement SoftAP enable/disable functions
- [ ] 2.5 Set SoftAP as default enabled on first boot
- [ ] 2.6 Test client connection to SoftAP

## 3. Station (Client) Mode & Provisioning
- [ ] 3.1 Implement WiFi credentials storage (SSID, password in NVS)
- [ ] 3.2 Create Station mode connection logic with auto-reconnect
- [ ] 3.3 Implement connection timeout (30 seconds)
- [ ] 3.4 Add provisioning API endpoint (POST WiFi credentials)
- [ ] 3.5 Test connection to home router
- [ ] 3.6 Implement credential validation before storing

## 4. Dual-Mode Operation
- [ ] 4.1 Enable simultaneous SoftAP + Station mode
- [ ] 4.2 Handle IP routing for both interfaces
- [ ] 4.3 Implement fallback: keep SoftAP if Station fails
- [ ] 4.4 Test accessing device from both AP and LAN
- [ ] 4.5 Verify no interference between modes

## 5. SoftAP Toggle Integration
- [ ] 5.1 Connect Phase 2 power button long press to SoftAP toggle
- [ ] 5.2 Implement SoftAP enable/disable with visual feedback
- [ ] 5.3 Update OLED WiFi icon when SoftAP state changes
- [ ] 5.4 Persist SoftAP enabled state to NVS
- [ ] 5.5 Test button long press → SoftAP toggle cycle

## 6. HTTP Web Server
- [ ] 6.1 Initialize ESP-IDF HTTP server on port 80
- [ ] 6.2 Register URI handlers for static files and API
- [ ] 6.3 Implement CORS headers for browser compatibility
- [ ] 6.4 Add request logging for debugging
- [ ] 6.5 Test basic HTTP GET from browser

## 7. WebSocket Real-Time Server
- [ ] 7.1 Implement WebSocket upgrade handler
- [ ] 7.2 Create WebSocket message queue for outgoing data
- [ ] 7.3 Implement push channel for real-time updates
- [ ] 7.4 Add JSON message formatting
- [ ] 7.5 Implement Option=ALL (full snapshot) on connection
- [ ] 7.6 Implement Option=CHANGE (delta updates) for ongoing
- [ ] 7.7 Test WebSocket connection and data streaming

## 8. REST API Endpoints
- [ ] 8.1 POST /api/heating/enable - Enable heating system
- [ ] 8.2 POST /api/heating/disable - Disable heating system
- [ ] 8.3 POST /api/temperature - Set temperature setpoint
- [ ] 8.4 POST /api/timer - Set timer duration
- [ ] 8.5 GET /api/status - Get current system status
- [ ] 8.6 POST /api/wifi - WiFi provisioning (SSID, password)
- [ ] 8.7 GET /api/wifi - Get WiFi status (without password)
- [ ] 8.8 POST /api/softap - Toggle SoftAP mode
- [ ] 8.9 Implement input validation for all endpoints
- [ ] 8.10 Return appropriate HTTP status codes and error messages
- [ ] 8.11 Test all API endpoints with curl/Postman

## 9. WebSocket Real-Time Data Protocol
- [ ] 9.1 Define JSON message format for sensor data
- [ ] 9.2 Implement temperature/humidity push on change
- [ ] 9.3 Implement heating state push on change
- [ ] 9.4 Send timer value only on: page load, timer change by user
- [ ] 9.5 Implement client-side timer countdown in JavaScript
- [ ] 9.6 Add connection/disconnection handling
- [ ] 9.7 Test real-time updates in browser console

## 10. Web Panel UI - HTML Structure
- [ ] 10.1 Create responsive HTML layout (mobile + desktop)
- [ ] 10.2 Add temperature control section (setpoint, current temp)
- [ ] 10.3 Add timer control section
- [ ] 10.4 Add power on/off toggle button
- [ ] 10.5 Add WiFi provisioning form (SSID, password)
- [ ] 10.6 Add status indicators (heating, timer, WiFi)
- [ ] 10.7 Test HTML structure renders correctly

## 11. Web Panel UI - CSS Styling
- [ ] 11.1 Design modern, clean IoT-style interface
- [ ] 11.2 Implement responsive breakpoints (mobile, tablet, desktop)
- [ ] 11.3 Style temperature display prominently
- [ ] 11.4 Add visual feedback for button presses (active states)
- [ ] 11.5 Implement loading states during API calls
- [ ] 11.6 Add error message styling
- [ ] 11.7 Test UI on multiple screen sizes

## 12. Web Panel UI - JavaScript Logic
- [ ] 12.1 Implement WebSocket connection on page load
- [ ] 12.2 Parse and display WebSocket messages (real-time updates)
- [ ] 12.3 Implement client-side timer countdown display
- [ ] 12.4 Create temperature increment/decrement buttons
- [ ] 12.5 Create timer increment/decrement buttons
- [ ] 12.6 Implement power on/off API calls
- [ ] 12.7 Implement WiFi provisioning form submission
- [ ] 12.8 Add error handling and user feedback messages
- [ ] 12.9 Implement auto-reconnect on WebSocket disconnect
- [ ] 12.10 Test all interactive elements

## 13. WiFi Status Display Integration
- [ ] 13.1 Update Phase 2 display WiFi icon for SoftAP mode
- [ ] 13.2 Update Phase 2 display WiFi icon for Station connected
- [ ] 13.3 Update Phase 2 display WiFi icon for Station disconnected
- [ ] 13.4 Add WiFi mode indicator (AP/STA/Both)
- [ ] 13.5 Test WiFi icon updates on state changes

## 14. Settings Storage Extension
- [ ] 14.1 Add "wifi_ssid" key to NVS
- [ ] 14.2 Add "wifi_pass" key to NVS (encrypted if possible)
- [ ] 14.3 Add "softap_enabled" key to NVS
- [ ] 14.4 Implement WiFi credentials load on boot
- [ ] 14.5 Implement WiFi credentials save on provisioning
- [ ] 14.6 Test settings persistence across reboots

## 15. Security & Validation
- [ ] 15.1 Validate SSID length (1-32 characters)
- [ ] 15.2 Validate password length (8-63 characters for WPA2)
- [ ] 15.3 Sanitize user inputs in API
- [ ] 15.4 Implement rate limiting for API endpoints (optional)
- [ ] 15.5 Test with malformed requests

## 16. Integration Testing
- [ ] 16.1 Test complete flow: SoftAP → Web panel → Control device
- [ ] 16.2 Test complete flow: Provisioning → Station mode → LAN access
- [ ] 16.3 Test dual-mode: Access from both AP and LAN simultaneously
- [ ] 16.4 Test WebSocket real-time updates during sensor changes
- [ ] 16.5 Test API control vs. physical button control (no conflicts)
- [ ] 16.6 Test SoftAP toggle via long press
- [ ] 16.7 Verify settings persistence across multiple reboots

## 17. Performance & Stability
- [ ] 17.1 Test WebSocket with multiple concurrent connections
- [ ] 17.2 Monitor memory usage with WiFi and HTTP server active
- [ ] 17.3 Test long-term stability (24+ hour operation)
- [ ] 17.4 Verify no memory leaks in WebSocket/HTTP handlers
- [ ] 17.5 Test WiFi reconnection after router reboot

## 18. Documentation
- [ ] 18.1 Document default SoftAP SSID and password
- [ ] 18.2 Document all REST API endpoints and parameters
- [ ] 18.3 Document WebSocket message format
- [ ] 18.4 Create user guide for WiFi provisioning
- [ ] 18.5 Document troubleshooting steps

