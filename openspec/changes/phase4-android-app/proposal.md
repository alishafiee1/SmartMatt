# Phase 4: Android Mobile Application

## Why
Users need a dedicated mobile application for convenient control and monitoring of the RODI Smart Mattress from their Android smartphones. A native app provides better user experience than web browser, with push notifications, faster access, and mobile-optimized UI. This completes the full user experience ecosystem: device panel, web interface, and mobile app.

## What Changes
- Develop Android application with modern Kotlin/Jetpack Compose
- Implement direct WiFi connection to device (both SoftAP and local network)
- Reuse Phase 3 REST API and WebSocket protocol (no ESP32 changes needed)
- Create IoT-optimized UI with real-time sensor monitoring
- Add device discovery on local network
- Implement push-style real-time updates using WebSocket
- Create settings management and device configuration UI
- Add WiFi provisioning flow within app
- Optimize for mobile battery usage

## Impact
- **Affected specs**: 
  - `android-app` (new)
  - `app-network` (new)
  - **NO changes to ESP32 firmware** - app uses existing Phase 3 APIs
- **Affected code**: 
  - Android app (new repository/project)
  - No ESP32 code changes
- **Hardware dependencies**: 
  - Android smartphone/tablet (Android 7.0+ / API level 24+)
  - WiFi connectivity
- **Phase 3 dependency**:
  - Requires Phase 3 REST API and WebSocket server
  - Uses same JSON protocol as web panel
  - Benefits from standardized API design

## Non-Goals (for this phase)
- iOS application (Android only initially)
- Multi-device management (single device only)
- Cloud connectivity or remote access
- Background service or persistent monitoring
- Widget or wearable integration
- Advanced analytics or historical data

## Prerequisites
- Phase 3 (WiFi & Web Panel) completed and tested
- Android development environment (Android Studio)
- Phase 3 REST API and WebSocket fully functional
- Understanding of Android app development (Kotlin)

## Breaking Changes
- None - Phase 4 is purely additive, no changes to ESP32 firmware

