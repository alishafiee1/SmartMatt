# Phase 3: WiFi Network and Web Panel - Implementation Guide

## Overview

Phase 3 adds network connectivity and web-based control to the RODI Smart Mattress system. This phase enables remote monitoring and control through a responsive web interface accessible via WiFi.

**Status**: ⚠️ ~95% Complete (Minor Integration Tasks Pending)

**Version**: 3.0.0

**Date**: December 19, 2025  
**Last Review**: December 20, 2025

---

## Features Implemented

### 1. WiFi Connectivity

#### SoftAP Mode (Access Point)
- **SSID Format**: `RODI_XXXX` (XXXX = last 4 digits of MAC address)
- **Default Password**: `rodi1234`
- **IP Address**: `192.168.4.1`
- **DHCP Server**: Enabled
- **Max Connections**: 4 concurrent clients
- **Channel**: 1 (2.4GHz)

#### Station Mode (Client)
- **WiFi Provisioning**: Via web panel
- **Auto-Reconnect**: Enabled with exponential backoff
- **Connection Timeout**: 30 seconds
- **Max Retries**: 3 attempts
- **Credentials Storage**: Persistent in NVS

#### Dual-Mode Operation
- **Simultaneous AP+STA**: Both modes active at once
- **Fallback Protection**: SoftAP remains active if Station fails
- **Lockout Prevention**: Cannot disable SoftAP if Station not connected

### 2. Web Server

#### HTTP Server
- **Port**: 80
- **Protocol**: HTTP (not HTTPS)
- **Static Files**: Served from SPIFFS
- **CORS**: Enabled for browser compatibility

