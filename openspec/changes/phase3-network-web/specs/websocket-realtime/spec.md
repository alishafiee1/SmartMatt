# WebSocket Real-Time Communication Specification

## ADDED Requirements

### Requirement: WebSocket Server Initialization
The system SHALL initialize WebSocket server on HTTP port 80 with upgrade handler.

#### Scenario: WebSocket upgrade handling
- **WHEN** client sends WebSocket upgrade request to /ws
- **THEN** server SHALL upgrade HTTP connection to WebSocket protocol

#### Scenario: Multiple concurrent connections
- **WHEN** multiple clients request WebSocket connections
- **THEN** server SHALL support up to 4 concurrent WebSocket connections

### Requirement: Initial Snapshot Message
The system SHALL send complete system state snapshot immediately after WebSocket connection.

#### Scenario: Full snapshot on connection
- **WHEN** WebSocket connection is established
- **THEN** server SHALL send JSON message with type "snapshot" containing all current values

#### Scenario: Snapshot includes all state
- **WHEN** snapshot message is sent
- **THEN** message SHALL include room temp, room humidity, mattress temp, setpoint, heating state, timer remaining, and WiFi mode

### Requirement: Change-Based Updates
The system SHALL send WebSocket messages only when monitored values change.

#### Scenario: Temperature change push
- **WHEN** room or mattress temperature changes by 0.1°C or more
- **THEN** server SHALL send update message with changed temperature values

#### Scenario: Heating state change push
- **WHEN** heating element turns ON or OFF
- **THEN** server SHALL immediately send update message with new heating state

#### Scenario: No update for unchanged values
- **WHEN** sensor reading repeats same value
- **THEN** server SHALL NOT send redundant update message

### Requirement: Timer Value Distribution
The system SHALL send timer values only on specific events, not continuously.

#### Scenario: Timer value on connection
- **WHEN** WebSocket connection is established
- **THEN** snapshot SHALL include current timer remaining time

#### Scenario: Timer value on user change
- **WHEN** user changes timer duration via any interface
- **THEN** server SHALL send update with new timer value

#### Scenario: Timer expiration notification
- **WHEN** timer reaches zero
- **THEN** server SHALL send update indicating timer expired

#### Scenario: Client-side countdown
- **WHEN** client receives timer value
- **THEN** client SHALL manage countdown display locally without server updates

### Requirement: JSON Message Format
The system SHALL use standardized JSON format for all WebSocket messages.

#### Scenario: Snapshot message structure
- **WHEN** snapshot is sent
- **THEN** JSON SHALL have fields: type="snapshot", data={all values}, timestamp

#### Scenario: Update message structure
- **WHEN** update is sent
- **THEN** JSON SHALL have fields: type="update", data={changed values only}, timestamp

#### Scenario: Timestamp inclusion
- **WHEN** any message is sent
- **THEN** UNIX timestamp SHALL be included for client-side latency detection

### Requirement: WebSocket Connection Lifecycle
The system SHALL properly handle WebSocket connection, disconnection, and errors.

#### Scenario: Clean disconnection
- **WHEN** client closes WebSocket connection
- **THEN** server SHALL release connection resources and allow new connection

#### Scenario: Unexpected disconnection
- **WHEN** client disconnects without close frame
- **THEN** server SHALL detect disconnection and clean up connection

#### Scenario: Connection error handling
- **WHEN** WebSocket error occurs during send
- **THEN** server SHALL close connection gracefully and log error

### Requirement: Message Broadcasting
The system SHALL broadcast state changes to all connected WebSocket clients.

#### Scenario: Broadcast to multiple clients
- **WHEN** state change occurs with multiple clients connected
- **THEN** update message SHALL be sent to all connected clients

#### Scenario: Independent client handling
- **WHEN** one client disconnects
- **THEN** other clients SHALL continue receiving updates

### Requirement: WebSocket Performance
The system SHALL maintain low-latency WebSocket communication.

#### Scenario: Update latency
- **WHEN** state change occurs
- **THEN** WebSocket message SHALL be sent within 100ms

#### Scenario: Message size limit
- **WHEN** WebSocket message is prepared
- **THEN** message size SHALL not exceed 1KB

### Requirement: Connection Limits
The system SHALL enforce maximum concurrent WebSocket connections.

#### Scenario: Accept connection within limit
- **WHEN** new WebSocket connection requested and fewer than 4 connections active
- **THEN** connection SHALL be accepted

#### Scenario: Reject connection at limit
- **WHEN** new WebSocket connection requested and 4 connections already active
- **THEN** connection SHALL be rejected with appropriate HTTP status

### Requirement: WebSocket Error Messages
The system SHALL send error notifications via WebSocket for critical events.

#### Scenario: Sensor error notification
- **WHEN** critical sensor failure occurs
- **THEN** WebSocket SHALL send error message to all clients

#### Scenario: Error message format
- **WHEN** error is sent via WebSocket
- **THEN** message SHALL have type="error", error description, and timestamp

