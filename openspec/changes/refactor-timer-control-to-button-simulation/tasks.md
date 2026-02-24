## 1. Backend Changes (C++)

- [x] 1.1 Add button simulation API endpoints (`/api/timer/up` and `/api/timer/down`) in `WebServer.cpp`
- [x] 1.2 Expose `ButtonManager` button handlers for API access (or create wrapper methods)
- [x] 1.3 Remove direct timer duration setting API (`/api/timer` POST with duration_min)
- [x] 1.4 Update WebSocket to send timer duration changes in real-time (similar to temperature)
- [x] 1.5 Update WebSocket snapshot to include timer duration (similar to temperature setpoint)
- [x] 1.6 Ensure timer duration is sent whenever it changes (from button press or API)

## 2. Frontend Changes (JavaScript)

- [x] 2.1 Remove `startTimerCountdown()` and `stopTimerCountdown()` functions from `app.js`
- [x] 2.2 Remove `timerInterval` variable and countdown logic
- [x] 2.3 Update `updateTimerDisplay()` to show duration in minutes (not countdown)
- [x] 2.4 Change timer button event listeners to call button simulation API (`/api/timer/up` and `/api/timer/down`)
- [x] 2.5 Update `updateUI()` to handle timer duration from WebSocket (similar to temperature setpoint)
- [x] 2.6 Remove timer countdown display logic and interval management

## 3. HTML Updates

- [x] 3.1 Update timer display element to show duration instead of countdown format (JavaScript handles display update)
- [x] 3.2 Update range info text if needed (should already show correct range) - Updated to "10 دقیقه - 8 ساعت"

## 4. Testing

- [x] 4.1 Test timer duration reading on initial page load
- [x] 4.2 Test timer up/down buttons in web application
- [x] 4.3 Test real-time timer duration updates via WebSocket
- [x] 4.4 Test timer duration changes from hardware buttons appear in web application
- [x] 4.5 Verify timer step size and limits are controlled only in C++ code
