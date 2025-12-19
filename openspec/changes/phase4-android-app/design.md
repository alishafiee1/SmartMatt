# Phase 4: Android Mobile Application - Design Document

## Context
Phase 4 creates a native Android mobile application for the RODI Smart Mattress, providing users with convenient smartphone access to device monitoring and control. The app reuses the Phase 3 REST API and WebSocket infrastructure, requiring no changes to ESP32 firmware. This completes the user experience trilogy: physical panel, web interface, and mobile app.

### Stakeholders
- End users wanting mobile convenience
- Android developers building the app
- Phase 3 infrastructure (REST API must remain stable)

### Constraints
- Android 7.0+ (API level 24+) compatibility for wide device support
- Must work offline (local network only, no cloud)
- Battery efficiency (WebSocket management)
- Reuse Phase 3 APIs without firmware changes
- Single device management (no multi-device complexity)

## Goals / Non-Goals

### Goals
- Native Android app with modern UI (Jetpack Compose)
- Direct WiFi connection (SoftAP and local network)
- Real-time monitoring using Phase 3 WebSocket
- Full device control using Phase 3 REST API
- WiFi provisioning within app
- Intuitive mobile-first UX
- Battery-efficient background behavior

### Non-Goals
- iOS application (separate future project)
- Multi-device management
- Cloud connectivity or remote internet access
- Historical data storage or trends
- Widgets, notifications, or background service
- Wearable (smartwatch) integration

## Technical Decisions

### 1. Development Platform: Kotlin + Jetpack Compose
**Decision**: Use Kotlin as language with Jetpack Compose for UI

**Rationale**:
- Kotlin is modern, concise, and Android-official language
- Jetpack Compose is declarative UI framework (modern replacement for XML)
- Compose simplifies UI updates from real-time WebSocket data
- Coroutines provide excellent async/await patterns for network
- Future-proof: Google is investing heavily in Compose

**Alternatives considered**:
- Java + XML layouts: Rejected as outdated, verbose
- Flutter: Rejected to avoid cross-platform overhead for Android-only
- React Native: Rejected due to JavaScript bridge performance

### 2. Network Architecture: OkHttp for REST and WebSocket
**Decision**: Use OkHttp library for both REST API calls and WebSocket connection

**Rationale**:
- Industry-standard HTTP client for Android
- Built-in WebSocket support
- Excellent connection pooling and reliability
- Kotlin-friendly APIs
- Mature and well-documented

**Implementation**:
```kotlin
// REST API
val client = OkHttpClient()
val request = Request.Builder()
    .url("http://192.168.4.1/api/temperature")
    .post(json)
    .build()
val response = client.newCall(request).execute()

// WebSocket
val wsRequest = Request.Builder()
    .url("ws://192.168.4.1/ws")
    .build()
val webSocket = client.newWebSocket(wsRequest, listener)
```

**Alternatives considered**:
- Retrofit: Good for REST but requires separate WebSocket library
- Ktor: Too heavyweight for simple local device communication

### 3. State Management: ViewModel + StateFlow
**Decision**: Use ViewModel with StateFlow for reactive state management

**Architecture**:
```kotlin
class DeviceViewModel : ViewModel() {
    private val _deviceState = MutableStateFlow<DeviceState>(DeviceState.Disconnected)
    val deviceState: StateFlow<DeviceState> = _deviceState.asStateFlow()
    
    private val _sensorData = MutableStateFlow<SensorData?>(null)
    val sensorData: StateFlow<SensorData?> = _sensorData.asStateFlow()
    
    fun connectDevice(ipAddress: String) { ... }
    fun setTemperature(temp: Float) { ... }
    fun enableHeating() { ... }
}
```

**Rationale**:
- ViewModel survives configuration changes (screen rotation)
- StateFlow integrates perfectly with Compose (automatic UI updates)
- Clear separation: ViewModel (logic) vs. Composable (UI)
- Testable architecture

**Alternatives considered**:
- LiveData: Older API, StateFlow is more Kotlin-idiomatic
- Raw state in Activity/Fragment: Rejected due to lifecycle issues

