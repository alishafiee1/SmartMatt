# Web Panel UI Specification

## ADDED Requirements

### Requirement: Responsive Web Interface
The system SHALL provide responsive web interface accessible from mobile and desktop browsers.

#### Scenario: Mobile layout rendering
- **WHEN** web panel is accessed from mobile device (<600px width)
- **THEN** UI SHALL display in single-column stacked layout

#### Scenario: Desktop layout rendering
- **WHEN** web panel is accessed from desktop browser (>900px width)
- **THEN** UI SHALL display in centered layout with maximum 800px width

#### Scenario: Tablet layout rendering
- **WHEN** web panel is accessed from tablet (600-900px width)
- **THEN** UI SHALL display in 2-column layout for controls

### Requirement: Real-Time Data Display
The system SHALL display real-time sensor data without page refresh using WebSocket.

#### Scenario: Initial data load
- **WHEN** web panel loads
- **THEN** UI SHALL connect to WebSocket and display initial snapshot data

#### Scenario: Live temperature updates
- **WHEN** WebSocket message with temperature change is received
- **THEN** UI SHALL immediately update displayed temperature without refresh

#### Scenario: Live heating status updates
- **WHEN** heating element state changes
- **THEN** heating indicator SHALL update within 200ms

#### Scenario: Connection status indication
- **WHEN** WebSocket connection status changes
- **THEN** UI SHALL display connection indicator (connected/disconnected)

### Requirement: Temperature Control Interface
The system SHALL provide intuitive temperature setpoint control interface.

#### Scenario: Temperature increment button
- **WHEN** user clicks temperature "+" button
- **THEN** setpoint SHALL increase by 1°C and API call SHALL be made

#### Scenario: Temperature decrement button
- **WHEN** user clicks temperature "-" button
- **THEN** setpoint SHALL decrease by 1°C and API call SHALL be made

#### Scenario: Temperature range limits
- **WHEN** setpoint reaches 45°C
- **THEN** "+" button SHALL be disabled

#### Scenario: Temperature visual feedback
- **WHEN** temperature change API succeeds
- **THEN** new setpoint SHALL be highlighted briefly for user confirmation

### Requirement: Timer Control Interface
The system SHALL provide timer duration control interface with countdown display.

#### Scenario: Timer increment button
- **WHEN** user clicks timer "+" button
- **THEN** duration SHALL increase by 15 minutes and API call SHALL be made

#### Scenario: Timer decrement button
- **WHEN** user clicks timer "-" button
- **THEN** duration SHALL decrease by 15 minutes and API call SHALL be made

#### Scenario: Timer countdown display
- **WHEN** timer is running
- **THEN** UI SHALL display countdown in HH:MM:SS format updating every second

#### Scenario: Client-side countdown
- **WHEN** timer value is received from WebSocket
- **THEN** JavaScript SHALL manage countdown locally without server updates

#### Scenario: Timer expiration display
- **WHEN** countdown reaches 00:00:00
- **THEN** UI SHALL display "Expired" or "00:00:00" and disable heating

### Requirement: Power Control Interface
The system SHALL provide prominent power on/off toggle control.

#### Scenario: Power on button
- **WHEN** user clicks power ON button
- **THEN** API call SHALL enable heating and UI SHALL show active state

#### Scenario: Power off button
- **WHEN** user clicks power OFF button
- **THEN** API call SHALL disable heating and UI SHALL show inactive state

#### Scenario: Power button visual state
- **WHEN** heating is ON
- **THEN** power button SHALL be visually distinct (highlighted/colored)

### Requirement: WiFi Provisioning Interface
The system SHALL provide WiFi configuration form for home network connection.

#### Scenario: WiFi SSID input
- **WHEN** user enters WiFi SSID in form
- **THEN** input SHALL accept 1-32 character SSID

#### Scenario: WiFi password input
- **WHEN** user enters WiFi password
- **THEN** input SHALL be masked and accept 8-63 characters

#### Scenario: WiFi connect button
- **WHEN** user clicks connect button with valid credentials
- **THEN** API call SHALL provision WiFi and display connection status

#### Scenario: WiFi connection feedback
- **WHEN** WiFi provisioning is in progress
- **THEN** loading indicator SHALL be displayed

