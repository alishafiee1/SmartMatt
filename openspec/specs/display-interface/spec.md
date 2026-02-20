# display-interface Specification

## Purpose
TBD - created by archiving change phase2-device-panel. Update Purpose after archive.
## Requirements
### Requirement: OLED Display Initialization
The system SHALL initialize SSD1306 OLED display (128x64 pixels) on I2C bus during boot.

#### Scenario: Successful display initialization
- **WHEN** system boots with display connected
- **THEN** display SHALL be initialized at I2C address 0x3C with 400kHz clock

#### Scenario: Display initialization failure
- **WHEN** display initialization fails
- **THEN** system SHALL log error and continue operation without display

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

### Requirement: Room Temperature and Humidity Display
The system SHALL display room temperature and humidity in bottom section of center column in compact single-line format (without "Room:" prefix).

#### Scenario: Room conditions in center bottom
- **WHEN** DHT11 provides valid readings
- **THEN** system SHALL display "XX° YY%" at Y: 54-64 in center column

#### Scenario: Room conditions font size
- **WHEN** room conditions are displayed
- **THEN** system SHALL use Font Size 1 for compact display

#### Scenario: Room sensor error indication
- **WHEN** DHT11 sensor fails
- **THEN** system SHALL display "--- --%" in place of values

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

### Requirement: Heating Element Status Icon
The system SHALL display heating element icon in center column top when element is active.

#### Scenario: Heating icon position
- **WHEN** heating element is active
- **THEN** Heating icon SHALL be visible at X=52, Y=1

#### Scenario: Heating icon size
- **WHEN** heating icon is displayed
- **THEN** icon SHALL be 8×8 pixels

#### Scenario: Heating icon hidden when inactive
- **WHEN** heating element is inactive
- **THEN** Heating icon SHALL not be displayed

### Requirement: WiFi Status Icon
The system SHALL display WiFi status icon in center column top indicating connection state.

#### Scenario: WiFi icon position
- **WHEN** device has WiFi connection (SoftAP or Station)
- **THEN** WiFi icon SHALL be visible at X=40, Y=1

#### Scenario: WiFi icon size
- **WHEN** WiFi icon is displayed
- **THEN** icon SHALL be 8×8 pixels

#### Scenario: WiFi icon hidden when disabled
- **WHEN** WiFi is disabled
- **THEN** WiFi icon SHALL not be displayed

### Requirement: Display Update Frequency
The system SHALL update display at 1 Hz for periodic data and immediately for user interactions.

#### Scenario: Periodic sensor update
- **WHEN** new sensor readings are available
- **THEN** display SHALL update within 1 second

#### Scenario: Immediate button feedback
- **WHEN** button is pressed changing value
- **THEN** display SHALL update within 100ms

#### Scenario: Immediate status change
- **WHEN** heating element state changes
- **THEN** heating icon SHALL update within 100ms

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

### Requirement: Display Symbols and Icons
The system SHALL use appropriate symbols and icons for temperature, humidity, and status.

#### Scenario: Temperature symbol
- **WHEN** temperature is displayed
- **THEN** system SHALL use °C symbol

#### Scenario: Humidity symbol
- **WHEN** humidity is displayed
- **THEN** system SHALL use % symbol or humidity icon

#### Scenario: Icon clarity
- **WHEN** icons are displayed
- **THEN** icons SHALL be 12x12 pixels and clearly recognizable

### Requirement: Display Contrast and Readability
The system SHALL configure display for optimal readability at normal viewing angles.

#### Scenario: Display contrast
- **WHEN** display is initialized
- **THEN** contrast SHALL be set to provide clear visibility

#### Scenario: Font sizing
- **WHEN** text is rendered
- **THEN** font sizes SHALL be large enough to read from 50cm distance

### Requirement: Current Mattress Temperature Display
The system SHALL display current mattress temperature in center column with largest font size as primary information.

#### Scenario: Mattress temperature as primary display
- **WHEN** DS18B20 provides valid mattress temperature reading
- **THEN** system SHALL display temperature with one decimal place (no degree symbol) in center column using Font Size 2

#### Scenario: Mattress sensor error indication
- **WHEN** DS18B20 sensor fails or reading is invalid
- **THEN** system SHALL display three-line error message in center column: "check" on line 1, "mattress" on line 2, "cable" on line 3

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

