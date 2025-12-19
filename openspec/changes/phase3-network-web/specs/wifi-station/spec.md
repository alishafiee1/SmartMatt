# WiFi Station Mode Specification

## ADDED Requirements

### Requirement: WiFi Credentials Storage
The system SHALL store WiFi SSID and password in non-volatile storage.

#### Scenario: Save WiFi credentials on provisioning
- **WHEN** valid WiFi credentials are provided via provisioning API
- **THEN** SSID and password SHALL be saved to NVS

#### Scenario: Load WiFi credentials on boot
- **WHEN** device boots and WiFi credentials exist in NVS
- **THEN** credentials SHALL be loaded for Station mode connection

#### Scenario: No credentials on first boot
- **WHEN** device boots for first time with no stored credentials
- **THEN** Station mode SHALL remain disconnected

### Requirement: Station Mode Connection
The system SHALL attempt to connect to configured WiFi network using stored credentials.

#### Scenario: Successful Station connection
- **WHEN** device boots with valid WiFi credentials
- **THEN** system SHALL connect to WiFi network within 30 seconds

#### Scenario: Failed Station connection
- **WHEN** connection attempt fails after 30 seconds
- **THEN** system SHALL remain in SoftAP-only mode and log error

#### Scenario: DHCP IP assignment
- **WHEN** Station mode connects to network
- **THEN** device SHALL obtain IP address via DHCP

### Requirement: Auto-Reconnection
The system SHALL automatically reconnect to WiFi network if connection is lost.

#### Scenario: Connection lost detection
- **WHEN** WiFi connection is lost
- **THEN** system SHALL detect disconnection within 10 seconds

#### Scenario: Automatic reconnection attempts
- **WHEN** WiFi disconnection is detected
- **THEN** system SHALL attempt reconnection every 30 seconds for up to 3 retries

#### Scenario: Successful reconnection
- **WHEN** reconnection attempt succeeds
- **THEN** system SHALL resume normal Station mode operation

### Requirement: WiFi Provisioning API
The system SHALL provide REST API endpoint for WiFi network configuration.

#### Scenario: Provision WiFi via API
- **WHEN** POST /api/wifi with valid SSID and password
- **THEN** system SHALL validate credentials and attempt connection

#### Scenario: Invalid SSID rejection
- **WHEN** SSID is empty or longer than 32 characters
- **THEN** API SHALL return error response and reject provisioning

#### Scenario: Invalid password rejection
- **WHEN** password is shorter than 8 characters (for WPA2) or longer than 63 characters
- **THEN** API SHALL return error response and reject provisioning

#### Scenario: Test connection before saving
- **WHEN** new WiFi credentials are provided
- **THEN** system SHALL test connection before persisting to NVS

### Requirement: Dual-Mode Operation
The system SHALL support simultaneous SoftAP and Station mode operation.

#### Scenario: Both modes active
- **WHEN** Station connects successfully and SoftAP is enabled
- **THEN** both SoftAP and Station SHALL be active simultaneously

#### Scenario: Access via both interfaces
- **WHEN** dual mode is active
- **THEN** device SHALL be accessible via both SoftAP IP (192.168.4.1) and Station IP

### Requirement: Station Status Information
The system SHALL provide Station mode status including connection state and IP address.

#### Scenario: Query Station status
- **WHEN** GET /api/wifi is requested
- **THEN** response SHALL include connection state, SSID (without password), and IP address

#### Scenario: Display Station connection on OLED
- **WHEN** Station mode connects successfully
- **THEN** WiFi icon SHALL indicate connected state

#### Scenario: Display Station disconnection on OLED
- **WHEN** Station mode loses connection
- **THEN** WiFi icon SHALL indicate disconnected or AP-only state

### Requirement: WiFi Credential Validation
The system SHALL validate WiFi credentials before attempting connection.

#### Scenario: SSID length validation
- **WHEN** SSID is provided
- **THEN** length SHALL be between 1 and 32 characters

#### Scenario: Password length validation for WPA2
- **WHEN** password is provided for WPA2 network
- **THEN** length SHALL be between 8 and 63 characters

#### Scenario: Open network handling
- **WHEN** empty password is provided
- **THEN** system SHALL attempt connection to open network

### Requirement: Connection Timeout Handling
The system SHALL implement timeout for Station mode connection attempts.

#### Scenario: Connection timeout
- **WHEN** connection attempt exceeds 30 seconds
- **THEN** system SHALL abort attempt and return to SoftAP-only mode

#### Scenario: User feedback on timeout
- **WHEN** connection times out
- **THEN** error message SHALL be available via API and optionally displayed

