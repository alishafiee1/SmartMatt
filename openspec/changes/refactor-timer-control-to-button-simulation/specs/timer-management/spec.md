## MODIFIED Requirements

### Requirement: Timer Duration Configuration
The system SHALL allow timer duration configuration within the range defined by TIMER_MIN_MIN and TIMER_MAX_MIN with step size defined by BTN_TIMER_INCREMENT. All configuration values (min, max, step) SHALL be defined only in C++ code (RodiConfig.h).

#### Scenario: Valid timer duration configuration
- **WHEN** timer duration is set to a value within valid range using button press (hardware or simulated)
- **THEN** system SHALL accept and store the duration according to step size

#### Scenario: Below minimum duration rejection
- **WHEN** timer duration below TIMER_MIN_MIN is requested
- **THEN** system SHALL reject the request and maintain current duration

#### Scenario: Above maximum duration rejection
- **WHEN** timer duration above TIMER_MAX_MIN is requested
- **THEN** system SHALL reject the request and maintain current duration

#### Scenario: Configuration centralized in C++
- **WHEN** timer step size, minimum, or maximum needs to be changed
- **THEN** changes SHALL only require modification in C++ code (RodiConfig.h)

### Requirement: Timer Countdown Operation
The system SHALL maintain an active countdown timer when heating system is enabled.

#### Scenario: Timer starts on heating enable
- **WHEN** heating system is enabled
- **THEN** timer SHALL start counting down from configured duration

#### Scenario: Timer stops on heating disable
- **WHEN** heating system is disabled by user
- **THEN** timer SHALL pause countdown

#### Scenario: Timer expiration triggers shutoff
- **WHEN** timer countdown reaches zero
- **THEN** system SHALL disable heating system and stop timer

### Requirement: Timer Reset Behavior
The system SHALL reset timer to configured duration when heating system is re-enabled.

#### Scenario: Timer reset on enable after disable
- **WHEN** heating system is disabled and then re-enabled
- **THEN** timer SHALL reset to configured duration and start countdown

#### Scenario: Timer reset on enable after expiration
- **WHEN** timer has expired and heating system is re-enabled
- **THEN** timer SHALL reset to configured duration and start countdown

### Requirement: Remaining Time Calculation
The system SHALL provide accurate remaining time in seconds for active timer. Remaining time SHALL be calculated server-side and sent to clients via WebSocket.

#### Scenario: Calculate remaining time
- **WHEN** timer is active and remaining time is requested
- **THEN** system SHALL return time remaining in seconds

#### Scenario: Zero remaining time after expiration
- **WHEN** timer has expired
- **THEN** remaining time SHALL be zero

#### Scenario: No remaining time when disabled
- **WHEN** heating system is disabled
- **THEN** remaining time SHALL reflect paused countdown value

## REMOVED Requirements

### Requirement: Client-Side Timer Countdown
**Reason**: Timer countdown is now handled server-side only. Clients receive remaining time via WebSocket but do not perform countdown calculations locally.

**Migration**: Web applications SHALL display timer duration (not countdown) and receive duration updates via WebSocket when duration changes.

#### Scenario: Client-side countdown removed
- **WHEN** existing web applications rely on client-side timer countdown logic
- **THEN** they SHALL remove local countdown implementation
- **AND** they SHALL display only timer duration received from server via WebSocket
