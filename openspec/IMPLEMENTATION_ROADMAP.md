# RODI Smart Mattress - Implementation Roadmap

## Overview
This document provides an overview of the complete OpenSpec proposal structure for the RODI Smart Mattress project, organized into 4 sequential development phases.

## Proposal Status
✅ All phases designed and validated with `openspec --strict`

## Phase Summary

### Phase 1: Core Hardware Control
**Status**: Ready for implementation  
**Duration Estimate**: 2-3 weeks  
**Tasks**: 37 tasks  
**Location**: `openspec/changes/phase1-core-hardware/`

**Purpose**: Establish foundational hardware control system with temperature monitoring, heating control, timer management, and persistent settings storage.

**Key Capabilities**:
- Temperature monitoring (DHT11 + DS18B20)
- Heating element control with hysteresis
- Software timer with automatic shutoff
- Settings persistence (NVS)
- Safety features (watchdog, sensor validation)

**Deliverables**:
- Working heating control system
- Temperature range: 20-45°C
- Timer range: 15 min - 8 hours
- Safe boot state (heating OFF)

---

### Phase 2: Device Panel Interface
**Status**: Ready for implementation (requires Phase 1)  
**Duration Estimate**: 2-3 weeks  
**Tasks**: 70 tasks  
**Location**: `openspec/changes/phase2-device-panel/`

**Purpose**: Add physical user interface with buttons and OLED display for direct device interaction.

**Key Capabilities**:
- Physical button interface (5 buttons with debouncing)
- OLED display (SSD1306, 128x64)
- Boot sequence animation (Rodiset.ir → RODI logo)
- Real-time display updates
- Display animations (zoom on value change)
- Status icons (WiFi, heating element)

**Deliverables**:
- Functional button control panel
- Animated OLED interface
- 3-column display layout
- Intuitive button patterns (short/long press, hold-repeat)

---

### Phase 3: WiFi Network and Web Panel
**Status**: Ready for implementation (requires Phase 1 & 2)  
**Duration Estimate**: 4-5 weeks  
**Tasks**: 114 tasks  
**Location**: `openspec/changes/phase3-network-web/`

**Purpose**: Enable network connectivity and browser-based control interface.

**Key Capabilities**:
- WiFi SoftAP mode (direct device access)
- WiFi Station mode (home network integration)
- Dual-mode operation (SoftAP + Station)
- WebSocket server (real-time data streaming)
- REST API (command & control)
- Responsive web panel (HTML/CSS/JavaScript)
- WiFi provisioning

**Deliverables**:
- Working WiFi access (both modes)
- Functional web panel accessible via browser
- Real-time sensor updates without page refresh
- Standardized REST API for all controls
- WiFi credentials storage and management

---

### Phase 4: Android Mobile Application
**Status**: Ready for implementation (requires Phase 3)  
**Duration Estimate**: 3-4 weeks  
**Tasks**: 135 tasks  
**Location**: `openspec/changes/phase4-android-app/`

**Purpose**: Provide native Android app for convenient mobile control.

**Key Capabilities**:
- Native Android app (Kotlin + Jetpack Compose)
- Reuses Phase 3 REST API and WebSocket (no firmware changes)
- Direct WiFi connection (SoftAP and local network)
- Real-time monitoring
- WiFi provisioning within app
- Modern Material Design 3 UI

**Deliverables**:
- Working Android APK
- Support for Android 7.0+ (API 24+)
- Mobile-optimized UI
- Battery-efficient background behavior
- Full device control from smartphone

---

## Total Project Scope

| Metric | Count |
|--------|-------|
| **Total Phases** | 4 |
| **Total Tasks** | 356 tasks |
| **Total Capabilities** | 17 capabilities |
| **Estimated Duration** | 11-15 weeks |

---

## Capability Breakdown

### Phase 1 Capabilities (4)
1. `heating-control` - Temperature-based heating management
2. `sensor-monitoring` - DHT11 + DS18B20 integration
3. `timer-management` - Countdown timer for auto-shutoff
4. `settings-storage` - NVS persistence

### Phase 2 Capabilities (3)
5. `button-interface` - Physical button control
6. `display-interface` - OLED display management
7. `boot-sequence` - Branded boot animation

### Phase 3 Capabilities (5)
8. `wifi-softap` - Access Point mode
9. `wifi-station` - Client mode and provisioning
10. `websocket-realtime` - Real-time data streaming
11. `rest-api` - Command & control API
12. `web-panel` - Browser-based UI

### Phase 4 Capabilities (2)
13. `android-app` - Native mobile application
14. `app-network` - Mobile app network integration

---

## Dependencies

```
Phase 1 (Core Hardware Control)
    ↓
Phase 2 (Device Panel Interface)
    ↓
Phase 3 (WiFi & Web Panel)
    ↓
Phase 4 (Android App)
```

**Note**: Each phase depends on successful completion of previous phases.

---

## Implementation Strategy

### Sequential Development
Phases must be completed in order due to dependencies:
1. Phase 1 provides core control APIs
2. Phase 2 adds local UI using Phase 1 APIs
3. Phase 3 adds network layer using Phase 1 APIs
4. Phase 4 reuses Phase 3 network protocols

### Testing Approach
- **Unit Testing**: Each module tested independently
- **Integration Testing**: Phase integration verified before moving forward
- **System Testing**: Complete system tested after each phase
- **Regression Testing**: Previous phases retested when new phase added

