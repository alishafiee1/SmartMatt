# Phase 3: WiFi Network and Web Panel - Design Document

## Context
Phase 3 transforms the RODI Smart Mattress from a standalone device into a network-connected IoT system. Users can access the device through a web browser either by connecting directly to the device's WiFi (SoftAP mode) or through their home network (Station mode). Real-time monitoring and control are provided via WebSocket for live data and REST API for commands.

### Stakeholders
- End users requiring remote monitoring and control
- Home network integration requirements
- Web UI design requiring responsive, modern interface
- Phase 1 & 2 maintaining stability with network layer added

### Constraints
- ESP32 memory limitations (~320KB RAM available)
- Single-core performance with WiFi + WebSocket + HTTP
- Browser compatibility (modern browsers only, no IE)
- No external dependencies (self-hosted web panel)
- Must work without internet connectivity (offline-first)

## Goals / Non-Goals

### Goals
- Direct WiFi access without router dependency (SoftAP)
- Home network integration for convenient access (Station)
- Real-time web interface with live sensor updates
- Standardized REST API for future extensibility
- Responsive UI working on mobile and desktop browsers
- Persistent WiFi configuration across reboots
- User-friendly provisioning process

### Non-Goals
- Native mobile apps (Phase 4)
- Cloud services or external APIs
- HTTPS/SSL encryption (local network only)
- User authentication or multi-user support
- OTA firmware updates
- Advanced networking (mDNS, UPnP)

## Technical Decisions

### 1. WiFi Architecture: Dual-Mode (AP + STA)
**Decision**: Support both SoftAP and Station modes simultaneously

**Rationale**:
- SoftAP allows initial setup without existing WiFi
- Station mode enables convenient home network access
- Dual-mode provides fallback: SoftAP always available if Station fails
- Users can access from both direct connection and LAN

**Configuration**:
```c
// SoftAP Configuration
SSID: "RODI_" + last 4 MAC digits  (e.g., "RODI_A3F2")
Password: "rodi1234" (or open, configurable)
IP: 192.168.4.1
Channel: 1 (2.4GHz)
Max Connections: 4

// Station Configuration
SSID: User-provided
Password: User-provided
IP: DHCP (auto-assigned by router)
Auto-reconnect: Enabled
Max Retries: 3
```

**Trade-offs**:
- Memory overhead: ~40KB for dual-mode vs. 25KB single mode
- Complexity: More state management
- Benefit: Better user experience, no lockout scenarios

### 2. Communication Protocol: WebSocket + REST Hybrid
**Decision**: Use WebSocket for real-time sensor data, REST API for commands

**Rationale**:
- WebSocket provides efficient push for continuous sensor updates
- REST provides standard, stateless command interface
- Separating concerns: monitoring vs. control
- REST easily testable with curl/Postman
- WebSocket reduces polling overhead

**Protocol Design**:
```
┌─────────────┐                      ┌─────────────┐
│  Browser    │                      │   ESP32     │
│             │                      │             │
│  ┌─────┐    │    WebSocket         │  ┌────────┐ │
│  │ UI  │◄───┼──────────────────────┼──│Sensor  │ │
│  └─────┘    │    (push data)       │  │Monitor │ │
│     │       │                      │  └────────┘ │
│     │       │    REST API          │             │
│     └───────┼──────────────────────┼──►Control   │
│             │    (commands)        │   APIs      │
└─────────────┘                      └─────────────┘
```

**Alternatives considered**:
- REST only with polling: Rejected due to inefficiency and latency
- WebSocket only (bidirectional): Rejected as REST is more standard for commands
- MQTT: Rejected as overkill for single-device local network

### 3. WebSocket Data Push Strategy
**Decision**: Send data only on change (Option=CHANGE) except initial snapshot (Option=ALL)

**Message Format**:
```json
// Initial connection (Option=ALL)
{
  "type": "snapshot",
  "data": {
    "room_temp": 25.5,
    "room_humidity": 65,
    "mattress_temp": 35.2,
    "setpoint": 35,
    "heating_on": true,
    "timer_remaining_sec": 3600,
    "timer_duration_min": 60,
    "wifi_mode": "AP+STA"
  },
  "timestamp": 1703001234
}

// Subsequent updates (Option=CHANGE)
{
  "type": "update",
  "data": {
    "room_temp": 25.6,
    "heating_on": false
  },
  "timestamp": 1703001240
}
```

**Rationale**:
- Reduces network traffic (only send changes)
- Full snapshot on connect ensures UI has complete state
- Timestamp allows client-side latency detection
- JSON is universally supported and human-readable

