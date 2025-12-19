# Android Application Specification

## ADDED Requirements

### Requirement: Android Platform Compatibility
The application SHALL support Android devices running Android 7.0 (API level 24) or higher.

#### Scenario: Minimum Android version support
- **WHEN** app is installed on device running Android 7.0 or higher
- **THEN** app SHALL install and function correctly

#### Scenario: Unsupported Android version
- **WHEN** installation is attempted on device below Android 7.0
- **THEN** Play Store or APK installer SHALL prevent installation

### Requirement: Main Control Screen
The application SHALL provide main screen with device monitoring and control interface.

#### Scenario: Display room temperature and humidity
- **WHEN** main screen is loaded with active WebSocket connection
- **THEN** current room temperature and humidity SHALL be displayed prominently

#### Scenario: Display temperature setpoint
- **WHEN** main screen is visible
- **THEN** current temperature setpoint SHALL be displayed with increment/decrement buttons

#### Scenario: Display timer countdown
- **WHEN** heating system is enabled
- **THEN** timer countdown SHALL be displayed in HH:MM:SS format and update every second

#### Scenario: Display heating status
- **WHEN** main screen is visible
- **THEN** heating element status (ON/OFF) SHALL be clearly indicated

### Requirement: Temperature Control
The application SHALL provide temperature setpoint control with increment/decrement buttons.

#### Scenario: Increase temperature setpoint
- **WHEN** user taps increment button
- **THEN** setpoint SHALL increase by 1°C and API call SHALL be made

#### Scenario: Decrease temperature setpoint
- **WHEN** user taps decrement button
- **THEN** setpoint SHALL decrease by 1°C and API call SHALL be made

#### Scenario: Temperature at maximum limit
- **WHEN** setpoint is 45°C
- **THEN** increment button SHALL be disabled

#### Scenario: Temperature at minimum limit
- **WHEN** setpoint is 20°C
- **THEN** decrement button SHALL be disabled

#### Scenario: Temperature control feedback
- **WHEN** temperature change succeeds
- **THEN** UI SHALL show confirmation and updated value

### Requirement: Timer Control
The application SHALL provide timer duration control with increment/decrement buttons.

#### Scenario: Increase timer duration
- **WHEN** user taps timer increment button
- **THEN** duration SHALL increase by 15 minutes and API call SHALL be made

#### Scenario: Decrease timer duration
- **WHEN** user taps timer decrement button
- **THEN** duration SHALL decrease by 15 minutes and API call SHALL be made

#### Scenario: Timer at maximum limit
- **WHEN** timer duration is 8 hours
- **THEN** increment button SHALL be disabled

#### Scenario: Timer at minimum limit
- **WHEN** timer duration is 15 minutes
- **THEN** decrement button SHALL be disabled

#### Scenario: Client-side countdown
- **WHEN** timer value is received via WebSocket
- **THEN** app SHALL manage countdown display locally without continuous server updates

### Requirement: Power Control
The application SHALL provide heating system enable/disable toggle.

#### Scenario: Enable heating via app
- **WHEN** user taps power ON button
- **THEN** heating system SHALL be enabled via API and button state SHALL update

#### Scenario: Disable heating via app
- **WHEN** user taps power OFF button
- **THEN** heating system SHALL be disabled via API and button state SHALL update

#### Scenario: Power button visual feedback
- **WHEN** heating is enabled
- **THEN** power button SHALL be visually distinct (colored/highlighted)

### Requirement: Real-Time Data Updates
The application SHALL display real-time sensor data updates via WebSocket connection.

#### Scenario: Receive initial snapshot
- **WHEN** WebSocket connection is established
- **THEN** app SHALL receive and display complete device state snapshot

#### Scenario: Update on sensor change
- **WHEN** WebSocket update message is received
- **THEN** UI SHALL immediately reflect changed values without refresh

#### Scenario: Update latency
- **WHEN** sensor value changes on device
- **THEN** app SHALL display updated value within 500ms

### Requirement: Connection Status Indication
The application SHALL indicate connection status to device.

#### Scenario: Connected state display
- **WHEN** WebSocket connection is active
- **THEN** connection indicator SHALL show "Connected" status

#### Scenario: Connecting state display
- **WHEN** connection is being established
- **THEN** loading indicator SHALL be shown with "Connecting..." message

#### Scenario: Disconnected state display
- **WHEN** connection is lost
- **THEN** UI SHALL show "Disconnected" with retry option

### Requirement: Settings Screen
The application SHALL provide settings screen for device configuration.

#### Scenario: Access settings screen
- **WHEN** user taps settings icon
- **THEN** settings screen SHALL be displayed

#### Scenario: Configure device IP address
- **WHEN** user enters IP address in settings
- **THEN** app SHALL save IP and use for connections

#### Scenario: Access WiFi provisioning
- **WHEN** user taps WiFi setup option
- **THEN** WiFi provisioning interface SHALL be shown

### Requirement: Error Display
The application SHALL display error messages for failed operations and sensor errors.

#### Scenario: API error display
- **WHEN** REST API call fails
- **THEN** error message SHALL be displayed to user

#### Scenario: Network error display
- **WHEN** device is unreachable
- **THEN** "Cannot connect to device" message SHALL be shown

#### Scenario: Sensor error display
- **WHEN** sensor error is received via WebSocket
- **THEN** error indicator SHALL be shown with error description

### Requirement: App Lifecycle Management
The application SHALL properly manage WebSocket connection based on app lifecycle.

#### Scenario: Connect on foreground
- **WHEN** app is brought to foreground
- **THEN** WebSocket connection SHALL be established to device

#### Scenario: Disconnect on background
- **WHEN** app moves to background
- **THEN** WebSocket connection SHALL be closed to conserve battery

#### Scenario: Reconnect on resume
- **WHEN** app resumes from background
- **THEN** WebSocket SHALL reconnect and fetch current state

### Requirement: Configuration Persistence
The application SHALL persist device connection settings across app restarts.

#### Scenario: Save device IP
- **WHEN** user successfully connects to device
- **THEN** device IP SHALL be saved to SharedPreferences

#### Scenario: Auto-connect on launch
- **WHEN** app launches and saved IP exists
- **THEN** app SHALL automatically attempt connection to saved IP

### Requirement: Material Design UI
The application SHALL use Material Design 3 guidelines for user interface.

#### Scenario: Material components usage
- **WHEN** UI is rendered
- **THEN** Material Design components SHALL be used (buttons, cards, typography)

#### Scenario: Touch target sizing
- **WHEN** interactive elements are displayed
- **THEN** minimum touch target SHALL be 48dp for accessibility

#### Scenario: Color scheme consistency
- **WHEN** UI is displayed
- **THEN** color scheme SHALL be consistent with RODI branding (blue primary, orange accent)

### Requirement: Screen Orientation Support
The application SHALL support both portrait and landscape orientations.

#### Scenario: Portrait orientation
- **WHEN** device is in portrait mode
- **THEN** UI SHALL display in single-column layout

#### Scenario: Landscape orientation
- **WHEN** device is rotated to landscape
- **THEN** UI SHALL adapt to wider layout without data loss

#### Scenario: State preservation on rotation
- **WHEN** screen is rotated
- **THEN** device connection and data SHALL be preserved

