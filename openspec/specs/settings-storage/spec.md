# settings-storage Specification

## Purpose
TBD - created by archiving change phase1-core-hardware. Update Purpose after archive.
## Requirements
### Requirement: Persistent Temperature Setpoint Storage
The system SHALL persist temperature setpoint to non-volatile storage and restore on boot.

#### Scenario: Save temperature setpoint
- **WHEN** temperature setpoint is changed
- **THEN** system SHALL write new value to NVS within 100ms

#### Scenario: Load temperature setpoint on boot
- **WHEN** system boots
- **THEN** system SHALL read temperature setpoint from NVS and apply to heating controller

#### Scenario: Default temperature on first boot
- **WHEN** system boots for first time with no stored setpoint
- **THEN** system SHALL initialize setpoint to 25°C default value

### Requirement: Persistent Timer Duration Storage
The system SHALL persist timer duration configuration to non-volatile storage and restore on boot.

#### Scenario: Save timer duration
- **WHEN** timer duration is changed
- **THEN** system SHALL write new value to NVS within 100ms

#### Scenario: Load timer duration on boot
- **WHEN** system boots
- **THEN** system SHALL read timer duration from NVS and configure timer

#### Scenario: Default timer duration on first boot
- **WHEN** system boots for first time with no stored duration
- **THEN** system SHALL initialize duration to 60 minutes default value

### Requirement: Settings Data Validation
The system SHALL validate all settings read from storage against valid ranges before applying.

#### Scenario: Valid temperature setpoint restored
- **WHEN** temperature setpoint is read from storage
- **THEN** system SHALL verify value is between 20-45°C before applying

#### Scenario: Invalid temperature setpoint recovery
- **WHEN** stored temperature setpoint is outside valid range
- **THEN** system SHALL use 25°C default and overwrite corrupted storage

#### Scenario: Valid timer duration restored
- **WHEN** timer duration is read from storage
- **THEN** system SHALL verify value is between 15 minutes and 8 hours before applying

#### Scenario: Invalid timer duration recovery
- **WHEN** stored timer duration is outside valid range
- **THEN** system SHALL use 60 minutes default and overwrite corrupted storage

### Requirement: Settings Namespace Management
The system SHALL use dedicated NVS namespace "rodi_settings" for all persistent settings.

#### Scenario: Initialize settings namespace
- **WHEN** system first accesses settings storage
- **THEN** system SHALL open or create "rodi_settings" namespace

#### Scenario: Isolated settings storage
- **WHEN** settings are stored or retrieved
- **THEN** system SHALL use "rodi_settings" namespace exclusively

### Requirement: Storage Error Handling
The system SHALL handle NVS read/write errors gracefully with fallback to defaults.

#### Scenario: Read error fallback to defaults
- **WHEN** NVS read operation fails
- **THEN** system SHALL use default values and log error

#### Scenario: Write error continues operation
- **WHEN** NVS write operation fails
- **THEN** system SHALL log error but continue with in-memory values

#### Scenario: Corrupted NVS recovery
- **WHEN** NVS namespace is corrupted
- **THEN** system SHALL erase namespace, reinitialize with defaults

### Requirement: Minimal Write Optimization
The system SHALL only write to NVS when settings actually change to minimize flash wear.

#### Scenario: Skip redundant writes
- **WHEN** setting is changed to its current value
- **THEN** system SHALL skip NVS write operation

#### Scenario: Write only on value change
- **WHEN** setting is changed to different value
- **THEN** system SHALL perform single NVS write operation

### Requirement: Settings Storage Keys
The system SHALL use specific key names for each stored setting with appropriate data types.

#### Scenario: Temperature setpoint key
- **WHEN** temperature setpoint is stored or retrieved
- **THEN** system SHALL use key "temp_sp" with float data type

#### Scenario: Timer duration key
- **WHEN** timer duration is stored or retrieved
- **THEN** system SHALL use key "timer_dur" with uint32 data type

### Requirement: Factory Reset Capability
The system SHALL provide mechanism to reset all settings to factory defaults.

#### Scenario: Factory reset clears settings
- **WHEN** factory reset is triggered
- **THEN** system SHALL erase "rodi_settings" namespace and reinitialize with defaults

#### Scenario: Factory reset values
- **WHEN** factory reset completes
- **THEN** temperature setpoint SHALL be 25°C and timer duration SHALL be 60 minutes