**Timer Handling**:
- Timer value sent only on: page load, user timer change, timer expiration
- Client-side JavaScript handles countdown display
- Reduces WebSocket messages from 1/sec to ~1/15min

### 4. REST API Design
**Decision**: RESTful endpoints with JSON request/response

**API Specification**:
```
POST /api/heating/enable
  Response: {"success": true, "heating_on": true}

POST /api/heating/disable
  Response: {"success": true, "heating_on": false}

POST /api/temperature
  Body: {"setpoint": 35}
  Response: {"success": true, "setpoint": 35}

POST /api/timer
  Body: {"duration_min": 120}
  Response: {"success": true, "duration_min": 120}

GET /api/status
  Response: {full system status JSON}

POST /api/wifi
  Body: {"ssid": "HomeWiFi", "password": "secret123"}
  Response: {"success": true, "message": "Connecting..."}

GET /api/wifi
  Response: {"mode": "AP+STA", "ssid": "HomeWiFi", "connected": true, "ip": "192.168.1.100"}

POST /api/softap
  Body: {"enabled": true}
  Response: {"success": true, "softap_enabled": true}
```

**Error Responses**:
```json
{
  "success": false,
  "error": "Invalid temperature range (20-45°C)"
}
```

**Rationale**:
- Standard HTTP methods and status codes
- JSON is lightweight and JavaScript-native
- Clear error messages for debugging
- Stateless design (no session management)

### 5. Web Panel Architecture
**Decision**: Single-page application (SPA) with vanilla JavaScript

**Tech Stack**:
- HTML5 for structure
- CSS3 for styling (no frameworks, custom responsive design)
- Vanilla JavaScript for logic (no jQuery, React, etc.)
- WebSocket API (native browser support)
- Fetch API for REST calls

**Rationale**:
- No external dependencies or CDNs (works offline)
- Minimal file size fits in ESP32 SPIFFS (~100KB for all files)
- Modern browsers support all features natively
- Faster page load without framework overhead
- Simpler debugging and maintenance

**File Structure**:
```
data/
├── index.html           (Main page, ~15KB)
├── style.css            (Styling, ~10KB)
├── app.js               (Application logic, ~20KB)
├── icons/               (SVG icons, ~5KB total)
└── favicon.ico          (Optional, 1KB)
```

**Alternatives considered**:
- React/Vue framework: Rejected due to size and complexity
- Server-side rendering: Rejected as ESP32 underpowered for templating
- Mobile-first framework (Bootstrap): Rejected due to size, custom is sufficient

### 6. WiFi Provisioning Flow
**Decision**: Web-based provisioning through SoftAP interface

**Provisioning Steps**:
```
1. User powers on device (first boot)
2. Device starts SoftAP "RODI_XXXX"
3. User connects phone/laptop to RODI_XXXX WiFi
4. User opens browser to http://192.168.4.1
5. Web panel loads with WiFi setup form
6. User enters home WiFi SSID and password
7. Device validates and attempts Station connection
8. On success: Device joins home network (dual-mode active)
9. User can now access from home network IP
10. SoftAP remains active for fallback access
```

**Validation**:
- SSID: 1-32 characters, non-empty
- Password: 8-63 characters (WPA2 requirement) or empty (open network)
- Test connection before persisting credentials
- Timeout after 30 seconds if connection fails

**Fallback**:
- If Station connection fails, remain in SoftAP mode
- Display error message in web panel
- Allow user to retry with different credentials
- SoftAP never disabled automatically

### 7. SoftAP Toggle Implementation
**Decision**: Power button long press (3 seconds) toggles SoftAP on/off

**Rationale**:
- Users may want to disable SoftAP for security/battery reasons
- Long press prevents accidental triggering
- Visual feedback on OLED display
- Persists across reboots

**Behavior**:
```
SoftAP OFF + Long Press → SoftAP ON
SoftAP ON + Long Press → SoftAP OFF
```

**Safety**:
- Cannot disable SoftAP if Station not connected (prevents lockout)
- Warning message on display before disabling
- Can always re-enable via button

## Network Architecture

```
┌─────────────────────────────────────────────────┐
│                   ESP32                         │
│                                                 │
│  ┌──────────────┐         ┌──────────────┐     │
│  │   SoftAP     │         │   Station    │     │
│  │ 192.168.4.1  │         │   (DHCP)     │     │
│  └──────┬───────┘         └──────┬───────┘     │
│         │                        │             │
│         │    ┌──────────────┐    │             │
│         └────┤  HTTP Server │────┘             │
│              │  WebSocket   │                  │
│              └──────────────┘                  │
│                     │                          │
│              ┌──────┴───────┐                  │
│              │  Phase 1 & 2 │                  │
│              │   Control    │                  │
│              └──────────────┘                  │
└─────────────────────────────────────────────────┘
         │                        │
         │                        │
    ┌────▼─────┐            ┌─────▼──────┐
    │ Phone/PC │            │   Router    │
    │  Direct  │            │   (LAN)     │
    │ Connect  │            │             │
    └──────────┘            └─────┬───────┘
                                  │
                            ┌─────▼──────┐
                            │  Home      │
                            │  Devices   │
                            └────────────┘
```

