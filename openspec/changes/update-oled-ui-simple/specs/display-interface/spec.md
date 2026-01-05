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
- **THEN** system SHALL display temperature with one decimal place and °C symbol in center column using Font Size 2

#### Scenario: Mattress temperature Bold animation
- **WHEN** mattress temperature changes significantly (≥0.5°C)
- **THEN** system SHALL display temperature with Font Size 3 for 500ms

#### Scenario: Mattress sensor error indication
- **WHEN** DS18B20 sensor fails or reading is invalid
- **THEN** system SHALL display three-line error message in center column: "check" on line 1, "mattress" on line 2, "cable" on line 3

### Requirement: Temperature Setpoint Display
The system SHALL display temperature setpoint in left column (X: 0-32) centered vertically with SET label.

#### Scenario: Setpoint value display in left column
- **WHEN** setpoint is configured
- **THEN** system SHALL display value with one decimal place and °C symbol in left column using Font Size 1

#### Scenario: Setpoint Bold animation on change
- **WHEN** setpoint is changed via button or API
- **THEN** system SHALL display value with Font Size 2 for 500ms and then return to Font Size 1

#### Scenario: SET label display
- **WHEN** setpoint is displayed
- **THEN** system SHALL show "SET" label below temperature value

### Requirement: Timer Remaining Display
The system SHALL display timer remaining time in right column (X: 96-128) as minutes only with "min" label.

#### Scenario: Timer minutes-only format
- **WHEN** timer is running
- **THEN** system SHALL display remaining time as integer minutes (e.g., "480 min")

#### Scenario: Timer Bold animation on change
- **WHEN** timer value is changed via button or API
- **THEN** system SHALL display value with Font Size 2 for 500ms and then return to Font Size 1

#### Scenario: Timer zero display
- **WHEN** timer has expired or is disabled
- **THEN** system SHALL display "0 min"

### Requirement: Room Conditions Display
The system SHALL display room temperature and humidity in bottom section of center column in compact single-line format.

#### Scenario: Room conditions in center bottom
- **WHEN** DHT22 provides valid readings
- **THEN** system SHALL display "XX° YY%" at Y: 54-64 in center column

#### Scenario: Room conditions font size
- **WHEN** room conditions are displayed
- **THEN** system SHALL use Font Size 1 for compact display

#### Scenario: Room sensor error indication
- **WHEN** DHT22 sensor fails
- **THEN** system SHALL display "--- --%" in place of values

### Requirement: System Status Icons
The system SHALL display three status icons (WiFi, Heating, System) in top section of center column.

#### Scenario: Three status icons layout
- **WHEN** main display is rendered
- **THEN** system SHALL display WiFi icon at X=40, Heating icon at X=52, System icon at X=64, all at Y=1

#### Scenario: Icon size
- **WHEN** status icons are rendered
- **THEN** each icon SHALL be 8×8 pixels

#### Scenario: WiFi icon states
- **WHEN** device has WiFi connection (SoftAP or Station)
- **THEN** WiFi icon SHALL be visible
- **WHEN** WiFi is disabled
- **THEN** WiFi icon SHALL not be displayed

#### Scenario: Heating icon states
- **WHEN** heating element is active
- **THEN** Heating icon SHALL be visible
- **WHEN** heating element is inactive
- **THEN** Heating icon SHALL not be displayed

#### Scenario: System icon states
- **WHEN** system is monitoring and active
- **THEN** System icon SHALL be visible
- **WHEN** system is powered off or standby
- **THEN** System icon SHALL not be displayed

## ADDED Requirements

### Requirement: Bold Animation System
The system SHALL provide simple Bold animation effect by temporarily increasing font size for 500ms when displayed values change.

#### Scenario: Animation trigger on value change
- **WHEN** setpoint, mattress temperature, or timer value changes
- **THEN** system SHALL trigger Bold animation for that specific value

#### Scenario: Animation duration
- **WHEN** Bold animation is triggered
- **THEN** animation SHALL last 500ms ± 50ms

#### Scenario: Font size increase during animation
- **WHEN** Bold animation is active for setpoint or timer
- **THEN** system SHALL display value with Font Size 2 instead of Font Size 1
- **WHEN** Bold animation is active for current temperature
- **THEN** system SHALL display value with Font Size 3 instead of Font Size 2

#### Scenario: Automatic animation reset
- **WHEN** 500ms elapsed since animation trigger
- **THEN** system SHALL return to normal font size

#### Scenario: Multiple simultaneous animations
- **WHEN** multiple values change simultaneously
- **THEN** system SHALL animate all changed values independently with separate timers

### Requirement: Mattress Sensor Error Display
The system SHALL display a three-line error message when mattress sensor fails and automatically disable heating element for safety.

#### Scenario: Error message format
- **WHEN** DS18B20 mattress sensor fails or disconnects
- **THEN** system SHALL display "check" on first line, "mattress" on second line, "cable" on third line in center column

#### Scenario: Error message position
- **WHEN** sensor error message is displayed
- **THEN** message SHALL be centered vertically and horizontally in center column (X: 32-96)

#### Scenario: Error message font size
- **WHEN** sensor error message is displayed
- **THEN** system SHALL use Font Size 1 for clear readability

#### Scenario: Heating element disabled during error
- **WHEN** mattress sensor error occurs
- **THEN** system SHALL immediately disable heating element and display error message

#### Scenario: Error message clearing
- **WHEN** mattress sensor reconnects and provides valid readings
- **THEN** system SHALL clear error message and resume normal three-column display

#### Scenario: Layout preservation during error
- **WHEN** sensor error is active
- **THEN** system SHALL maintain three-column layout structure with vertical dividers and status icons

### Requirement: Animation Callback Integration
The system SHALL accept animation trigger callbacks from ButtonManager for immediate user feedback.

#### Scenario: Set temperature callback registration
- **WHEN** ButtonManager registers onSetTempChanged callback
- **THEN** system SHALL store callback and invoke triggerSetTempBold() when called

#### Scenario: Timer callback registration
- **WHEN** ButtonManager registers onTimerChanged callback
- **THEN** system SHALL store callback and invoke triggerTimerBold() when called

#### Scenario: Button event triggers animation
- **WHEN** user presses temperature up/down button
- **THEN** ButtonManager SHALL invoke callback to trigger Bold animation for setpoint

#### Scenario: API change triggers animation
- **WHEN** value is changed via REST API or WebSocket
- **THEN** system SHALL trigger appropriate Bold animation

## REMOVED Requirements

### Requirement: Display Zoom Animation
**Reason**: Overly complex animation system with easing, zoom scaling, and hold states is replaced with simpler Bold effect.

**Migration**: Remove zoom animation state machine (`ANIM_ZOOM_IN`, `ANIM_ZOOM_OUT`, `ANIM_HOLD`), `m_animScale` variable, and `calculateAnimationScale()` method. Replace with simple Boolean active flag and start time for Bold animation.

### Requirement: Room Temperature and Humidity Display (old centered format)
**Reason**: Replaced with compact single-line format at bottom of center column.

**Migration**: Change from separate large-font display in center to compact "Room: XX° YY%" format at bottom. No API changes required.

## RENAMED Requirements

None - requirement names remain consistent with existing spec structure.

