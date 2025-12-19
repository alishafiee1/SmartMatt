# Timer Management Specification

## ADDED Requirements

### Requirement: Timer Duration Configuration
The system SHALL allow timer duration configuration within the range of 15 minutes to 8 hours with 15-minute resolution.

#### Scenario: Valid timer duration configuration
- **WHEN** timer duration is set to a value between 15 minutes and 8 hours
- **THEN** system SHALL accept and store the duration

#### Scenario: Below minimum duration rejection
- **WHEN** timer duration below 15 minutes is requested
- **THEN** system SHALL reject the request and maintain current duration

#### Scenario: Above maximum duration rejection
- **WHEN** timer duration above 8 hours is requested
- **THEN** system SHALL reject the request and maintain current duration

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
The system SHALL provide accurate remaining time in minutes for active timer.

#### Scenario: Calculate remaining time
- **WHEN** timer is active and remaining time is requested
- **THEN** system SHALL return time remaining in minutes

#### Scenario: Zero remaining time after expiration
- **WHEN** timer has expired
- **THEN** remaining time SHALL be zero

#### Scenario: No remaining time when disabled
- **WHEN** heating system is disabled
- **THEN** remaining time SHALL reflect paused countdown value

### Requirement: Timer Accuracy
The system SHALL maintain timer accuracy within ±1% over any configured duration using ESP32 hardware timer.

#### Scenario: Long duration accuracy
- **WHEN** timer is configured for 8 hours
- **THEN** timer SHALL expire within ±4.8 minutes of expected time

#### Scenario: Short duration accuracy
- **WHEN** timer is configured for 15 minutes
- **THEN** timer SHALL expire within ±9 seconds of expected time

### Requirement: Timer State Persistence
The system SHALL persist timer duration configuration across reboots but NOT persist countdown state.

#### Scenario: Timer duration persists across reboot
- **WHEN** system reboots
- **THEN** configured timer duration SHALL be restored from storage

#### Scenario: Timer countdown resets on reboot
- **WHEN** system reboots with active countdown
- **THEN** timer SHALL be in stopped state requiring user to restart

### Requirement: Timer Status Information
The system SHALL provide timer status including running state, remaining time, and configured duration.

#### Scenario: Query timer running state
- **WHEN** timer status is requested
- **THEN** system SHALL indicate if timer is running, stopped, or expired

#### Scenario: Query configured duration
- **WHEN** timer configuration is requested
- **THEN** system SHALL return stored duration in minutes