## Web Panel UI Design

### Layout Structure
```
┌─────────────────────────────────────────┐
│  RODI Smart Mattress                    │  ← Header
├─────────────────────────────────────────┤
│                                         │
│  Status: [●] Heating  [📶] WiFi: LAN   │  ← Status Bar
│                                         │
│  ┌───────────────────────────────────┐  │
│  │     Room Temperature              │  │
│  │         25.5°C    65%             │  │  ← Large Display
│  │                                   │  │
│  └───────────────────────────────────┘  │
│                                         │
│  Temperature Setpoint                   │
│  [  -  ]    35°C    [  +  ]            │  ← Temperature Control
│                                         │
│  Timer                                  │
│  [  -  ]  2:30:00  [  +  ]             │  ← Timer Control
│                                         │
│  [ Power ON / OFF ]                     │  ← Main Control
│                                         │
│  ─────── WiFi Setup ───────             │
│  SSID:     [____________]               │
│  Password: [____________]               │
│  [ Connect ]                            │  ← Provisioning
│                                         │
└─────────────────────────────────────────┘
```

### Responsive Breakpoints
- Mobile (<600px): Single column, stacked elements
- Tablet (600-900px): 2-column layout for controls
- Desktop (>900px): Centered max-width 800px

### Color Scheme
- Primary: #2196F3 (blue, IoT standard)
- Accent: #FF5722 (orange, heating indicator)
- Background: #F5F5F5 (light gray)
- Text: #333333 (dark gray)
- Success: #4CAF50 (green)
- Error: #F44336 (red)

## Data Flow Diagrams

### Real-Time Sensor Update
```
Sensor Reading Changed
        ↓
Phase 1 Sensor Monitor
        ↓
WebSocket Message Queue
        ↓
JSON Serialization (Option=CHANGE)
        ↓
WebSocket Broadcast to Connected Clients
        ↓
Browser Receives Message
        ↓
JavaScript Updates DOM
        ↓
UI Displays New Value (no page refresh)
```

### User Command Flow
```
User Clicks "Temperature +" Button
        ↓
JavaScript Event Handler
        ↓
Fetch API POST /api/temperature {setpoint: 36}
        ↓
ESP32 HTTP Handler Receives Request
        ↓
Input Validation (20-45°C range)
        ↓
Phase 1 Heating Controller API Call
        ↓
Settings Storage (NVS Write)
        ↓
HTTP Response {"success": true, "setpoint": 36}
        ↓
WebSocket Push (Option=CHANGE)
        ↓
UI Updates Display (confirmation)
```

## Security Considerations

### Current Phase (Phase 3)
- **No encryption**: HTTP only (not HTTPS)
- **No authentication**: Open access to web panel
- **Local network only**: Not exposed to internet
- **WiFi password**: Stored in NVS (plaintext)

**Rationale**:
- HTTPS requires TLS/SSL: Too memory-intensive for ESP32
- Target users: Home environment, trusted network
- Phase 3 focuses on functionality, not hardened security

### Future Enhancements (Post-Phase 4)
- Basic authentication (username/password)
- HTTPS with self-signed certificate
- WiFi password encryption in NVS
- API rate limiting

### Mitigation for Phase 3
- SoftAP password protects direct access
- Home network firewall protects LAN access
- No sensitive data stored (just settings)
- Clear documentation about security limitations

## Performance Targets

### Network Performance
- WebSocket connection latency: <100ms
- REST API response time: <50ms
- Web panel page load: <2 seconds on SoftAP
- WebSocket message rate: <10 messages/sec typical
- Concurrent connections: Up to 4 clients

### Memory Footprint
- WiFi stack: ~40KB RAM
- HTTP server: ~20KB RAM
- WebSocket: ~15KB RAM per connection
- Web files (SPIFFS): ~50KB flash
- Total Phase 3: ~100KB RAM, ~80KB flash

### CPU Usage
- WiFi + HTTP: ~15% CPU
- WebSocket per connection: ~5% CPU
- Total with 2 connections: ~25% CPU
- Remaining for Phase 1 & 2: ~75% CPU

## Error Handling

