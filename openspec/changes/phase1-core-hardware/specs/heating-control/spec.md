# Heating Control Specification

## ADDED Requirements

### Requirement: Temperature Setpoint Management
The system SHALL allow temperature setpoint configuration within the range of 20-45°C with 1°C resolution.

#### Scenario: Valid setpoint configuration
- **WHEN** setpoint is set to a value between 20-45°C
- **THEN** the system SHALL accept and store the setpoint

#### Scenario: Below minimum temperature rejection
- **WHEN** setpoint is requested below 20°C
- **THEN** the system SHALL reject the request and maintain current setpoint

#### Scenario: Above maximum temperature rejection
- **WHEN** setpoint is requested above 45°C
- **THEN** the system SHALL reject the request and maintain current setpoint

### Requirement: Hysteresis-Based Heating Control
The system SHALL control the heating element using a hysteresis algorithm with 2°C default differential (configurable).

#### Scenario: Heating activation below threshold
- **WHEN** heating is OFF and mattress temperature falls below (setpoint - hysteresis)
- **THEN** the system SHALL activate the heating element

#### Scenario: Heating deactivation at setpoint
- **WHEN** heating is ON and mattress temperature reaches setpoint
- **THEN** the system SHALL deactivate the heating element

#### Scenario: Prevent rapid cycling
- **WHEN** temperature oscillates within hysteresis range
- **THEN** the system SHALL maintain current heating state without toggling

### Requirement: Heating Enable/Disable Control
The system SHALL provide manual heating system enable/disable functionality independent of temperature control.

#### Scenario: Enable heating system
- **WHEN** user enables heating and timer is not expired
- **THEN** the system SHALL allow temperature-based heating control

#### Scenario: Disable heating system
- **WHEN** user disables heating
- **THEN** the system SHALL immediately turn OFF heating element and suspend temperature control

#### Scenario: Disabled state prevents heating
- **WHEN** heating system is disabled
- **THEN** heating element SHALL remain OFF regardless of temperature readings

### Requirement: Timer-Based Automatic Shutoff
The system SHALL automatically disable heating when the countdown timer expires.

#### Scenario: Heating stops on timer expiration
- **WHEN** countdown timer reaches zero
- **THEN** the system SHALL disable heating element and heating system

#### Scenario: Timer reset on enable
- **WHEN** heating system is re-enabled after being disabled
- **THEN** the timer SHALL reset to configured duration

### Requirement: Safe Boot State
The system SHALL initialize with heating element OFF and heating system disabled on every boot or reset.

#### Scenario: Power-on initialization
- **WHEN** device boots or resets
- **THEN** heating element SHALL be OFF and heating system SHALL be disabled

#### Scenario: Settings persist across boot
- **WHEN** device boots
- **THEN** temperature setpoint and timer duration SHALL be loaded from storage

### Requirement: Heating Status Indication
The system SHALL maintain and provide current heating element state (ON/OFF) and heating system enabled state.

#### Scenario: Query heating element state
- **WHEN** heating element state is requested
- **THEN** system SHALL return current relay state (ON/OFF)

#### Scenario: Query heating system enabled state
- **WHEN** heating system enabled state is requested
- **THEN** system SHALL return current enabled flag (true/false)

### Requirement: Emergency Heating Shutoff
The system SHALL immediately disable heating element when critical safety conditions are detected.

#### Scenario: Sensor failure emergency stop
- **WHEN** mattress temperature sensor (DS18B20) fails validation
- **THEN** system SHALL immediately turn OFF heating element and disable heating system

#### Scenario: Out-of-range temperature emergency stop
- **WHEN** mattress temperature reading exceeds 50°C (hardware safety limit)
- **THEN** system SHALL immediately turn OFF heating element and enter error state

