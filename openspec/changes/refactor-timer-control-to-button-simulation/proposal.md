## Why
Currently, timer control in web application uses direct API calls to set timer duration, and timer countdown is handled client-side. This creates inconsistency with hardware button behavior and requires changes in multiple places (C++ and JavaScript) when timer configuration changes. Refactoring to simulate hardware button presses from web application will centralize all timer logic in C++ code, making configuration changes (step size, min/max limits) only require C++ modifications.

## What Changes
- **BREAKING**: Remove client-side timer countdown in web application
- **BREAKING**: Change timer API from direct duration setting to button simulation (up/down)
- Timer duration value will be read on initial page load (similar to temperature setpoint)
- Timer duration changes will be sent via WebSocket in real-time (similar to temperature updates)
- Web application timer buttons will simulate hardware button presses via API
- All timer configuration (step size, min/max) will be centralized in C++ code

## Impact
- Affected specs: 
  - `timer-management` - Remove countdown requirement, add button simulation
  - `web-panel` - Remove client-side countdown, change button behavior
  - `websocket-realtime` - Add timer duration to real-time updates
  - `rest-api` - Replace timer duration API with button simulation API
  - `button-interface` - Add API endpoints for button simulation
- Affected code:
  - `src/server/WebServer.cpp` - Replace `/api/timer` with `/api/timer/up` and `/api/timer/down`
  - `src/buttons/ButtonManager.cpp` - Expose button handlers for API calls
  - `data/app.js` - Remove countdown logic, change to button simulation calls
  - `data/index.html` - Update timer display to show duration (not countdown)