### WiFi Errors
- **Station connection failure**: Remain in SoftAP, show error in web panel
- **DHCP timeout**: Retry 3 times, fallback to SoftAP only
- **SoftAP start failure**: Log error, attempt restart
- **Password mismatch**: Clear feedback in provisioning UI

### HTTP/WebSocket Errors
- **WebSocket disconnect**: Auto-reconnect with exponential backoff
- **HTTP timeout**: Display loading indicator, timeout after 10s
- **Malformed request**: Return 400 Bad Request with error message
- **Rate limit exceeded**: Return 429 Too Many Requests

### Integration Errors
- **Phase 1 API failure**: Return error in REST response, log to serial
- **Settings save failure**: Warn user but continue operation
- **Memory allocation failure**: Reject new connections gracefully

## Testing Strategy

### Unit Tests (Manual)
- WiFi connection to various routers
- SoftAP client connections (multiple devices)
- REST API with curl (all endpoints)
- WebSocket connection and disconnect handling
- Provisioning with valid/invalid credentials

### Integration Tests
- Web panel → REST API → Phase 1 control
- Phase 1 sensors → WebSocket → Web panel display
- Button control vs. web control (no conflicts)
- SoftAP toggle via button → WiFi status update

### Stress Tests
- 4 concurrent web panel connections
- Rapid API requests (10/sec for 1 minute)
- WebSocket message flood handling
- Long-term stability (48 hours connected)

### Browser Compatibility
- Chrome/Edge (Chromium-based)
- Firefox
- Safari (iOS and macOS)
- Mobile browsers (Android Chrome, iOS Safari)

## Migration Plan

### Phase 2 → Phase 3 Integration
- Add WiFi status to Phase 2 OLED display
- Connect power button long press to SoftAP toggle
- Extend settings storage for WiFi credentials
- No breaking changes to Phase 1 or Phase 2 APIs

### Phase 3 → Phase 4 Preparation
- REST API designed for mobile app reuse
- WebSocket protocol compatible with Android app
- Standardized JSON format for all data
- API versioning for future compatibility

## Risks / Trade-offs

### Risk: WiFi Interference
- **Impact**: Connection dropouts or slow performance
- **Mitigation**: Use 2.4GHz channel 1, 6, or 11 (avoid overlap)
- **Trade-off**: Cannot use 5GHz (ESP32 limitation)

### Risk: Memory Exhaustion
- **Impact**: System crash with multiple WebSocket connections
- **Mitigation**: Limit to 4 concurrent connections, monitor heap
- **Trade-off**: May reject new connections when limit reached

### Risk: Browser Compatibility
- **Impact**: Older browsers may not support WebSocket or Fetch API
- **Mitigation**: Detect unsupported browsers, show upgrade message
- **Trade-off**: No support for IE11 or older Android browsers

### Risk: WiFi Provisioning Lockout
- **Impact**: User provides wrong credentials and loses access
- **Mitigation**: Keep SoftAP always available, never auto-disable
- **Trade-off**: SoftAP uses extra power and memory

### Risk: WebSocket Reconnection Storm
- **Impact**: Multiple clients reconnecting simultaneously could overwhelm ESP32
- **Mitigation**: Exponential backoff on client side, connection queue on server
- **Trade-off**: Slight delay in reconnection

## Open Questions

1. **Q**: Should SoftAP SSID be customizable by user?
   **A**: No in Phase 3 - fixed format for simplicity. Possible in future.

2. **Q**: Should we implement mDNS for "rodi.local" hostname?
   **A**: Deferred - nice-to-have but not essential, adds complexity.

3. **Q**: Maximum WebSocket message size?
   **A**: 1KB per message (more than sufficient for sensor data).

4. **Q**: Should web panel work offline (service worker cache)?
   **A**: Not needed - always served from ESP32, no external dependencies.

5. **Q**: What if user forgets SoftAP password?
   **A**: Factory reset option (hold power button 10 seconds) - implement in Phase 3.

## Dependencies

### Hardware Dependencies
- ESP32 WiFi radio (built-in)
- Adequate antenna (PCB antenna or external)
- Stable power supply (WiFi increases current draw to ~200mA)

### Software Dependencies
- ESP-IDF WiFi libraries
- ESP-IDF HTTP server (esp_http_server component)
- WebSocket library (may need third-party or custom implementation)
- SPIFFS for web file storage
- Phase 1 APIs (heating control, sensors, timer, storage)
- Phase 2 APIs (display update for WiFi status)

### External Dependencies
- WiFi router (for Station mode)
- Browser on user device (Chrome/Firefox/Safari)
- Local network infrastructure (router, DHCP server)

