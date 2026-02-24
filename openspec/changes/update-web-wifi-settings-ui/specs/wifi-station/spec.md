## ADDED Requirements

### Requirement: WiFi Credentials Deletion
The system SHALL allow deleting stored WiFi credentials via API and handle Station disconnect safely.

#### Scenario: Delete WiFi credentials via API
- **WHEN** client calls WiFi delete endpoint (e.g., DELETE /api/wifi or equivalent)
- **THEN** system SHALL remove stored SSID and password from NVS

#### Scenario: Station disconnect on credentials delete
- **WHEN** WiFi credentials are deleted while Station mode is connected
- **THEN** system SHALL disconnect from current WiFi network
- **AND** SHALL continue to operate in SoftAP-only or dual-mode according to SoftAP configuration

#### Scenario: WiFi status after credentials delete
- **WHEN** credentials are deleted
- **THEN** subsequent WiFi status query (e.g., GET /api/wifi) SHALL report:
  - Station state as "disconnected"
  - empty or null SSID
  - no Station IP address

#### Scenario: Idempotent delete behavior
- **WHEN** WiFi delete endpoint is called with no stored credentials
- **THEN** system SHALL return success response without error and keep current WiFi state unchanged

