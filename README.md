# RODI Smart Mattress

<div align="center">

![Version](https://img.shields.io/badge/version-3.0.0-blue.svg)
![Platform](https://img.shields.io/badge/platform-ESP32-green.svg)
![Framework](https://img.shields.io/badge/framework-Arduino-orange.svg)
![Status](https://img.shields.io/badge/status-Phase%203%20Complete-success.svg)

**An intelligent IoT mattress heating system with temperature control, timer management, and web-based monitoring**

[Features](#features) • [Hardware](#hardware) • [Getting Started](#getting-started) • [Documentation](#documentation) • [Team](#team)

</div>

---

## 📋 Overview

RODI Smart Mattress is a comprehensive IoT solution for intelligent mattress temperature control. The system provides precise heating management with multiple control interfaces: physical buttons, OLED display, and web panel.

### Key Capabilities

- 🌡️ **Dual Temperature Monitoring**: Room (DHT11) and mattress (DS18B20) sensors
- 🔥 **Smart Heating Control**: Hysteresis-based temperature regulation (20-45°C)
- ⏱️ **Timer Management**: Auto-shutoff timer (15 minutes - 8 hours)
- 📱 **Web Control Panel**: Responsive web interface with real-time updates
- 📶 **WiFi Connectivity**: SoftAP and Station modes for flexible access
- 🖥️ **OLED Display**: Animated UI with status icons
- 🔘 **Physical Buttons**: Direct device control with debouncing
- 💾 **Persistent Settings**: NVS storage for configuration
- 🛡️ **Safety Features**: Watchdog timer, sensor validation, safe boot state

---

## 🚀 Features

### Phase 1: Core Hardware Control ✅
- Temperature monitoring (DHT11 + DS18B20)
- Heating element control with hysteresis
- Software timer with automatic shutoff
- Settings persistence (NVS)
- Safety features (watchdog, sensor validation)

### Phase 2: Device Panel Interface ✅
- Physical button interface (5 buttons with debouncing)
- OLED display (SSD1306, 128x64)
- Boot sequence animation (Rodiset.ir → RODI logo)
- Real-time display updates
- Display animations (zoom on value change)
- Status icons (WiFi, heating element)

### Phase 3: WiFi Network and Web Panel ✅
- WiFi SoftAP mode (direct device access)
- WiFi Station mode (home network integration)
- Dual-mode operation (SoftAP + Station simultaneously)
- HTTP web server (port 80)
- WebSocket server (real-time data streaming)
- REST API (8 endpoints for device control)
- Responsive web panel (HTML/CSS/JavaScript)
- WiFi provisioning

### Phase 4: Android Mobile Application 🔜
- Native Android app (Kotlin + Jetpack Compose)
- Reuses Phase 3 REST API and WebSocket
- Mobile-optimized UI
- WiFi provisioning within app
- Material Design 3

---

## 🔧 Hardware

### Required Components

| Component | Model | Purpose |
|-----------|-------|---------|
| Microcontroller | ESP32 | Main controller with WiFi |
| Room Sensor | DHT11 | Temperature & humidity monitoring |
| Mattress Sensor | DS18B20 | Internal mattress temperature |
| Display | SSD1306 OLED (128x64) | Status display |
| Buttons | 5x Tactile switches | Physical control interface |
| Relay | 5V Relay module | Heating element control |
| Heating Element | 12V/24V heating pad | Mattress heating |
| Power Supply | 5V 2A + 12V/24V | System power |

### Pin Assignments

```cpp
// Temperature Sensors
DHT11:     GPIO4
DS18B20:   GPIO5

// Heating Control
Relay:     GPIO2

// I2C Display
SDA:       GPIO21
SCL:       GPIO22

// Physical Buttons
Temp Up:   GPIO13
Temp Down: GPIO12
Timer Up:  GPIO14
Timer Down: GPIO27
Power:     GPIO26
```

---

## 📦 Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) (recommended) or Arduino IDE
- ESP32 development board
- USB cable for programming
- Hardware components (see [Hardware](#hardware))

### Installation

1. **Clone the repository**

```bash
git clone https://github.com/your-repo/rodi-smart-mattress.git
cd rodi-smart-mattress/SmartMatt
```

2. **Install dependencies**

```bash
pio lib install
```

3. **Build firmware**

```bash
pio run
```

4. **Upload firmware**

```bash
pio run --target upload
```

5. **Upload web panel files (SPIFFS)**

```bash
pio run --target uploadfs
```

6. **Monitor serial output**

```bash
pio device monitor
```

### First-Time Setup

1. **Power on device** - Wait for boot sequence (~15 seconds)
2. **Connect to WiFi** - Look for `RODI_XXXX` network (password: `rodi1234`)
3. **Access web panel** - Open browser to `http://192.168.4.1`
4. **Configure WiFi** (optional) - Enter home WiFi credentials in web panel

---

## 🌐 Web Panel

### Access URLs

- **Direct (SoftAP)**: `http://192.168.4.1`
- **Home Network**: `http://<device-ip>` (check serial monitor)

### Features

- Real-time temperature and humidity display
- Temperature setpoint control (20-45°C)
- Timer control (15 min - 8 hours)
- Heating on/off toggle
- WiFi provisioning form
- Responsive design (mobile, tablet, desktop)
- Persian language interface (RTL)

### REST API Endpoints

```http
POST /api/heating/enable          # Enable heating
POST /api/heating/disable         # Disable heating
POST /api/temperature             # Set temperature setpoint
POST /api/timer                   # Set timer duration
GET  /api/status                  # Get system status
POST /api/wifi                    # WiFi provisioning
GET  /api/wifi                    # Get WiFi status
POST /api/softap                  # Toggle SoftAP
```

### WebSocket

```javascript
// Connect to WebSocket
const ws = new WebSocket('ws://192.168.4.1:81');

// Receive real-time updates
ws.onmessage = (event) => {
  const data = JSON.parse(event.data);
  console.log('Update:', data);
};
```

---

## 📚 Documentation

### Implementation Guides
- [Phase 1: Core Hardware Control](docs/TESTING_GUIDE_PHASE1.md)
- [Phase 2: Device Panel Interface](docs/PHASE2_IMPLEMENTATION.md)
- [Phase 3: WiFi Network and Web Panel](docs/PHASE3_IMPLEMENTATION.md)

### OpenSpec Documentation
- [Project Overview](openspec/project.md)
- [Implementation Roadmap](openspec/IMPLEMENTATION_ROADMAP.md)
- [Phase 1 Proposal](openspec/changes/phase1-core-hardware/proposal.md)
- [Phase 2 Proposal](openspec/changes/phase2-device-panel/proposal.md)
- [Phase 3 Proposal](openspec/changes/phase3-network-web/proposal.md)
- [Phase 4 Proposal](openspec/changes/phase4-android-app/proposal.md)

### API Reference
- [REST API Documentation](docs/PHASE3_IMPLEMENTATION.md#rest-api)
- [WebSocket Protocol](docs/PHASE3_IMPLEMENTATION.md#websocket)

### Troubleshooting
- [Common Issues](docs/TROUBLESHOOTING.md)
- [Phase 3 Troubleshooting](docs/PHASE3_IMPLEMENTATION.md#troubleshooting)

---

## 🏗️ Project Structure

```
SmartMatt/
├── src/
│   ├── main.cpp                   # Main application
│   ├── sensors/                   # Temperature sensors
│   │   ├── RoomSensor.*           # DHT11 room sensor
│   │   └── MattressSensor.*       # DS18B20 mattress sensor
│   ├── heating/                   # Heating control
│   │   └── HeatingController.*    # Temperature-based heating
│   ├── timer/                     # Timer management
│   │   └── HeatingTimer.*         # Countdown timer
│   ├── storage/                   # Settings persistence
│   │   └── SettingsStorage.*      # NVS storage
│   ├── safety/                    # Safety features
│   │   └── SafetyMonitor.*        # Watchdog and validation
│   ├── buttons/                   # Physical buttons
│   │   ├── ButtonDebounce.*       # Button debouncing
│   │   └── ButtonManager.*        # Button event handling
│   ├── display/                   # OLED display
│   │   └── DisplayManager.*       # Display rendering
│   ├── network/                   # WiFi connectivity
│   │   └── WiFiManager.*          # SoftAP + Station modes
│   └── server/                    # Web server
│       └── WebServer.*            # HTTP + WebSocket
├── data/                          # Web panel files (SPIFFS)
│   ├── index.html                 # Web UI
│   ├── style.css                  # Styles
│   └── app.js                     # JavaScript
├── include/
│   └── config/
│       └── RodiConfig.h           # Configuration constants
├── docs/                          # Documentation
├── openspec/                      # OpenSpec specifications
└── platformio.ini                 # Build configuration
```

---

## 🔒 Security

⚠️ **Important**: Phase 3 is designed for local network use only.

### Current Limitations
- No HTTPS/SSL encryption
- No user authentication
- No API rate limiting
- WiFi password stored in plaintext

### Recommendations
- Use SoftAP password to restrict access
- Keep device on isolated network segment
- Don't expose device to internet
- Change default SoftAP password in code

### Future Enhancements
- HTTPS with self-signed certificate
- Basic authentication
- API rate limiting
- WiFi password encryption

---

## 🧪 Testing

### Quick Test

```bash
# Enable heating
curl -X POST http://192.168.4.1/api/heating/enable

# Set temperature to 30°C
curl -X POST http://192.168.4.1/api/temperature \
  -H "Content-Type: application/json" \
  -d '{"setpoint": 30}'

# Get status
curl http://192.168.4.1/api/status
```

### Comprehensive Testing

See [Phase 3 Testing Guide](docs/PHASE3_IMPLEMENTATION.md#testing-guide) for detailed test procedures.

---

## 📊 Performance

### Memory Usage
- **Phase 1**: ~80KB RAM
- **Phase 2**: ~40KB RAM (display + buttons)
- **Phase 3**: ~100KB RAM (WiFi + web server)
- **Total**: ~220KB RAM (ESP32 has ~320KB available)

### Network Performance
- WebSocket latency: < 100ms
- REST API response: < 50ms
- Web panel load time: < 2 seconds
- Concurrent connections: Up to 4 clients

### Power Consumption
- Base system: ~150mA
- WiFi active: +80-150mA
- Heating element: Depends on heating pad (typically 1-2A)

---

## 🛣️ Roadmap

- [x] **Phase 1**: Core Hardware Control (37 tasks) ✅
- [x] **Phase 2**: Device Panel Interface (70 tasks) ✅
- [x] **Phase 3**: WiFi Network and Web Panel (114 tasks) ✅
- [ ] **Phase 4**: Android Mobile Application (135 tasks) 🔜

**Total**: 356 tasks across 4 phases

---

## 👥 Team

**Project**: RODI Smart Mattress  
**Team**: RODI  

**Roles**:
- **Software Developer**: AliShafiee
- **Hardware Designer**: H.Rostamizade

---

## 📄 License

This project is proprietary software developed by Team RODI.

---

## 🙏 Acknowledgments

- ESP-IDF framework by Espressif Systems
- Arduino ecosystem
- OpenSpec specification framework
- PlatformIO development platform

---

## 📞 Contact

For questions, issues, or contributions:

- **GitHub Issues**: [Create an issue](https://github.com/your-repo/rodi-smart-mattress/issues)
- **Email**: [your-email@example.com]
- **Website**: [Rodiset.ir](http://rodiset.ir)

---

<div align="center">

**Made with ❤️ by Team RODI**

[⬆ Back to Top](#rodi-smart-mattress)

</div>

