# App Network Communication Specification

## ADDED Requirements

### Requirement: REST API Client Integration
The application SHALL use Phase 3 REST API endpoints for device control commands.

#### Scenario: Enable heating API call
- **WHEN** POST /api/heating/enable is called
- **THEN** HTTP request SHALL be sent and response SHALL be parsed

#### Scenario: Temperature setpoint API call
- **WHEN** POST /api/temperature with setpoint value
- **THEN** HTTP request SHALL be sent with JSON body and success SHALL be confirmed

#### Scenario: Timer duration API call
- **WHEN** POST /api/timer with duration value
- **THEN** HTTP request SHALL be sent with JSON body and success SHALL be confirmed

#### Scenario: System status query
- **WHEN** GET /api/status is called
- **THEN** complete device status JSON SHALL be retrieved and parsed

### Requirement: WebSocket Client Implementation
The application SHALL use WebSocket connection for real-time sensor data reception.

#### Scenario: WebSocket connection establishment
- **WHEN** app attempts to connect to device
- **THEN** WebSocket upgrade SHALL be performed to ws://<ip>/ws endpoint

#### Scenario: Receive snapshot message
- **WHEN** WebSocket connection completes
- **THEN** snapshot message with type="snapshot" SHALL be received and parsed

#### Scenario: Receive update messages
- **WHEN** device state changes
- **THEN** update message with type="update" and changed fields SHALL be received

#### Scenario: WebSocket message parsing
- **WHEN** WebSocket message is received
- **THEN** JSON SHALL be parsed into typed data models

### Requirement: Device IP Configuration
The application SHALL support both SoftAP default IP and custom IP address configuration.

#### Scenario: SoftAP default connection
- **WHEN** user selects SoftAP connection
- **THEN** app SHALL connect to 192.168.4.1 by default

#### Scenario: Custom IP address entry
- **WHEN** user enters custom IP address
- **THEN** app SHALL validate IP format and attempt connection

#### Scenario: IP address persistence
- **WHEN** successful connection is made
- **THEN** IP address SHALL be saved for future app launches

### Requirement: WiFi Provisioning API
The application SHALL provide WiFi provisioning interface using Phase 3 provisioning API.

#### Scenario: Send WiFi credentials
- **WHEN** user submits SSID and password
- **THEN** POST /api/wifi SHALL be called with credentials

#### Scenario: Provisioning validation
- **WHEN** credentials are entered
- **THEN** SSID and password length SHALL be validated before API call

#### Scenario: Provisioning feedback
- **WHEN** provisioning API returns
- **THEN** success or error message SHALL be displayed to user

### Requirement: Network Request Timeout
The application SHALL implement timeout for network requests.

#### Scenario: REST API timeout
- **WHEN** REST API call does not complete within 10 seconds
- **THEN** request SHALL timeout and error SHALL be reported

#### Scenario: WebSocket connection timeout
- **WHEN** WebSocket connection attempt does not complete within 5 seconds
- **THEN** connection SHALL timeout and retry logic SHALL activate

### Requirement: WebSocket Reconnection Logic
The application SHALL automatically reconnect WebSocket on disconnection.

#### Scenario: Unexpected disconnection
- **WHEN** WebSocket connection is lost unexpectedly
- **THEN** app SHALL attempt reconnection with exponential backoff

#### Scenario: Reconnection backoff
- **WHEN** reconnection attempts fail
- **THEN** retry intervals SHALL be 1s, 2s, 4s, 8s, up to maximum 30s

#### Scenario: Successful reconnection
- **WHEN** WebSocket reconnects
- **THEN** snapshot message SHALL be requested to refresh state

### Requirement: Network Error Handling
The application SHALL handle network errors gracefully with user feedback.

#### Scenario: Connection refused error
- **WHEN** device connection is refused
- **THEN** "Device unreachable" error SHALL be displayed

#### Scenario: Timeout error handling
- **WHEN** network timeout occurs
- **THEN** "Connection timeout" error SHALL be displayed with retry option

#### Scenario: HTTP error codes
- **WHEN** API returns 400 or 500 error
- **THEN** error message from response SHALL be displayed

### Requirement: JSON Message Format Compliance
The application SHALL parse JSON messages matching Phase 3 format specification.

#### Scenario: Parse snapshot message
- **WHEN** snapshot message is received
- **THEN** JSON SHALL be parsed with fields: type, data, timestamp

#### Scenario: Parse update message
- **WHEN** update message is received
- **THEN** only changed fields SHALL be extracted and applied

#### Scenario: Handle unknown fields
- **WHEN** JSON contains unknown fields
- **THEN** parser SHALL ignore unknown fields without error

### Requirement: Concurrent Request Handling
The application SHALL handle multiple concurrent API requests safely.

#### Scenario: Multiple API calls queued
- **WHEN** user rapidly presses multiple control buttons
- **THEN** API calls SHALL be queued and executed sequentially

#### Scenario: API call during WebSocket update
- **WHEN** API call is made while WebSocket message arrives
- **THEN** both SHALL be processed without data corruption

### Requirement: Connection State Management
The application SHALL maintain and expose connection state to UI.

#### Scenario: Disconnected state
- **WHEN** no connection exists
- **THEN** connection state SHALL be "Disconnected"

#### Scenario: Connecting state
- **WHEN** connection attempt is in progress
- **THEN** connection state SHALL be "Connecting"

#### Scenario: Connected state
- **WHEN** WebSocket connection is active and healthy
- **THEN** connection state SHALL be "Connected"

### Requirement: Network Security
The application SHALL use HTTP and unencrypted WebSocket as per Phase 3 design.

#### Scenario: HTTP protocol usage
- **WHEN** REST API calls are made
- **THEN** http:// protocol SHALL be used (not https)

#### Scenario: WebSocket protocol usage
- **WHEN** WebSocket connection is established
- **THEN** ws:// protocol SHALL be used (not wss)

### Requirement: Request Headers
The application SHALL include appropriate HTTP headers in API requests.

#### Scenario: Content-Type header
- **WHEN** POST request with JSON body is sent
- **THEN** Content-Type: application/json header SHALL be included

#### Scenario: Accept header
- **WHEN** any API request is sent
- **THEN** Accept: application/json header SHALL be included

### Requirement: Device Discovery (Manual)
The application SHALL support manual device connection without auto-discovery.

#### Scenario: Manual IP entry
- **WHEN** user enters device IP address
- **THEN** app SHALL attempt connection to entered IP

#### Scenario: SoftAP quick connect
- **WHEN** user selects SoftAP mode
- **THEN** app SHALL automatically use 192.168.4.1 without manual entry

### Requirement: Network Permission Handling
The application SHALL request and handle necessary Android network permissions.

#### Scenario: Internet permission
- **WHEN** app is installed
- **THEN** INTERNET permission SHALL be declared in manifest

#### Scenario: Network state permission
- **WHEN** app checks network connectivity
- **THEN** ACCESS_NETWORK_STATE permission SHALL be used

#### Scenario: WiFi state permission
- **WHEN** app checks WiFi connection
- **THEN** ACCESS_WIFI_STATE permission SHALL be used