### 4. WebSocket Lifecycle Management
**Decision**: Connect WebSocket on app foreground, disconnect on background

**Lifecycle**:
```
App Launch → Connect WebSocket
App Foreground → Ensure WebSocket connected
App Background → Disconnect WebSocket (save battery)
App Killed → Clean disconnect
```

**Rationale**:
- Battery efficiency: WebSocket is battery-intensive if always on
- No background monitoring needed (user controls device when app open)
- Reconnect on foreground provides fresh data
- Aligns with Android best practices

**Reconnection Strategy**:
- Immediate retry on unexpected disconnect (while in foreground)
- Exponential backoff: 1s, 2s, 4s, 8s, max 30s
- Reset backoff on successful connection

### 5. Device Connection Methods
**Decision**: Support both SoftAP and local network with manual IP entry

**Connection Flows**:

**A) SoftAP Direct Connection**:
```
1. User connects phone to "RODI_XXXX" WiFi
2. App detects SoftAP connection (SSID pattern or default IP)
3. Auto-connect to 192.168.4.1
4. User controls device
```

**B) Local Network Connection**:
```
1. Device provisioned to home WiFi (via app or web panel)
2. User enters device IP manually (or saved from previous session)
3. App connects to entered IP
4. User controls device
```

**Auto-Discovery (Optional Enhancement)**:
- Not implemented in Phase 4 MVP
- Future: mDNS/Bonjour scanning for "rodi.local"
- Future: UDP broadcast discovery on local subnet

**Rationale**:
- SoftAP flow simple and foolproof
- Manual IP entry works for tech-savvy users
- Auto-discovery adds complexity without major benefit initially

### 6. UI Design: Material Design 3 with IoT Theme
**Decision**: Use Material Design 3 with custom color scheme matching project branding

**Color Scheme**:
```kotlin
Primary: Blue (#2196F3) - IoT standard
Secondary: Orange (#FF5722) - Heating accent
Background: Light Gray (#F5F5F5)
Surface: White (#FFFFFF)
Error: Red (#F44336)
Success: Green (#4CAF50)
```

**Layout Structure**:
```
┌─────────────────────────────────┐
│  ← RODI Smart Mattress      ⚙  │  Top Bar
├─────────────────────────────────┤
│  ●  ●  ●                        │  Status Chips
│  [Heating] [WiFi: LAN] [●]      │
├─────────────────────────────────┤
│                                 │
│      25.5°C     65%             │  Large Sensor Display
│    Room Conditions              │
│                                 │
├─────────────────────────────────┤
│  Temperature Setpoint           │
│                                 │
│      [  -  ]  35°C  [  +  ]    │  Control Card
│                                 │
├─────────────────────────────────┤
│  Timer                          │
│                                 │
│   [  -  ]  2:30:00  [  +  ]    │  Control Card
│                                 │
├─────────────────────────────────┤
│                                 │
│    [ 🔥 Power ON ]              │  Large Action Button
│                                 │
└─────────────────────────────────┘
```

**Rationale**:
- Familiar Android patterns
- Material 3 provides modern, polished look
- Card-based layout groups related controls
- Large touch targets for mobile (minimum 48dp)

### 7. JSON Protocol Reuse from Phase 3
**Decision**: Use identical JSON structures as Phase 3 web panel

**Data Models**:
```kotlin
@Serializable
data class SnapshotMessage(
    val type: String = "snapshot",
    val data: DeviceData,
    val timestamp: Long
)

@Serializable
data class UpdateMessage(
    val type: String = "update",
    val data: Map<String, JsonElement>,
    val timestamp: Long
)

@Serializable
data class DeviceData(
    val room_temp: Float,
    val room_humidity: Int,
    val mattress_temp: Float,
    val setpoint: Int,
    val heating_on: Boolean,
    val timer_remaining_sec: Int,
    val timer_duration_min: Int,
    val wifi_mode: String
)
```

**Rationale**:
- No ESP32 firmware changes needed
- Proven protocol from Phase 3 testing
- Easy to maintain consistency across web and app

## App Architecture

