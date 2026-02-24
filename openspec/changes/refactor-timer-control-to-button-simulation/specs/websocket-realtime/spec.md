## MODIFIED Requirements

### Requirement: Initial Snapshot Message
The system SHALL send complete system state snapshot immediately after WebSocket connection.

#### Scenario: Full snapshot on connection
- **WHEN** WebSocket connection is established
- **THEN** server SHALL send JSON message with type "snapshot" containing all current values

#### Scenario: Snapshot includes all state
- **WHEN** snapshot message is sent
- **THEN** message SHALL include room temp, room humidity, mattress temp, setpoint, heating state, timer duration (not remaining time), timer remaining seconds, and WiFi mode

### Requirement: Change-Based Updates
The system SHALL send WebSocket messages only when monitored values change.

#### Scenario: Temperature change push
- **WHEN** room or mattress temperature changes by 0.1°C or more
- **THEN** server SHALL send update message with changed temperature values

#### Scenario: Heating state change push
- **WHEN** heating element turns ON or OFF
- **THEN** server SHALL immediately send update message with new heating state

#### Scenario: Timer duration change push
- **WHEN** timer duration changes (from hardware button or API)
- **THEN** server SHALL immediately send update message with new timer duration value

#### Scenario: No update for unchanged values
- **WHEN** sensor reading repeats same value
- **THEN** server SHALL NOT send redundant update message

### Requirement: Timer Value Distribution
The system SHALL send timer duration and remaining time values on specific events.

#### Scenario: Timer values on connection
- **WHEN** WebSocket connection is established
- **THEN** snapshot SHALL include current timer duration and timer remaining seconds

#### Scenario: Timer duration on change
- **WHEN** timer duration changes via any interface (hardware button or API)
- **THEN** server SHALL send update with new timer duration value immediately

#### Scenario: Timer remaining time updates
- **WHEN** timer is running and remaining time changes
- **THEN** server SHALL send remaining time updates periodically (similar to temperature updates)

## REMOVED Requirements

### Requirement: Client-Side Countdown
**Reason**: Timer countdown is now handled server-side. Clients receive remaining time via WebSocket but do not perform countdown locally.

**Migration**: Clients SHALL display timer duration (not countdown) and receive duration updates via WebSocket.

#### Scenario: WebSocket migration from client countdown
- **WHEN** a client previously performed local countdown based on initial duration
- **THEN** the client SHALL stop local countdown logic
- **AND** the client SHALL display timer duration and remaining time values received from WebSocket messages
