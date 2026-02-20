# Sensor Monitoring Specification

## ADDED Requirements

### Requirement: Mattress Temperature Monitoring
The system SHALL continuously monitor mattress internal temperature using DS18B20 sensor with readings updated every 2 seconds.

#### Scenario: Successful temperature reading
- **WHEN** DS18B20 sensor is connected and functioning
- **THEN** system SHALL read and update mattress temperature every 2 seconds with ±0.5°C accuracy

#### Scenario: Temperature reading available
- **WHEN** valid temperature reading is requested
- **THEN** system SHALL return most recent mattress temperature value

### Requirement: Room Temperature and Humidity Monitoring
The system SHALL continuously monitor room temperature and humidity using DHT11 sensor with readings updated every 2 seconds.

#### Scenario: Successful room conditions reading
- **WHEN** DHT11 sensor is connected and functioning
- **THEN** system SHALL read and update room temperature and humidity every 2 seconds

#### Scenario: Room conditions available
- **WHEN** room conditions are requested
- **THEN** system SHALL return most recent temperature (±0.5°C) and humidity (±2%) values

### Requirement: DS18B20 Sensor Validation
The system SHALL validate DS18B20 sensor connectivity and reading validity with retry mechanism.

#### Scenario: Sensor read retry on transient failure
- **WHEN** DS18B20 read fails
- **THEN** system SHALL retry up to 3 times with 100ms intervals

#### Scenario: Sensor failure detection
- **WHEN** DS18B20 read fails after 3 retries
- **THEN** system SHALL set sensor error flag and trigger emergency heating shutoff

#### Scenario: Sensor recovery detection
- **WHEN** DS18B20 sensor failure is active and successful read occurs
- **THEN** system SHALL clear error flag and allow normal operation

### Requirement: DHT11 Sensor Error Handling
The system SHALL handle DHT11 sensor errors gracefully without affecting critical heating control.

#### Scenario: DHT11 read retry on failure
- **WHEN** DHT11 read fails
- **THEN** system SHALL retry up to 3 times with 100ms intervals

#### Scenario: Non-critical DHT11 failure
- **WHEN** DHT11 read fails after 3 retries
- **THEN** system SHALL set warning flag but continue heating operation

#### Scenario: Use last valid DHT11 reading
- **WHEN** DHT11 sensor fails and last valid reading is less than 60 seconds old
- **THEN** system SHALL continue using cached reading for display purposes

### Requirement: Sensor Reading Range Validation
The system SHALL validate all sensor readings are within expected physical ranges.

#### Scenario: Valid DS18B20 reading range
- **WHEN** DS18B20 returns temperature reading
- **THEN** system SHALL accept values between -10°C and 85°C as valid

#### Scenario: Invalid DS18B20 reading rejection
- **WHEN** DS18B20 returns temperature outside -10°C to 85°C range
- **THEN** system SHALL treat reading as sensor error and trigger validation failure

#### Scenario: Valid DHT11 reading range
- **WHEN** DHT11 returns temperature and humidity readings
- **THEN** system SHALL accept temperature between -40°C and 80°C and humidity between 0% and 100%

### Requirement: Sensor Data Structure
The system SHALL maintain structured sensor data with timestamps and validity flags.

#### Scenario: Sensor data includes metadata
- **WHEN** sensor reading is stored
- **THEN** data structure SHALL include temperature/humidity value, timestamp, and valid flag

#### Scenario: Stale data detection
- **WHEN** sensor reading is older than 10 seconds
- **THEN** system SHALL mark reading as stale

### Requirement: Sensor Initialization
The system SHALL initialize all sensors during boot sequence with validation.

#### Scenario: DS18B20 initialization success
- **WHEN** system boots and DS18B20 is detected
- **THEN** system SHALL configure sensor and perform initial reading

#### Scenario: DHT11 initialization success
- **WHEN** system boots and DHT11 is detected
- **THEN** system SHALL configure sensor and perform initial reading

#### Scenario: Critical sensor initialization failure
- **WHEN** DS18B20 fails initialization during boot
- **THEN** system SHALL enter safe mode with heating disabled

#### Scenario: Non-critical sensor initialization failure
- **WHEN** DHT11 fails initialization during boot
- **THEN** system SHALL log warning and continue operation without room monitoring

