# display-interface Specification Delta

## MODIFIED Requirements

### Requirement: Three-Column Main Layout
The system SHALL display information in three-column vertical layout (Left: Set Temp, Center: Current Temp, Right: Timer) separated by full-height vertical divider lines.

#### Scenario: Three-column structure
- **WHEN** main display is rendered
- **THEN** layout SHALL show three vertical columns at X: 0-32 (left), 32-96 (center), 96-128 (right)

#### Scenario: Vertical divider lines
- **WHEN** main display is rendered
- **THEN** system SHALL draw vertical lines at X=32 and X=96 from Y=0 to Y=64

#### Scenario: No horizontal dividers
- **WHEN** main display is rendered
- **THEN** system SHALL NOT draw horizontal divider lines

### Requirement: Current Mattress Temperature Display
The system SHALL display current mattress temperature in center column with largest font size as primary information.

#### Scenario: Mattress temperature as primary display
- **WHEN** DS18B20 provides valid mattress temperature reading
- **THEN** system SHALL display temperature with one decimal place (no degree symbol) in center column using Font Size 2

#### Scenario: Mattress sensor error indication
- **WHEN** DS18B20 sensor fails or reading is invalid
- **THEN** system SHALL display three-line error message in center column: "check" on line 1, "mattress" on line 2, "cable" on line 3

### Requirement: Temperature Setpoint Display
The system SHALL display temperature setpoint in left column (X: 0-32) centered vertically with SET label.

#### Scenario: Setpoint value display in left column
- **WHEN** setpoint is configured
- **THEN** system SHALL display value with one decimal place and °C symbol in left column using Font Size 1


#### Scenario: SET label display
- **WHEN** setpoint is displayed
- **THEN** system SHALL show "SET" label below temperature value

### Requirement: Timer Remaining Display
The system SHALL display timer remaining time in right column (X: 96-128) in two-line format (number + "min" label) or "OFF" when system is disabled.

#### Scenario: Timer two-line format
- **WHEN** timer is running and system is enabled
- **THEN** system SHALL display remaining time as integer minutes on first line and "min" label on second line

#### Scenario: Timer OFF display
- **WHEN** system is disabled (heatingEnabled = false)
- **THEN** system SHALL display "OFF" in timer area instead of timer value

#### Scenario: Timer display when system enabled but element inactive
- **WHEN** system is enabled but heating element is inactive
- **THEN** system SHALL display timer value normally (not "OFF")

### Requirement: Room Temperature and Humidity Display
The system SHALL display room temperature and humidity in bottom section of center column in compact single-line format (without "Room:" prefix).

#### Scenario: Room conditions in center bottom
- **WHEN** DHT22 provides valid readings
- **THEN** system SHALL display "XX° YY%" at Y: 54-64 in center column

#### Scenario: Room conditions font size
- **WHEN** room conditions are displayed
- **THEN** system SHALL use Font Size 1 for compact display

#### Scenario: Room sensor error indication
- **WHEN** DHT22 sensor fails
- **THEN** system SHALL display "--- --%" in place of values

### Requirement: System Status Icon
The system SHALL display system status icon in center column top when system is enabled.

#### Scenario: System icon position
- **WHEN** system is enabled (heatingEnabled = true)
- **THEN** System icon SHALL be visible at X=64, Y=1

#### Scenario: System icon size
- **WHEN** system icon is displayed
- **THEN** icon SHALL be 8×8 pixels

#### Scenario: System icon hidden when disabled
- **WHEN** system is disabled (heatingEnabled = false)
- **THEN** System icon SHALL not be displayed

### Requirement: Error Message Display
The system SHALL display error messages for critical sensor failures with three-line format for mattress sensor.

#### Scenario: Mattress sensor error message format
- **WHEN** DS18B20 mattress sensor fails or disconnects
- **THEN** system SHALL display "check" on first line, "mattress" on second line, "cable" on third line in center column

#### Scenario: Error message position
- **WHEN** sensor error message is displayed
- **THEN** message SHALL be centered vertically and horizontally in center column (X: 32-96)

#### Scenario: Error message font size
- **WHEN** sensor error message is displayed
- **THEN** system SHALL use Font Size 1 for clear readability

#### Scenario: Layout preservation during error
- **WHEN** sensor error is active
- **THEN** system SHALL maintain three-column layout structure with vertical dividers and status icons

#### Scenario: Error message clearing
- **WHEN** sensor error is resolved
- **THEN** error message SHALL clear and normal display SHALL resume

### Requirement: System State Distinction
The system SHALL distinguish between heating enabled (system on/off) and heating active (element on/off) states for proper display behavior.

#### Scenario: Heating enabled state tracking
- **WHEN** DisplayManager receives update with heatingEnabled parameter
- **THEN** system SHALL store heatingEnabled state separately from heatingActive state

#### Scenario: Display behavior based on system state
- **WHEN** heatingEnabled is false
- **THEN** system SHALL display "OFF" in timer area and hide System icon
- **WHEN** heatingEnabled is true but heatingActive is false
- **THEN** system SHALL display timer normally and show System icon

### Requirement: Timer Button System Control
The system SHALL turn on when timer adjustment buttons are pressed while system is disabled.

#### Scenario: Timer up button turns system on
- **WHEN** system is disabled and user presses timer up button
- **THEN** system SHALL enable heating, start timer with new duration, and update display

#### Scenario: Timer down button turns system on
- **WHEN** system is disabled and user presses timer down button
- **THEN** system SHALL enable heating, start timer with new duration, and update display

## ADDED Requirements

None - all new functionality was already added in previous changes.

## REMOVED Requirements

None - no existing requirements were removed. Animation features were planned but never implemented in the base specification.

## RENAMED Requirements

None - requirement names remain consistent with existing spec structure.

