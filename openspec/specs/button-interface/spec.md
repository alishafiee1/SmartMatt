# button-interface Specification

## Purpose
TBD - created by archiving change phase2-device-panel. Update Purpose after archive.
## Requirements
### Requirement: Button Debouncing
The system SHALL implement software debouncing for all physical buttons with 50ms debounce time.

#### Scenario: Button press detection with debounce
- **WHEN** button is pressed and released
- **THEN** system SHALL register single press event after 50ms stable low state

#### Scenario: Bounce rejection
- **WHEN** button produces electrical noise during press
- **THEN** system SHALL filter bounces and register single clean press

#### Scenario: Rapid press handling
- **WHEN** button is pressed and released rapidly (within 50ms)
- **THEN** system SHALL detect separate press events with proper debouncing

### Requirement: Temperature Control Buttons
The system SHALL provide temperature up/down buttons to adjust setpoint in 1°C increments.

#### Scenario: Temperature increase on up button
- **WHEN** temperature up button is pressed
- **THEN** system SHALL increase temperature setpoint by 1°C within valid range

#### Scenario: Temperature decrease on down button
- **WHEN** temperature down button is pressed
- **THEN** system SHALL decrease temperature setpoint by 1°C within valid range

#### Scenario: Temperature at maximum limit
- **WHEN** temperature up button is pressed and setpoint is at 45°C
- **THEN** system SHALL ignore press and maintain 45°C setpoint

#### Scenario: Temperature at minimum limit
- **WHEN** temperature down button is pressed and setpoint is at 20°C
- **THEN** system SHALL ignore press and maintain 20°C setpoint

### Requirement: Timer Control Buttons
The system SHALL provide timer up/down buttons to adjust duration using step size defined by `BTN_TIMER_INCREMENT` within the range defined by `TIMER_MIN_MIN` and `TIMER_MAX_MIN`. In the current configuration this corresponds to 5-minute increments between 10 minutes and 8 hours.

#### Scenario: Timer increase on up button
- **WHEN** timer up button is pressed
- **THEN** system SHALL increase timer duration by `BTN_TIMER_INCREMENT` within valid range

#### Scenario: Timer decrease on down button
- **WHEN** timer down button is pressed
- **THEN** system SHALL decrease timer duration by `BTN_TIMER_INCREMENT` within valid range

#### Scenario: Timer at maximum limit
- **WHEN** timer up button is pressed and duration is at `TIMER_MAX_MIN`
- **THEN** system SHALL ignore press and maintain maximum duration

#### Scenario: Timer at minimum limit
- **WHEN** timer down button is pressed and duration is at `TIMER_MIN_MIN`
- **THEN** system SHALL ignore press and maintain minimum duration

### Requirement: Power Button Toggle
The system SHALL provide power button for heating system enable/disable toggle.

#### Scenario: Enable heating with power button
- **WHEN** power button is pressed and heating is disabled
- **THEN** system SHALL enable heating system and start timer

#### Scenario: Disable heating with power button
- **WHEN** power button is pressed and heating is enabled
- **THEN** system SHALL disable heating system and pause timer

### Requirement: Long Press Detection
The system SHALL detect long press (3 seconds) for alternate button functions.

#### Scenario: Power button long press detection
- **WHEN** power button is held for 3 seconds
- **THEN** system SHALL trigger alternate function (SoftAP toggle in Phase 3)

#### Scenario: Long press does not trigger short press
- **WHEN** button is held for long press duration
- **THEN** system SHALL suppress short press event

#### Scenario: Release before long press threshold
- **WHEN** button is held for less than 3 seconds and released
- **THEN** system SHALL trigger short press event only

### Requirement: Hold-and-Repeat Acceleration
The system SHALL accelerate value changes when buttons are held continuously.

#### Scenario: Initial repeat rate
- **WHEN** temperature or timer button is held for more than 500ms
- **THEN** system SHALL start repeating action every 500ms

#### Scenario: Accelerated repeat rate
- **WHEN** button continues to be held for more than 3 seconds
- **THEN** system SHALL increase repeat rate to every 100ms

#### Scenario: Stop repeat on release
- **WHEN** held button is released
- **THEN** system SHALL immediately stop repeating actions

### Requirement: Button Press Responsiveness
The system SHALL process button press events within 100ms from physical press to display update.

#### Scenario: Quick feedback on press
- **WHEN** any button is pressed
- **THEN** corresponding display change SHALL be visible within 100ms

### Requirement: Button State Initialization
The system SHALL initialize all buttons to unpressed state on boot with internal pull-up resistors enabled.

#### Scenario: Boot with buttons unpressed
- **WHEN** system boots
- **THEN** all buttons SHALL be initialized with pull-up resistors and idle state

#### Scenario: Boot with button pressed
- **WHEN** system boots while button is pressed
- **THEN** system SHALL ignore initial pressed state until first release

