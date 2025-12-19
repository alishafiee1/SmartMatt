# Phase 4 Implementation Tasks

## 1. Android Project Setup
- [ ] 1.1 Create new Android Studio project with Kotlin
- [ ] 1.2 Configure build.gradle with required dependencies
- [ ] 1.3 Setup Jetpack Compose for UI
- [ ] 1.4 Configure minimum SDK (API 24 / Android 7.0)
- [ ] 1.5 Setup version control and project structure

## 2. Project Dependencies
- [ ] 2.1 Add OkHttp for HTTP/REST API calls
- [ ] 2.2 Add OkHttp WebSocket support for real-time data
- [ ] 2.3 Add Gson or Kotlinx Serialization for JSON parsing
- [ ] 2.4 Add Jetpack Compose UI dependencies
- [ ] 2.5 Add Kotlin Coroutines for async operations
- [ ] 2.6 Add ViewModel and LiveData for state management

## 3. Network Layer - REST API Client
- [ ] 3.1 Create REST API service interface
- [ ] 3.2 Implement POST /api/heating/enable endpoint call
- [ ] 3.3 Implement POST /api/heating/disable endpoint call
- [ ] 3.4 Implement POST /api/temperature endpoint call
- [ ] 3.5 Implement POST /api/timer endpoint call
- [ ] 3.6 Implement GET /api/status endpoint call
- [ ] 3.7 Implement POST /api/wifi provisioning endpoint call
- [ ] 3.8 Add error handling for network requests
- [ ] 3.9 Test all API calls with Phase 3 device

## 4. Network Layer - WebSocket Client
- [ ] 4.1 Implement WebSocket connection management
- [ ] 4.2 Handle WebSocket upgrade and connection
- [ ] 4.3 Parse incoming WebSocket JSON messages
- [ ] 4.4 Implement snapshot message handling (type="snapshot")
- [ ] 4.5 Implement update message handling (type="update")
- [ ] 4.6 Add WebSocket reconnection logic with backoff
- [ ] 4.7 Handle WebSocket errors and disconnections
- [ ] 4.8 Test real-time updates from device

## 5. Data Models
- [ ] 5.1 Create DeviceState data class
- [ ] 5.2 Create SensorData data class
- [ ] 5.3 Create HeatingStatus data class
- [ ] 5.4 Create WiFiStatus data class
- [ ] 5.5 Create API request/response models
- [ ] 5.6 Add JSON serialization annotations

## 6. State Management - ViewModel
- [ ] 6.1 Create MainViewModel for app state
- [ ] 6.2 Implement LiveData/StateFlow for sensor data
- [ ] 6.3 Implement LiveData for heating status
- [ ] 6.4 Implement LiveData for WiFi connection status
- [ ] 6.5 Add connection state management
- [ ] 6.6 Implement error state handling

## 7. Device Discovery
- [ ] 7.1 Implement local network device scanning (optional)
- [ ] 7.2 Manual IP address entry option
- [ ] 7.3 SoftAP connection detection (default 192.168.4.1)
- [ ] 7.4 Save device IP in SharedPreferences
- [ ] 7.5 Test device discovery on local network

## 8. Main UI Screen - Compose
- [ ] 8.1 Create main screen layout (Compose)
- [ ] 8.2 Implement top app bar with device name
- [ ] 8.3 Design status card (heating, WiFi, connection)
- [ ] 8.4 Implement room temperature display (large, prominent)
- [ ] 8.5 Implement room humidity display
- [ ] 8.6 Create temperature setpoint control UI
- [ ] 8.7 Create timer control UI
- [ ] 8.8 Implement power ON/OFF toggle button
- [ ] 8.9 Add loading states and error messages

## 9. Temperature Control UI
- [ ] 9.1 Design temperature display card
- [ ] 9.2 Implement increment button (+1°C)
- [ ] 9.3 Implement decrement button (-1°C)
- [ ] 9.4 Show current setpoint vs. actual temperature
- [ ] 9.5 Add visual feedback on button press
- [ ] 9.6 Disable buttons at min/max limits
- [ ] 9.7 Test temperature control flow

