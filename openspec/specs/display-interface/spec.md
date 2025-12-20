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
The system SHALL display information in three-column vertical layout with status icon row.

#### Scenario: Layout structure
- **WHEN** main display is rendered
- **THEN** layout SHALL show status icons top row, room conditions center, setpoint left, timer right

#### Scenario: Column separation
- **WHEN** multiple data elements are displayed
- **THEN** elements SHALL be visually separated and not overlap

### Requirement: Room Temperature and Humidity Display
The system SHALL display current room temperature and humidity in center section with large font.

#### Scenario: Room temperature display
- **WHEN** DHT22 provides valid reading
- **THEN** system SHALL display temperature with one decimal place and °C symbol in large font

#### Scenario: Room humidity display
- **WHEN** DHT22 provides valid reading
- **THEN** system SHALL display humidity as integer percentage with % symbol

#### Scenario: DHT22 sensor error indication
- **WHEN** DHT22 sensor fails or reading is invalid
- **THEN** system SHALL display "---" in place of temperature and humidity values

### Requirement: Temperature Setpoint Display
The system SHALL display temperature setpoint on left side in medium font.

#### Scenario: Setpoint value display
- **WHEN** setpoint is configured
- **THEN** system SHALL display value as integer with °C symbol on left side

#### Scenario: Setpoint update on change
- **WHEN** setpoint is changed via button or API
- **THEN** display SHALL update within 100ms

### Requirement: Timer Remaining Display
The system SHALL display timer remaining time on right side in small font as MM:SS format.

#### Scenario: Timer countdown display
- **WHEN** timer is running
- **THEN** system SHALL display remaining time in minutes:seconds format

#### Scenario: Timer expired display
- **WHEN** timer has expired
- **THEN** system SHALL display "00:00"

#### Scenario: Timer disabled display
- **WHEN** heating system is disabled
- **THEN** system SHALL display paused countdown value

### Requirement: Heating Element Status Icon
The system SHALL display heating element icon in top-left corner when element is active.

#### Scenario: Heating icon visible when active
- **WHEN** heating element is ON
- **THEN** heating icon SHALL be visible in top-left corner

#### Scenario: Heating icon hidden when inactive
- **WHEN** heating element is OFF
- **THEN** heating icon SHALL not be displayed

### Requirement: WiFi Status Icon
The system SHALL display WiFi status icon in top-right corner indicating connection state.

#### Scenario: WiFi connected icon
- **WHEN** device is connected to WiFi network
- **THEN** WiFi icon SHALL show connected state

#### Scenario: WiFi AP mode icon
- **WHEN** device is in SoftAP mode (Phase 3)
- **THEN** WiFi icon SHALL show AP mode indicator

#### Scenario: WiFi disconnected icon
- **WHEN** device has no WiFi connection and AP is off
- **THEN** WiFi icon SHALL show disconnected state or be hidden

### Requirement: Display Zoom Animation
The system SHALL animate changing values by zooming them larger temporarily.

#### Scenario: Zoom in on value change
- **WHEN** user changes temperature or timer via button
- **THEN** changed value SHALL zoom to 150% size over 300ms with smooth easing

#### Scenario: Hold zoom while adjusting
- **WHEN** user continues adjusting value
- **THEN** zoomed display SHALL remain until 3 seconds after last change

#### Scenario: Zoom out after idle
- **WHEN** 3 seconds pass without value changes
- **THEN** display SHALL animate back to normal layout over 300ms

#### Scenario: Animation smoothness
- **WHEN** zoom animation is active
- **THEN** animation SHALL maintain minimum 30 FPS frame rate

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
The system SHALL display error messages for critical sensor failures.

#### Scenario: Mattress sensor error message
- **WHEN** DS18B20 sensor fails
- **THEN** display SHALL show "Check mattress sensor" error message

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