### Quality Gates
Each phase must pass validation before next phase begins:
- ✅ All tasks completed (`tasks.md` checklist)
- ✅ `openspec validate --strict` passes
- ✅ Manual testing completed
- ✅ Safety features verified
- ✅ Documentation updated

---

## Technology Stack

### Hardware
- **Microcontroller**: ESP32
- **Sensors**: DHT11 (room), DS18B20 (mattress)
- **Display**: SSD1306 OLED (128x64, I2C)
- **Buttons**: 5x tactile switches
- **Control**: Relay for heating element

### Software - Firmware
- **Framework**: ESP-IDF
- **Language**: C++ (feature-based architecture)
- **Communication**: WiFi, WebSocket, HTTP REST
- **Storage**: ESP32 Preferences (NVS)

### Software - Web
- **Frontend**: HTML5, CSS3, Vanilla JavaScript
- **Backend**: ESP32 HTTP server (built-in)
- **Protocol**: REST API + WebSocket

### Software - Mobile
- **Platform**: Android 7.0+ (API 24+)
- **Language**: Kotlin
- **UI Framework**: Jetpack Compose
- **Network**: OkHttp (REST + WebSocket)

---

## Key Design Decisions

1. **Feature-Based Architecture**: Code organized by feature for maintainability
2. **Safety-First Design**: Multiple fail-safe mechanisms (watchdog, sensor validation)
3. **Offline-First**: Works without internet, local network only
4. **Standardized APIs**: REST + WebSocket for extensibility
5. **Progressive Enhancement**: Each phase adds functionality without breaking previous phases
6. **Battery Efficiency**: Mobile app disconnects in background
7. **Dual-Mode WiFi**: Both SoftAP (direct) and Station (home network) supported

---

## Next Steps

### To Begin Implementation:

1. **Review Phase 1 Documents**:
   - `openspec/changes/phase1-core-hardware/proposal.md`
   - `openspec/changes/phase1-core-hardware/design.md`
   - `openspec/changes/phase1-core-hardware/tasks.md`

2. **Setup Development Environment**:
   - Install ESP-IDF
   - Configure hardware connections
   - Setup version control

3. **Start Phase 1 Implementation**:
   - Follow `tasks.md` checklist sequentially
   - Refer to `design.md` for technical decisions
   - Validate each capability with `openspec validate --strict`

4. **Track Progress**:
   - Update task checkboxes in `tasks.md`
   - Test each completed task
   - Document any deviations from design

5. **Approval Gate**:
   - Do not start implementation until proposals are reviewed and approved by team

---

## Validation Status

All proposals have been validated with OpenSpec:

```bash
$ openspec list
Changes:
  phase1-core-hardware     0/37 tasks    ✅ Valid
  phase2-device-panel      0/70 tasks    ✅ Valid
  phase3-network-web       0/114 tasks   ✅ Valid
  phase4-android-app       0/135 tasks   ✅ Valid
```

---

## Documentation Index

### Global Documents
- `openspec/project.md` - Project context and conventions
- `openspec/AGENTS.md` - AI assistant instructions
- `openspec/IMPLEMENTATION_ROADMAP.md` - This document

### Phase 1 Documents
- `proposal.md` - Why, what, impact
- `design.md` - Technical decisions (68 pages)
- `tasks.md` - Implementation checklist (37 tasks)
- `specs/heating-control/spec.md` - Heating control requirements
- `specs/sensor-monitoring/spec.md` - Sensor requirements
- `specs/timer-management/spec.md` - Timer requirements
- `specs/settings-storage/spec.md` - Storage requirements

### Phase 2 Documents
- `proposal.md` - Why, what, impact
- `design.md` - Technical decisions (58 pages)
- `tasks.md` - Implementation checklist (70 tasks)
- `specs/button-interface/spec.md` - Button requirements
- `specs/display-interface/spec.md` - Display requirements
- `specs/boot-sequence/spec.md` - Boot requirements

### Phase 3 Documents
- `proposal.md` - Why, what, impact
- `design.md` - Technical decisions (87 pages)
- `tasks.md` - Implementation checklist (114 tasks)
- `specs/wifi-softap/spec.md` - SoftAP requirements
- `specs/wifi-station/spec.md` - Station mode requirements
- `specs/websocket-realtime/spec.md` - WebSocket requirements
- `specs/rest-api/spec.md` - REST API requirements
- `specs/web-panel/spec.md` - Web UI requirements

### Phase 4 Documents
- `proposal.md` - Why, what, impact
- `design.md` - Technical decisions (72 pages)
- `tasks.md` - Implementation checklist (135 tasks)
- `specs/android-app/spec.md` - Android app requirements
- `specs/app-network/spec.md` - App networking requirements

---

## Contact & Support

**Project**: RODI Smart Mattress  
**Date**: 1404/09/27 (2025/12/19)  
**Proposal Designer**: AliShafiee  
**HW Designer**: H.Rostamizade  
**SW Developer**: AliShafiee  
**Team**: RODI

---

## Version History

| Version | Date | Changes |
|---------|------|---------|
| 1.0 | 2025-12-19 | Initial roadmap created with all 4 phases |

---

**Ready to start implementation!** 🚀

All proposals are designed, validated, and ready for sequential development following the OpenSpec workflow.