```
┌─────────────────────────────────────────────┐
│              MainActivity                   │
│         (Jetpack Compose UI)                │
└─────────────┬───────────────────────────────┘
              │
              ↓
┌─────────────────────────────────────────────┐
│          DeviceViewModel                    │
│   (State Management + Business Logic)       │
└──────┬──────────────────────┬───────────────┘
       │                      │
       ↓                      ↓
┌──────────────┐      ┌──────────────────┐
│  RestApiRepo │      │  WebSocketRepo   │
│  (OkHttp)    │      │  (OkHttp WS)     │
└──────────────┘      └──────────────────┘
       │                      │
       └──────────┬───────────┘
                  │
                  ↓
         ┌────────────────┐
         │  ESP32 Device  │
         │  (Phase 3 API) │
         └────────────────┘
```

## Data Flow

### User Command Flow
```
User Clicks "+1°C"
     ↓
Composable UI Handler
     ↓
ViewModel.setTemperature(36)
     ↓
RestApiRepo.postTemperature(36)
     ↓
OkHttp POST /api/temperature
     ↓
ESP32 REST API Handler (Phase 3)
     ↓
HTTP 200 Response
     ↓
ViewModel updates StateFlow
     ↓
Compose UI auto-updates (reactive)
     ↓
WebSocket push confirms change
     ↓
UI shows confirmation
```

### Real-Time Sensor Update Flow
```
ESP32 Sensor Reading Changes
     ↓
WebSocket Push (Phase 3)
     ↓
OkHttp WebSocket Listener
     ↓
WebSocketRepo.onMessage()
     ↓
Parse JSON (type="update")
     ↓
ViewModel updates sensorData StateFlow
     ↓
Compose UI observes StateFlow
     ↓
UI recomposes with new values
```

## Screen Specifications

### 1. Main Screen (DeviceControlScreen)
- **Purpose**: Primary control and monitoring
- **Components**:
  - Top app bar with device name and settings icon
  - Status chips row (heating, WiFi, connection)
  - Room conditions card (temp/humidity, large)
  - Temperature control card (setpoint with +/- buttons)
  - Timer control card (countdown with +/- buttons)
  - Power toggle button (prominent, full-width)
- **State**: DeviceState (Connected/Disconnected/Connecting)

### 2. Settings Screen
- **Purpose**: Configuration and device management
- **Components**:
  - Device IP address input
  - WiFi provisioning option
  - About section (app version, device info)
  - Clear settings button
- **Navigation**: Accessed via settings icon in top bar

### 3. WiFi Provisioning Dialog/Sheet
- **Purpose**: Configure device WiFi from app
- **Components**:
  - SSID text input
  - Password text input (masked)
  - Connect button
  - Progress indicator
  - Success/error feedback
- **Trigger**: Button in settings or first-time setup

### 4. Connection Dialog (optional)
- **Purpose**: Manual device connection
- **Components**:
  - IP address input
  - Connect button
  - SoftAP quick connect button (192.168.4.1)
- **Trigger**: When no saved IP or connection fails

## Permissions Required

```xml
<uses-permission android:name="android.permission.INTERNET" />
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
<uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
```

No dangerous permissions needed (all are normal permissions).

## Performance Targets

### Network Performance
- REST API call: <100ms latency (local network)
- WebSocket connection: <200ms to establish
- WebSocket message handling: <50ms from receive to UI update

### App Performance
- App launch to ready: <2 seconds
- UI responsiveness: 60 FPS minimum
- Memory usage: <100MB typical

### Battery Usage
- Foreground with WebSocket: <5% per hour
- Background: 0% (WebSocket disconnected)

## Error Handling

### Network Errors
- **Device unreachable**: Show "Cannot connect to device" with retry button
- **Timeout**: Show loading indicator, timeout after 10 seconds
- **Invalid response**: Log error, show "Communication error"

### API Errors
- **400 Bad Request**: Show specific error message (e.g., "Temperature out of range")
- **500 Server Error**: Show "Device error, please try again"
- **Network disconnect during call**: Queue retry or show failure