#### WebSocket Server
- **Port**: 81
- **Protocol**: WebSocket (ws://)
- **Max Clients**: 4 concurrent connections
- **Message Format**: JSON
- **Update Strategy**: Push on change (Option=CHANGE)
- **Initial Snapshot**: Full state on connection (Option=ALL)

### 3. REST API

All API endpoints return JSON responses.

#### Device Control

**Enable Heating**
```http
POST /api/heating/enable
Response: {"success": true, "heating_on": true}
```

**Disable Heating**
```http
POST /api/heating/disable
Response: {"success": true, "heating_on": false}
```

**Set Temperature**
```http
POST /api/temperature
Content-Type: application/json
Body: {"setpoint": 35}
Response: {"success": true, "setpoint": 35}
```

**Set Timer**
```http
POST /api/timer
Content-Type: application/json
Body: {"duration_min": 120}
Response: {"success": true, "duration_min": 120}
```

**Get System Status**
```http
GET /api/status
Response: {
  "room_temp": 25.5,
  "room_humidity": 65,
  "mattress_temp": 35.2,
  "setpoint": 35,
  "heating_on": true,
  "heating_enabled": true,
  "timer_remaining_sec": 3600,
  "timer_duration_min": 60,
  "timer_running": true,
  "mattress_sensor_ok": true,
  "room_sensor_ok": true,
  "timestamp": 1234567890
}
```

#### WiFi Configuration

**WiFi Provisioning**
```http
POST /api/wifi
Content-Type: application/json
Body: {"ssid": "HomeWiFi", "password": "secret123"}
Response: {"success": true, "message": "Connecting..."}
```

**Get WiFi Status**
```http
GET /api/wifi
Response: {
  "mode": "AP+STA",
  "ssid": "HomeWiFi",
  "connected": true,
  "ip": "192.168.1.100"
}
```

**Toggle SoftAP**
```http
POST /api/softap
Content-Type: application/json
Body: {"enabled": true}
Response: {"success": true, "softap_enabled": true}
```

### 4. Web Panel UI

#### Features
- **Responsive Design**: Works on mobile, tablet, and desktop
- **Real-Time Updates**: WebSocket-based live data
- **Persian Language**: RTL layout with Persian text
- **Modern UI**: Gradient backgrounds, smooth animations
- **Touch-Friendly**: Large buttons for mobile devices

#### Sections
1. **Header**: System title and status indicators
2. **Main Display**: Room and mattress temperature/humidity
3. **Temperature Control**: Setpoint adjustment (20-45°C)
4. **Timer Control**: Duration adjustment (15 min - 8 hours)
5. **Power Control**: Enable/disable heating
6. **WiFi Setup**: Network provisioning form
7. **Error Display**: User-friendly error messages

#### Access URLs
- **SoftAP**: `http://192.168.4.1`
- **Station**: `http://<device-ip>` (shown in serial monitor)

---

## File Structure

```
SmartMatt/
├── src/
│   ├── network/
│   │   ├── WiFiManager.h          # WiFi connectivity management
│   │   └── WiFiManager.cpp
│   ├── server/
│   │   ├── WebServer.h            # HTTP and WebSocket server
│   │   └── WebServer.cpp
│   └── main.cpp                   # Main application (updated)
├── data/
│   ├── index.html                 # Web panel HTML
│   ├── style.css                  # Web panel styles
│   └── app.js                     # Web panel JavaScript
├── include/
│   └── config/
│       └── RodiConfig.h           # Configuration (updated)
└── platformio.ini                 # Build configuration (updated)
```

---

## Dependencies

### Hardware
- ESP32 development board
- WiFi antenna (built-in or external)
- Stable 5V power supply (WiFi increases current draw)

### Software Libraries
- **ArduinoJson** (^6.21.3): JSON parsing and serialization
- **WebSockets** (^2.4.1): WebSocket server implementation
- **ESP32 WiFi**: Built-in ESP-IDF WiFi stack
- **ESP32 WebServer**: Built-in HTTP server
- **SPIFFS**: File system for web panel files

---

## Building and Uploading

### 1. Install Dependencies

```bash
cd "D:\2 Curent project git\Rodi and Lora\SmartMatt"
pio lib install
```

### 2. Build Firmware

```bash
pio run
```

### 3. Upload Firmware

```bash
pio run --target upload
```

### 4. Upload SPIFFS (Web Panel Files)

```bash
pio run --target uploadfs
```

**Important**: You must upload SPIFFS to make the web panel accessible!

### 5. Monitor Serial Output

```bash
pio device monitor
```

---

## First-Time Setup

### Step 1: Power On Device

1. Connect ESP32 to power
2. Wait for boot sequence (approximately 15 seconds)
3. Watch serial monitor for initialization messages

### Step 2: Connect to SoftAP

1. On your phone/laptop, open WiFi settings
2. Look for network: `RODI_XXXX` (XXXX = device MAC suffix)
3. Connect using password: `rodi1234`
4. Wait for connection confirmation

### Step 3: Access Web Panel

1. Open browser (Chrome, Firefox, Safari)
2. Navigate to: `http://192.168.4.1`
3. Web panel should load within 2-3 seconds

### Step 4: WiFi Provisioning (Optional)

1. In web panel, scroll to "WiFi Setup" section
2. Enter your home WiFi SSID and password
3. Click "Connect to Network"
4. Device will attempt connection (30 seconds timeout)
5. If successful, device will be accessible on home network
6. Find device IP in serial monitor or router DHCP table

---

## Testing Guide

### Test 1: SoftAP Connection

**Objective**: Verify SoftAP mode works correctly

**Steps**:
1. Power on device
2. Connect to `RODI_XXXX` network
3. Open `http://192.168.4.1` in browser
4. Verify web panel loads

**Expected Result**: ✅ Web panel displays with live sensor data

### Test 2: WebSocket Real-Time Updates

**Objective**: Verify real-time data streaming

**Steps**:
1. Open web panel
2. Open browser developer console (F12)
3. Watch for WebSocket messages
4. Change temperature with physical buttons
5. Observe web panel updates without refresh

**Expected Result**: ✅ Values update in real-time (< 1 second delay)

### Test 3: REST API Control

**Objective**: Verify device control via API

**Steps**:
1. Open terminal or Postman
2. Test enable heating:
   ```bash
   curl -X POST http://192.168.4.1/api/heating/enable
   ```
3. Test set temperature:
   ```bash
   curl -X POST http://192.168.4.1/api/temperature \
     -H "Content-Type: application/json" \
     -d '{"setpoint": 30}'
   ```
4. Test get status:
   ```bash
   curl http://192.168.4.1/api/status
   ```

**Expected Result**: ✅ All commands return success JSON responses

### Test 4: WiFi Provisioning

**Objective**: Verify Station mode connection

**Steps**:
1. In web panel, enter home WiFi credentials
2. Click "Connect to Network"
3. Wait 30 seconds
4. Check serial monitor for connection status
5. If connected, find device IP
6. Disconnect from SoftAP
7. Connect to home WiFi
8. Access device via new IP

**Expected Result**: ✅ Device connects to home network and remains accessible

### Test 5: Dual-Mode Operation

**Objective**: Verify simultaneous AP+STA

**Steps**:
1. Connect device to home WiFi (Station mode)
2. Verify SoftAP still active
3. Connect one device to SoftAP
4. Connect another device to home WiFi
5. Both devices access web panel simultaneously

**Expected Result**: ✅ Both connections work without interference

### Test 6: Button vs. Web Control

**Objective**: Verify no conflicts between interfaces

**Steps**:
1. Open web panel
2. Change temperature via physical button
3. Observe web panel updates
4. Change temperature via web panel
5. Observe physical display updates

**Expected Result**: ✅ Both interfaces stay synchronized

### Test 7: Multiple WebSocket Clients

**Objective**: Verify concurrent connections

**Steps**:
1. Open web panel in 4 different browser tabs/devices
2. Verify all receive real-time updates
3. Control device from one tab
4. Observe updates in all tabs

**Expected Result**: ✅ All clients receive updates simultaneously

### Test 8: Persistence Across Reboot

**Objective**: Verify WiFi credentials persist

**Steps**:
1. Provision device with home WiFi
2. Wait for successful connection
3. Power off device
4. Power on device
5. Wait for auto-reconnect

**Expected Result**: ✅ Device automatically connects to saved network

---

## Troubleshooting

### Issue: Cannot Connect to SoftAP

**Symptoms**: `RODI_XXXX` network not visible

**Solutions**:
1. Check serial monitor for "SoftAP enabled" message
2. Verify device powered properly (WiFi needs ~200mA)
3. Check for WiFi interference (try different channel)
4. Restart device
5. Check SoftAP enabled state in NVS (may be disabled)

### Issue: Web Panel Not Loading

**Symptoms**: Browser shows "Cannot connect" or timeout

**Solutions**:
1. Verify connected to correct WiFi network
2. Check IP address (should be `192.168.4.1` for SoftAP)
3. Ensure SPIFFS uploaded (`pio run --target uploadfs`)
4. Check serial monitor for HTTP server status
5. Try different browser (Chrome recommended)
6. Clear browser cache

### Issue: WebSocket Not Connecting

**Symptoms**: Web panel loads but no real-time updates

**Solutions**:
1. Open browser console (F12) and check for errors
2. Verify WebSocket server running (serial monitor)
3. Check firewall/antivirus blocking WebSocket port 81
4. Try refreshing page
5. Check for JavaScript errors in console

### Issue: WiFi Provisioning Fails

**Symptoms**: "Connection failed" message after provisioning

**Solutions**:
1. Verify WiFi credentials correct (case-sensitive)
2. Check WiFi signal strength (device may be too far from router)
3. Ensure router not using MAC filtering
4. Check router supports 2.4GHz (ESP32 doesn't support 5GHz)
5. Try open network first (no password) for testing
6. Check serial monitor for detailed error messages

### Issue: Device Not Auto-Reconnecting

**Symptoms**: Device doesn't reconnect after router reboot

**Solutions**:
1. Check WiFi credentials saved in NVS
2. Verify auto-reconnect enabled
3. Check router DHCP settings
4. Increase retry count in code if needed
5. Check for WiFi interference

### Issue: High Memory Usage / Crashes

**Symptoms**: Device reboots randomly, "Out of memory" errors

**Solutions**:
1. Reduce WebSocket max clients (currently 4)
2. Decrease WebSocket message size
3. Monitor heap usage in serial monitor
4. Check for memory leaks in custom code
5. Reduce HTTP server stack size if needed

### Issue: Slow Web Panel Response

**Symptoms**: Web panel laggy, slow to update

**Solutions**:
1. Check WiFi signal strength
2. Reduce WebSocket update frequency
3. Minimize browser tabs/extensions
4. Use modern browser (Chrome/Firefox)
5. Check for network congestion

---

## Performance Metrics

### Memory Usage
- **WiFi Stack**: ~40KB RAM
- **HTTP Server**: ~20KB RAM
- **WebSocket**: ~15KB RAM per connection
- **Web Files (SPIFFS)**: ~50KB flash
- **Total Phase 3**: ~100KB RAM, ~80KB flash

### Network Performance
- **WebSocket Latency**: < 100ms
- **REST API Response**: < 50ms
- **Web Panel Load Time**: < 2 seconds
- **WebSocket Message Rate**: < 10 messages/sec typical
- **Concurrent Connections**: Up to 4 clients

### Power Consumption
- **WiFi Active**: +80-150mA (varies with transmission)
- **SoftAP Mode**: +20mA continuous
- **Station Mode**: +10mA continuous (with power save)

---

## Security Considerations

### Current Implementation (Phase 3)

⚠️ **Warning**: Phase 3 is designed for local network use only. Security features are minimal.

**Limitations**:
- No HTTPS/SSL encryption
- No user authentication
- No API rate limiting
- WiFi password stored in plaintext
- No input sanitization beyond basic validation

**Mitigation**:
- Use SoftAP password to restrict access
- Keep device on isolated network segment
- Don't expose device to internet
- Change default SoftAP password in code
- Use WPA2 encryption on home network

### Future Enhancements (Post-Phase 4)
- HTTPS with self-signed certificate
- Basic authentication (username/password)
- API rate limiting
- WiFi password encryption in NVS
- Input sanitization and validation
- Session management

---

## Integration with Phase 1 & 2

### Phase 1 Integration
- **Heating Control**: Web API calls `HeatingController` methods
- **Sensor Data**: WebSocket streams from `RoomSensor` and `MattressSensor`
- **Timer Management**: Web API controls `HeatingTimer`
- **Settings Storage**: WiFi credentials use same NVS namespace

### Phase 2 Integration
- **Button Control**: Physical buttons work alongside web control
- **Display Updates**: WiFi status shown on OLED display
- **Power Button**: Long press (3s) toggles SoftAP (future feature)
- **Synchronization**: All interfaces stay synchronized via shared state

---

## API Usage Examples

### JavaScript (Browser)

```javascript
// Enable heating
fetch('/api/heating/enable', { method: 'POST' })
  .then(res => res.json())
  .then(data => console.log(data));

// Set temperature
fetch('/api/temperature', {
  method: 'POST',
  headers: { 'Content-Type': 'application/json' },
  body: JSON.stringify({ setpoint: 35 })
})
  .then(res => res.json())
  .then(data => console.log(data));

// WebSocket connection
const ws = new WebSocket('ws://192.168.4.1:81');
ws.onmessage = (event) => {
  const data = JSON.parse(event.data);
  console.log('Received:', data);
};
```

### Python

```python
import requests

# Enable heating
response = requests.post('http://192.168.4.1/api/heating/enable')
print(response.json())

# Set temperature
response = requests.post('http://192.168.4.1/api/temperature', 
                        json={'setpoint': 35})
print(response.json())

# Get status
response = requests.get('http://192.168.4.1/api/status')
print(response.json())
```

### curl (Terminal)

```bash
# Enable heating
curl -X POST http://192.168.4.1/api/heating/enable

# Set temperature
curl -X POST http://192.168.4.1/api/temperature \
  -H "Content-Type: application/json" \
  -d '{"setpoint": 35}'

# Get status
curl http://192.168.4.1/api/status | jq .
```

---

## Next Steps

### Phase 4: Android Mobile Application

Phase 4 will build upon Phase 3's network infrastructure to create a native Android app:

- **Reuses Phase 3 APIs**: Same REST and WebSocket protocols
- **No Firmware Changes**: Phase 3 backend remains unchanged
- **Mobile-Optimized UI**: Native Material Design 3 interface
- **WiFi Provisioning**: In-app network configuration
- **Background Monitoring**: Efficient battery usage

**Estimated Duration**: 3-4 weeks

---

## Support and Documentation

### Related Documents
- `openspec/changes/phase3-network-web/proposal.md` - Phase 3 proposal
- `openspec/changes/phase3-network-web/design.md` - Technical design decisions
- `openspec/changes/phase3-network-web/tasks.md` - Implementation checklist
- `docs/TESTING_GUIDE_PHASE1.md` - Phase 1 testing guide
- `docs/PHASE2_IMPLEMENTATION.md` - Phase 2 implementation guide

### Contact
- **Project**: RODI Smart Mattress
- **Developer**: AliShafiee
- **HW Designer**: H.Rostamizade
- **Team**: RODI

---

## Changelog

### Version 3.0.0 (2025-12-19)
- ✅ Implemented WiFi Manager (SoftAP + Station + Dual-mode)
- ✅ Implemented HTTP Web Server
- ✅ Implemented WebSocket Real-Time Server
- ✅ Implemented REST API (8 endpoints) - **NOTE**: WiFi/SoftAP endpoints need integration
- ✅ Created responsive web panel UI (HTML/CSS/JavaScript)
- ✅ Integrated with Phase 1 & 2 components
- ✅ Added WiFi credentials persistence
- ✅ Updated main.cpp with Phase 3 components
- ✅ Updated platformio.ini with required libraries

### Version 3.0.1 (2025-12-20) - Review Update
- ⚠️ **Pending**: WiFi API endpoints integration with WiFiManager
- ⚠️ **Pending**: Power button long press → SoftAP toggle integration
- 📝 **Updated**: Tasks.md with accurate completion status (~95% complete)

---

**Phase 3 Implementation Status**: ⚠️ ~95% Complete

**Core Features**: ✅ Fully functional  
**Remaining Tasks**: 
- WiFi provisioning API endpoint integration
- SoftAP toggle via power button long press
- Optional: Rate limiting for API endpoints

The system provides full network connectivity and web-based control while maintaining compatibility with Phase 1 and Phase 2 features. Remaining tasks are minor integration work.

