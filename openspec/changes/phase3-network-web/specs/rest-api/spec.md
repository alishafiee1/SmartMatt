# REST API Specification

## ADDED Requirements

### Requirement: HTTP Server Initialization
The system SHALL initialize HTTP server on port 80 accessible via both SoftAP and Station interfaces.

#### Scenario: HTTP server startup
- **WHEN** network is initialized
- **THEN** HTTP server SHALL start on port 80

#### Scenario: Access via SoftAP
- **WHEN** client connects to SoftAP and requests http://192.168.4.1
- **THEN** HTTP server SHALL respond with web panel or API

#### Scenario: Access via Station
- **WHEN** Station is connected and client requests http://<station_ip>
- **THEN** HTTP server SHALL respond with web panel or API

### Requirement: Heating Control API
The system SHALL provide REST API endpoints for heating system control.

#### Scenario: Enable heating via API
- **WHEN** POST /api/heating/enable is requested
- **THEN** system SHALL enable heating and return JSON success response

#### Scenario: Disable heating via API
- **WHEN** POST /api/heating/disable is requested
- **THEN** system SHALL disable heating and return JSON success response

#### Scenario: Heating control response
- **WHEN** heating control API returns
- **THEN** response SHALL include success status and current heating state

### Requirement: Temperature Setpoint API
The system SHALL provide REST API endpoint for temperature setpoint configuration.

#### Scenario: Set temperature via API
- **WHEN** POST /api/temperature with valid setpoint (20-45°C)
- **THEN** system SHALL update setpoint and return JSON success response

#### Scenario: Invalid temperature rejection
- **WHEN** POST /api/temperature with setpoint outside 20-45°C range
- **THEN** API SHALL return HTTP 400 with error message

#### Scenario: Temperature API response
- **WHEN** temperature API succeeds
- **THEN** response SHALL include success status and confirmed setpoint value

### Requirement: Timer Configuration API
The system SHALL provide REST API endpoint for timer duration configuration.

#### Scenario: Set timer via API
- **WHEN** POST /api/timer with valid duration (15 min to 8 hours)
- **THEN** system SHALL update timer duration and return JSON success response

#### Scenario: Invalid timer duration rejection
- **WHEN** POST /api/timer with duration outside valid range
- **THEN** API SHALL return HTTP 400 with error message

#### Scenario: Timer API response
- **WHEN** timer API succeeds
- **THEN** response SHALL include success status and confirmed duration

### Requirement: System Status API
The system SHALL provide REST API endpoint for complete system status query.

#### Scenario: Query system status
- **WHEN** GET /api/status is requested
- **THEN** API SHALL return JSON with all sensor readings, states, and configuration

#### Scenario: Status includes all fields
- **WHEN** status response is sent
- **THEN** JSON SHALL include temperatures, humidity, setpoint, timer, heating state, and WiFi info

### Requirement: WiFi Provisioning API
The system SHALL provide REST API endpoint for WiFi network configuration.

#### Scenario: Provision WiFi credentials
- **WHEN** POST /api/wifi with valid SSID and password
- **THEN** system SHALL validate, attempt connection, and return status response

#### Scenario: WiFi provisioning validation
- **WHEN** WiFi credentials are invalid (length, format)
- **THEN** API SHALL return HTTP 400 with specific error message

#### Scenario: WiFi provisioning success response
- **WHEN** WiFi connection succeeds
- **THEN** API SHALL return success with connection status and assigned IP

#### Scenario: WiFi provisioning failure response
- **WHEN** WiFi connection fails
- **THEN** API SHALL return error with failure reason

### Requirement: WiFi Status API
The system SHALL provide REST API endpoint for WiFi status query.

#### Scenario: Query WiFi status
- **WHEN** GET /api/wifi is requested
- **THEN** API SHALL return WiFi mode, connection state, SSID (no password), and IP addresses

#### Scenario: WiFi status includes security
- **WHEN** WiFi status is queried
- **THEN** response SHALL NOT include WiFi password

### Requirement: SoftAP Control API
The system SHALL provide REST API endpoint for SoftAP enable/disable control.

#### Scenario: Enable SoftAP via API
- **WHEN** POST /api/softap with enabled=true
- **THEN** system SHALL enable SoftAP and return success response

#### Scenario: Disable SoftAP via API
- **WHEN** POST /api/softap with enabled=false and Station is connected
- **THEN** system SHALL disable SoftAP and return success response

#### Scenario: Prevent SoftAP lockout
- **WHEN** POST /api/softap with enabled=false and Station is NOT connected
- **THEN** API SHALL return HTTP 400 error preventing lockout

### Requirement: JSON Request/Response Format
The system SHALL use JSON format for all API requests and responses.

#### Scenario: JSON request parsing
- **WHEN** POST request with JSON body is received
- **THEN** server SHALL parse JSON and validate required fields

#### Scenario: JSON success response
- **WHEN** API operation succeeds
- **THEN** response SHALL be JSON with success=true and relevant data

#### Scenario: JSON error response
- **WHEN** API operation fails
- **THEN** response SHALL be JSON with success=false and error description

### Requirement: HTTP Status Codes
The system SHALL return appropriate HTTP status codes for API responses.

#### Scenario: Successful request status
- **WHEN** API request succeeds
- **THEN** HTTP status SHALL be 200 OK

#### Scenario: Invalid request status
- **WHEN** API request has invalid parameters
- **THEN** HTTP status SHALL be 400 Bad Request

#### Scenario: Server error status
- **WHEN** internal error occurs during API processing
- **THEN** HTTP status SHALL be 500 Internal Server Error

### Requirement: CORS Headers
The system SHALL include CORS headers for browser cross-origin compatibility.

#### Scenario: CORS headers on response
- **WHEN** any API response is sent
- **THEN** Access-Control-Allow-Origin header SHALL be included

### Requirement: API Input Validation
The system SHALL validate all API inputs before processing.

#### Scenario: Required field validation
- **WHEN** POST request is missing required field
- **THEN** API SHALL return 400 error indicating missing field

#### Scenario: Data type validation
- **WHEN** POST request field has incorrect data type
- **THEN** API SHALL return 400 error indicating type mismatch

#### Scenario: Range validation
- **WHEN** numeric value is outside valid range
- **THEN** API SHALL return 400 error with acceptable range

### Requirement: API Response Time
The system SHALL process API requests with low latency.

#### Scenario: API response latency
- **WHEN** API request is received
- **THEN** response SHALL be sent within 100ms under normal conditions

### Requirement: Static File Serving
The system SHALL serve web panel static files (HTML, CSS, JS) from HTTP server.

#### Scenario: Serve index HTML
- **WHEN** GET / or GET /index.html is requested
- **THEN** server SHALL serve web panel HTML file

#### Scenario: Serve CSS and JavaScript
- **WHEN** GET /style.css or GET /app.js is requested
- **THEN** server SHALL serve corresponding static files

#### Scenario: Correct content types
- **WHEN** static file is served
- **THEN** Content-Type header SHALL match file type (text/html, text/css, application/javascript)

