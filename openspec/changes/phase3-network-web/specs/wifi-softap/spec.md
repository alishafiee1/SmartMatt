# WiFi SoftAP Specification

## ADDED Requirements

### Requirement: SoftAP Mode Initialization
The system SHALL initialize WiFi SoftAP mode on first boot with device-specific SSID.

#### Scenario: First boot SoftAP activation
- **WHEN** device boots for first time
- **THEN** SoftAP SHALL be enabled with SSID "RODI_" plus last 4 MAC address digits

#### Scenario: SoftAP IP configuration
- **WHEN** SoftAP is enabled
- **THEN** device SHALL have IP address 192.168.4.1 with DHCP server active

#### Scenario: SoftAP SSID format
- **WHEN** SoftAP SSID is generated
- **THEN** format SHALL be "RODI_XXXX" where XXXX is last 4 hex digits of MAC address

### Requirement: SoftAP Client Connectivity
The system SHALL allow up to 4 clients to connect simultaneously to SoftAP.

#### Scenario: Client connection acceptance
- **WHEN** client attempts to connect to SoftAP
- **THEN** connection SHALL be accepted if fewer than 4 clients connected

#### Scenario: Maximum clients rejection
- **WHEN** 5th client attempts to connect
- **THEN** connection SHALL be rejected

#### Scenario: Client IP assignment
- **WHEN** client connects to SoftAP
- **THEN** DHCP server SHALL assign IP in range 192.168.4.2 to 192.168.4.254

### Requirement: SoftAP Enable/Disable Toggle
The system SHALL provide toggle functionality to enable or disable SoftAP mode.

#### Scenario: Enable SoftAP via button
- **WHEN** power button is held for 3 seconds and SoftAP is disabled
- **THEN** system SHALL enable SoftAP and update display icon

#### Scenario: Disable SoftAP via button
- **WHEN** power button is held for 3 seconds and SoftAP is enabled
- **THEN** system SHALL disable SoftAP and update display icon

#### Scenario: Prevent lockout on disable
- **WHEN** SoftAP disable is requested and Station mode is not connected
- **THEN** system SHALL refuse to disable SoftAP and display warning

### Requirement: SoftAP State Persistence
The system SHALL persist SoftAP enabled state across reboots.

#### Scenario: Save SoftAP state on change
- **WHEN** SoftAP is enabled or disabled
- **THEN** state SHALL be saved to NVS

#### Scenario: Restore SoftAP state on boot
- **WHEN** device boots
- **THEN** SoftAP state SHALL be restored from NVS

### Requirement: SoftAP Security Configuration
The system SHALL configure SoftAP with WPA2 password protection or open mode.

#### Scenario: Password-protected SoftAP
- **WHEN** SoftAP password is configured
- **THEN** WPA2 security SHALL be enabled with configured password

#### Scenario: Open SoftAP mode
- **WHEN** no SoftAP password is configured
- **THEN** SoftAP SHALL operate in open mode without authentication

### Requirement: SoftAP Channel Configuration
The system SHALL configure SoftAP on 2.4GHz WiFi channel 1 by default.

#### Scenario: Default channel selection
- **WHEN** SoftAP is initialized
- **THEN** WiFi channel SHALL be set to channel 1

### Requirement: SoftAP Status Indication
The system SHALL indicate SoftAP status on OLED display.

#### Scenario: SoftAP enabled display
- **WHEN** SoftAP is enabled
- **THEN** WiFi icon SHALL show AP mode indicator

#### Scenario: SoftAP disabled display
- **WHEN** SoftAP is disabled and Station is connected
- **THEN** WiFi icon SHALL show Station mode only

#### Scenario: SoftAP with Station display
- **WHEN** both SoftAP and Station are active
- **THEN** WiFi icon SHALL indicate dual mode operation

