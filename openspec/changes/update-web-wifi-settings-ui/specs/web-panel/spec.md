## MODIFIED Requirements

### Requirement: WiFi Provisioning Interface
The system SHALL provide dedicated WiFi configuration page with network scanning capability, connection status, IP display, and credential management (save and forget).

#### Scenario: Navigate to WiFi setup page
- **WHEN** user clicks "Connect WiFi" or "Change WiFi" button in main page
- **THEN** system SHALL navigate to dedicated WiFi setup page (wifi-setup.html) in the web panel

#### Scenario: WiFi network scanning
- **WHEN** WiFi setup page loads
- **THEN** system SHALL automatically scan for available WiFi networks and display list

#### Scenario: Network list display
- **WHEN** network scan completes
- **THEN** UI SHALL display list of available networks with:
  - SSID
  - signal strength indicator (e.g., 3–4 level icon based on RSSI)
  - lock icon for secured networks

#### Scenario: Network selection and SSID auto-fill
- **WHEN** user clicks on a network in the list
- **THEN** SSID field SHALL be automatically filled with selected network name

#### Scenario: Auto-fill saved password (optional)
- **WHEN** user selects a network that was previously saved
- **THEN** password field MAY be automatically filled with saved password (without ever displaying raw password text from backend)

#### Scenario: WiFi SSID input
- **WHEN** user enters WiFi SSID in form
- **THEN** input SHALL accept 1-32 character SSID

#### Scenario: WiFi password input
- **WHEN** user enters WiFi password
- **THEN** input SHALL be masked and accept 8-63 characters

#### Scenario: WiFi connect button
- **WHEN** user clicks "Save WiFi settings" (connect) button with valid credentials
- **THEN** API call SHALL provision WiFi and display connection status
- **AND** button SHALL be temporarily disabled and loading indicator displayed while request is in progress

#### Scenario: WiFi connection feedback and IP display
- **WHEN** WiFi connection succeeds
- **THEN** success message and assigned Station IP address SHALL be displayed on the WiFi setup page
- **AND** connection status + IP SHALL also be available on the main page as defined in "Main Page WiFi Status Display"

#### Scenario: WiFi connection failure
- **WHEN** WiFi connection fails
- **THEN** error message with failure reason (e.g., invalid password, timeout, SSID not found) SHALL be displayed
- **AND** user SHALL remain on WiFi setup page to retry

#### Scenario: Refresh network list
- **WHEN** user clicks refresh button on WiFi setup page
- **THEN** system SHALL re-scan for available networks and update list

#### Scenario: Forget WiFi configuration
- **WHEN** user clicks "Forget WiFi" button on WiFi setup page and confirms dialog
- **THEN** API call SHALL delete stored WiFi credentials from device
- **AND** Station mode SHALL disconnect (if connected) and device SHALL remain reachable via SoftAP
- **AND** SSID and password fields in UI SHALL be cleared

#### Scenario: WiFi page connection status header
- **WHEN** WiFi setup page is open
- **THEN** header section SHALL display current WiFi mode and Station status (disconnected/connecting/connected) and Station IP when available

#### Scenario: Back to main page
- **WHEN** user clicks "Back" button in WiFi setup page
- **THEN** system SHALL navigate back to main page without changing WiFi configuration (unless user explicitly saved or forgot WiFi settings)