#### Scenario: WiFi connection success
- **WHEN** WiFi connection succeeds
- **THEN** success message and assigned IP SHALL be displayed

#### Scenario: WiFi connection failure
- **WHEN** WiFi connection fails
- **THEN** error message with failure reason SHALL be displayed

### Requirement: Status Indicators
The system SHALL display visual indicators for system status.

#### Scenario: Heating active indicator
- **WHEN** heating element is ON
- **THEN** visual indicator (icon/color) SHALL show active heating

#### Scenario: WiFi status indicator
- **WHEN** web panel is loaded
- **THEN** WiFi mode (AP/STA/Both) and connection state SHALL be displayed

#### Scenario: Sensor error indicator
- **WHEN** sensor error is received via WebSocket
- **THEN** error message SHALL be prominently displayed

### Requirement: Error Handling and User Feedback
The system SHALL provide clear error messages and user feedback.

#### Scenario: API error display
- **WHEN** API call fails with error response
- **THEN** error message SHALL be displayed in UI

#### Scenario: Input validation feedback
- **WHEN** user enters invalid value (out of range)
- **THEN** validation message SHALL be displayed before API call

#### Scenario: Loading states
- **WHEN** API call is in progress
- **THEN** loading indicator SHALL be shown and controls SHALL be disabled

#### Scenario: Success feedback
- **WHEN** API call succeeds
- **THEN** brief success indication SHALL be shown

### Requirement: WebSocket Auto-Reconnection
The system SHALL automatically reconnect WebSocket on connection loss.

#### Scenario: Detect disconnection
- **WHEN** WebSocket connection is lost
- **THEN** UI SHALL display "Reconnecting..." message

#### Scenario: Reconnection attempts
- **WHEN** WebSocket disconnects
- **THEN** JavaScript SHALL attempt reconnection with exponential backoff

#### Scenario: Reconnection success
- **WHEN** WebSocket reconnects
- **THEN** UI SHALL refresh with current data and clear reconnection message

### Requirement: Modern UI Design
The system SHALL provide modern, IoT-style visual design.

#### Scenario: Clean visual layout
- **WHEN** web panel is rendered
- **THEN** UI SHALL use modern colors, rounded corners, and clear typography

#### Scenario: Touch-friendly controls
- **WHEN** panel is used on mobile device
- **THEN** buttons SHALL be large enough for touch (minimum 44x44 pixels)

#### Scenario: Visual hierarchy
- **WHEN** multiple information elements are displayed
- **THEN** most important data SHALL be visually prominent (larger, bolder)

### Requirement: Browser Compatibility
The system SHALL work on modern web browsers.

#### Scenario: Chrome/Edge compatibility
- **WHEN** web panel is opened in Chrome or Edge
- **THEN** all features SHALL function correctly

#### Scenario: Firefox compatibility
- **WHEN** web panel is opened in Firefox
- **THEN** all features SHALL function correctly

#### Scenario: Safari compatibility
- **WHEN** web panel is opened in Safari (macOS/iOS)
- **THEN** all features SHALL function correctly

#### Scenario: Unsupported browser detection
- **WHEN** web panel is opened in browser without WebSocket support
- **THEN** warning message SHALL be displayed suggesting browser upgrade

### Requirement: Fast Page Load
The system SHALL load web panel quickly with minimal resources.

#### Scenario: Initial page load time
- **WHEN** user navigates to web panel
- **THEN** page SHALL load and be interactive within 2 seconds

#### Scenario: Minimal external dependencies
- **WHEN** web panel is loaded
- **THEN** NO external CDN or internet resources SHALL be required

### Requirement: Room Conditions Display
The system SHALL prominently display current room temperature and humidity.

#### Scenario: Room temperature prominence
- **WHEN** web panel is displayed
- **THEN** room temperature SHALL be shown in large font as primary information

#### Scenario: Room humidity display
- **WHEN** valid humidity reading is available
- **THEN** humidity SHALL be displayed with percentage symbol

### Requirement: Offline Operation
The system SHALL work completely offline without internet connectivity.

#### Scenario: Offline functionality
- **WHEN** device and client have no internet connection
- **THEN** web panel SHALL function fully via local network or SoftAP

