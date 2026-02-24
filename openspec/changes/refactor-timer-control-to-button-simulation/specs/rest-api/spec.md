## MODIFIED Requirements

### Requirement: Timer Configuration API
The system SHALL provide REST API endpoints for timer control that simulate hardware button presses.

#### Scenario: Timer up button simulation
- **WHEN** POST /api/timer/up is requested
- **THEN** system SHALL simulate hardware timer up button press
- **AND** timer duration SHALL increase by BTN_TIMER_INCREMENT (if within valid range)
- **AND** API SHALL return JSON success response

#### Scenario: Timer down button simulation
- **WHEN** POST /api/timer/down is requested
- **THEN** system SHALL simulate hardware timer down button press
- **AND** timer duration SHALL decrease by BTN_TIMER_INCREMENT (if within valid range)
- **AND** API SHALL return JSON success response

#### Scenario: Timer button at maximum limit
- **WHEN** POST /api/timer/up is requested and duration is at TIMER_MAX_MIN
- **THEN** API SHALL return success but duration SHALL remain at maximum

#### Scenario: Timer button at minimum limit
- **WHEN** POST /api/timer/down is requested and duration is at TIMER_MIN_MIN
- **THEN** API SHALL return success but duration SHALL remain at minimum

#### Scenario: Timer API response
- **WHEN** timer button simulation API succeeds
- **THEN** response SHALL include success status and current timer duration

## REMOVED Requirements

### Requirement: Direct Timer Duration Setting API
**Reason**: Timer control now uses button simulation to centralize configuration in C++ code.

**Migration**: 
- Remove POST /api/timer endpoint that accepts duration_min
- Replace with POST /api/timer/up and POST /api/timer/down endpoints
- Web applications SHALL use button simulation endpoints instead of direct duration setting

#### Scenario: Migration from direct timer API
- **WHEN** an existing client calls the legacy POST /api/timer endpoint with duration_min
- **THEN** the client MUST be updated to call POST /api/timer/up and POST /api/timer/down endpoints instead
- **AND** the client SHALL rely on button simulation endpoints for all future timer duration changes
