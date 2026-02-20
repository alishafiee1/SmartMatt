## ADDED Requirements

### Requirement: Button Simulation API
The system SHALL provide REST API endpoints to simulate hardware button presses for timer control.

#### Scenario: Timer up button simulation via API
- **WHEN** POST /api/timer/up is requested
- **THEN** system SHALL execute same logic as hardware timer up button press
- **AND** timer duration SHALL increase by BTN_TIMER_INCREMENT
- **AND** all button press side effects SHALL occur (e.g., enabling system if disabled)

#### Scenario: Timer down button simulation via API
- **WHEN** POST /api/timer/down is requested
- **THEN** system SHALL execute same logic as hardware timer down button press
- **AND** timer duration SHALL decrease by BTN_TIMER_INCREMENT
- **AND** all button press side effects SHALL occur (e.g., enabling system if disabled)

#### Scenario: Button simulation consistency
- **WHEN** timer button is pressed via API
- **THEN** behavior SHALL be identical to hardware button press
- **AND** step size, limits, and validation SHALL be same as hardware buttons

#### Scenario: Button simulation response
- **WHEN** button simulation API is called
- **THEN** response SHALL indicate success and current timer duration
- **AND** WebSocket update SHALL be sent to all connected clients

## MODIFIED Requirements

### Requirement: Timer Control Buttons
The system SHALL provide timer up/down buttons to adjust duration using BTN_TIMER_INCREMENT step size. Button handlers SHALL be accessible both from hardware GPIO and API simulation.

#### Scenario: Timer increase on up button
- **WHEN** timer up button is pressed (hardware or API simulation)
- **THEN** system SHALL increase timer duration by BTN_TIMER_INCREMENT within valid range

#### Scenario: Timer decrease on down button
- **WHEN** timer down button is pressed (hardware or API simulation)
- **THEN** system SHALL decrease timer duration by BTN_TIMER_INCREMENT within valid range

#### Scenario: Timer at maximum limit
- **WHEN** timer up button is pressed and duration is at TIMER_MAX_MIN
- **THEN** system SHALL ignore press and maintain maximum duration

#### Scenario: Timer at minimum limit
- **WHEN** timer down button is pressed and duration is at TIMER_MIN_MIN
- **THEN** system SHALL ignore press and maintain minimum duration

#### Scenario: Button handler reusability
- **WHEN** timer button handler is implemented
- **THEN** same handler SHALL be callable from both GPIO interrupt and API endpoint
