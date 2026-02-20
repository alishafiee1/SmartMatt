## MODIFIED Requirements

### Requirement: Timer Control Interface
The system SHALL provide timer duration control interface that simulates hardware button presses. Timer duration SHALL be displayed (not countdown) and updated in real-time via WebSocket.

#### Scenario: Timer increment button
- **WHEN** user clicks timer "+" button
- **THEN** API call SHALL simulate hardware timer up button press
- **AND** timer duration SHALL increase by BTN_TIMER_INCREMENT (defined in C++)

#### Scenario: Timer decrement button
- **WHEN** user clicks timer "-" button
- **THEN** API call SHALL simulate hardware timer down button press
- **AND** timer duration SHALL decrease by BTN_TIMER_INCREMENT (defined in C++)

#### Scenario: Timer duration display
- **WHEN** timer duration is received from WebSocket
- **THEN** UI SHALL display duration in minutes format (e.g., "150 دقیقه")
- **AND** display SHALL update immediately when duration changes

#### Scenario: Timer duration on initial load
- **WHEN** web panel loads and WebSocket connects
- **THEN** UI SHALL display timer duration from initial snapshot (similar to temperature setpoint)

#### Scenario: Real-time timer duration updates
- **WHEN** timer duration changes (from hardware button or web button)
- **THEN** UI SHALL receive duration update via WebSocket and update display immediately

## REMOVED Requirements

### Requirement: Client-Side Timer Countdown
**Reason**: Timer countdown is now handled server-side only. Web application displays timer duration, not countdown.

**Migration**: 
- Remove `startTimerCountdown()` and `stopTimerCountdown()` functions
- Remove `timerInterval` variable
- Update `updateTimerDisplay()` to show duration only
- Remove countdown logic from `updateUI()`

### Requirement: Timer Countdown Display
**Reason**: Timer countdown display removed. System now displays timer duration only.

**Migration**: Display timer duration in minutes format (e.g., "150 دقیقه") instead of HH:MM:SS countdown.