### WebSocket Errors
- **Connection failed**: Auto-retry with backoff
- **Unexpected disconnect**: Show "Reconnecting..." and auto-retry
- **Persistent failure**: Show "Offline" indicator with manual retry option

## Testing Strategy

### Unit Tests
- ViewModel logic (temperature control, timer control)
- JSON parsing (snapshot, update messages)
- API request builders
- State transitions

### Integration Tests
- REST API calls to real device
- WebSocket connection and message handling
- WiFi provisioning flow
- Error scenarios (device offline, network loss)

### UI Tests
- Compose UI rendering
- Button click interactions
- State-driven UI updates
- Screen navigation

### Manual Tests
- SoftAP connection flow
- Local network connection flow
- All control functions (temp, timer, power)
- Real-time updates accuracy
- App backgrounding/foregrounding
- Device disconnect while app running

## Security Considerations

### Current Phase
- **No encryption**: HTTP and unencrypted WebSocket
- **No authentication**: Open access to device
- **Local network only**: Not exposed to internet

**Rationale**: Same as Phase 3 - simplicity and resource constraints

### Future Enhancements
- HTTPS support (if ESP32 gains TLS capability)
- Basic authentication (password protection)
- Certificate pinning for device verification

## Migration Plan

### Phase 3 → Phase 4 Integration
- **No firmware changes**: App uses existing Phase 3 APIs
- **Parallel development**: App developed separately from firmware
- **Testing against Phase 3**: Requires working Phase 3 device

### Deployment
- APK can be distributed directly or via Play Store
- No device firmware update needed
- App and firmware versions independent

## Risks / Trade-offs

### Risk: Android Version Fragmentation
- **Impact**: Older devices may lack features
- **Mitigation**: Target API 24+ covers 95%+ of devices
- **Trade-off**: Drop support for Android 6 and below

### Risk: WiFi Connection Complexity
- **Impact**: Users may struggle with SoftAP vs. local network
- **Mitigation**: Clear onboarding instructions, auto-detect where possible
- **Trade-off**: Simplicity vs. automatic discovery

### Risk: Battery Drain
- **Impact**: WebSocket connection drains battery
- **Mitigation**: Disconnect in background, efficient polling
- **Trade-off**: Real-time updates vs. battery life

### Risk: Network Discovery
- **Impact**: Users may not know device IP
- **Mitigation**: Manual entry, save for future, SoftAP default
- **Trade-off**: Convenience vs. implementation complexity

## Open Questions

1. **Q**: Should app support multiple devices?
   **A**: No in Phase 4 - single device only, simplifies UX and state management

2. **Q**: Should app cache historical data?
   **A**: No in Phase 4 - live data only, no local storage

3. **Q**: Should app have persistent notification when connected?
   **A**: No - app is foreground-only, no background service

4. **Q**: Should app support portrait and landscape?
   **A**: Yes - Compose handles orientation automatically

5. **Q**: Should app work without WiFi (Bluetooth)?
   **A**: No - WiFi only, Bluetooth would require firmware changes

## Dependencies

### Software Dependencies
- Android SDK (API 24+)
- Kotlin 1.9+
- Jetpack Compose 1.5+
- OkHttp 4.x
- Kotlinx Serialization or Gson
- Coroutines
- ViewModel & Lifecycle libraries

### Hardware Dependencies
- Android smartphone or tablet
- WiFi connectivity
- Phase 3 RODI device on same network or SoftAP

### External Dependencies
- Phase 3 REST API endpoints
- Phase 3 WebSocket server
- Device reachability (network connectivity)

## Future Enhancements (Post-Phase 4)

1. **Device Auto-Discovery**: mDNS/Bonjour support
2. **Multiple Device Support**: Manage multiple mattresses
3. **History/Trends**: Store and visualize temperature history
4. **Scheduling**: Preset temperature/timer schedules
5. **Widgets**: Home screen quick controls
6. **Notifications**: Push alerts for timer expiration
7. **iOS App**: Port to iOS using similar architecture
8. **Dark Mode**: Themed UI support
9. **Accessibility**: TalkBack and large font support
10. **Localization**: Multi-language support