## 10. Timer Control UI
- [ ] 10.1 Design timer display card
- [ ] 10.2 Implement increment button (+15 min)
- [ ] 10.3 Implement decrement button (-15 min)
- [ ] 10.4 Display countdown timer (HH:MM:SS)
- [ ] 10.5 Implement client-side countdown logic
- [ ] 10.6 Add timer expiration indication
- [ ] 10.7 Test timer control and countdown

## 11. WiFi Provisioning UI
- [ ] 11.1 Create WiFi setup screen/dialog
- [ ] 11.2 Implement SSID input field
- [ ] 11.3 Implement password input field (masked)
- [ ] 11.4 Add connect button with validation
- [ ] 11.5 Show connection progress indicator
- [ ] 11.6 Display success/failure feedback
- [ ] 11.7 Test WiFi provisioning flow

## 12. Settings Screen
- [ ] 12.1 Create settings screen (Compose)
- [ ] 12.2 Device IP address configuration
- [ ] 12.3 WiFi provisioning option
- [ ] 12.4 About section (app version, device info)
- [ ] 12.5 Clear device settings option
- [ ] 12.6 Test settings persistence

## 13. Real-Time Updates Integration
- [ ] 13.1 Connect WebSocket to ViewModel
- [ ] 13.2 Update UI on snapshot reception
- [ ] 13.3 Update UI on change messages
- [ ] 13.4 Ensure smooth UI updates without flicker
- [ ] 13.5 Handle background/foreground transitions
- [ ] 13.6 Test real-time updates with device

## 14. Connection Management
- [ ] 14.1 Implement connection state indicator
- [ ] 14.2 Add "Connecting..." state
- [ ] 14.3 Add "Connected" state with visual feedback
- [ ] 14.4 Add "Disconnected" state with retry option
- [ ] 14.5 Auto-connect on app launch
- [ ] 14.6 Handle app backgrounding (pause WebSocket)
- [ ] 14.7 Reconnect on app foregrounding

## 15. Error Handling
- [ ] 15.1 Display network errors to user
- [ ] 15.2 Handle device unreachable errors
- [ ] 15.3 Handle invalid responses from device
- [ ] 15.4 Display sensor errors from WebSocket
- [ ] 15.5 Add retry mechanisms for failures
- [ ] 15.6 Test error scenarios

## 16. UI Polish
- [ ] 16.1 Apply IoT-style Material Design theme
- [ ] 16.2 Add icons for temperature, humidity, heating, timer
- [ ] 16.3 Implement color scheme (consistent with web panel)
- [ ] 16.4 Add smooth animations for state changes
- [ ] 16.5 Ensure responsive layout for different screen sizes
- [ ] 16.6 Add haptic feedback on button presses (optional)
- [ ] 16.7 Test UI on multiple Android devices

## 17. Permissions
- [ ] 17.1 Request WiFi state permission
- [ ] 17.2 Request internet permission (manifest)
- [ ] 17.3 Handle permission denials gracefully
- [ ] 17.4 Test with all permissions granted/denied

## 18. Offline/Connectivity Handling
- [ ] 18.1 Detect when device goes offline
- [ ] 18.2 Display offline indicator
- [ ] 18.3 Cache last known state
- [ ] 18.4 Queue commands during offline (optional)
- [ ] 18.5 Test behavior during network loss

## 19. Testing
- [ ] 19.1 Test with SoftAP connection (192.168.4.1)
- [ ] 19.2 Test with local network connection
- [ ] 19.3 Test switching between WiFi networks
- [ ] 19.4 Test all control functions (temp, timer, power)
- [ ] 19.5 Test real-time updates accuracy
- [ ] 19.6 Test WiFi provisioning flow end-to-end
- [ ] 19.7 Test app backgrounding and foregrounding
- [ ] 19.8 Test error scenarios (device off, network down)
- [ ] 19.9 Performance testing (battery usage, memory)

## 20. Documentation
- [ ] 20.1 User guide for app installation
- [ ] 20.2 Connection instructions (SoftAP vs. local)
- [ ] 20.3 Troubleshooting guide
- [ ] 20.4 Developer documentation for code structure
- [ ] 20.5 API integration guide (for future developers)

## 21. Build and Release
- [ ] 21.1 Configure release build settings
- [ ] 21.2 Generate signed APK
- [ ] 21.3 Test release build on real devices
- [ ] 21.4 Prepare for Play Store submission (optional)
- [ ] 21.5 Create app icon and branding assets

