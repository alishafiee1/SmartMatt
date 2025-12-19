# Phase 1 Testing Guide

## Prerequisites

### Hardware Setup
1. **ESP32 Board**: Connect via USB
2. **DHT22 Sensor**: 
   - VCC → 3.3V (or 5V depending on module)
   - GND → GND
   - DATA → GPIO 4
   - Pull-up resistor (usually included on module)

3. **DS18B20 Sensor**:
   - VCC → 3.3V
   - GND → GND
   - DATA → GPIO 5
   - **IMPORTANT**: 4.7kΩ pull-up resistor between DATA and VCC

4. **Relay Module**:
   - VCC → 5V (or separate power if high current)
   - GND → GND
   - IN → GPIO 2
   - COM/NO/NC → Heating element circuit (follow safety guidelines!)

### Software Setup
1. Install PlatformIO
2. Clone/download project
3. Build and upload:
```bash
cd SmartMatt
pio run --target upload
pio device monitor
```

## Test Scenarios

### Test 1: System Initialization
**Purpose**: Verify all modules initialize correctly

**Steps**:
1. Open serial monitor (115200 baud)
2. Press reset button on ESP32
3. Observe boot sequence

**Expected Output**:
```
=====================================
     RODI Smart Mattress System     
    Phase 1: Core Hardware Control   
=====================================

[SafetyMonitor] Last reset reason: Power-on reset
[SafetyMonitor] Initializing watchdog timer (10 seconds timeout)...
[SafetyMonitor] Safety monitor initialized - Watchdog ENABLED

[SettingsStorage] Preferences initialized
[SettingsStorage] Loaded settings - Temp: 25.0°C, Timer: 15 min, Heating: OFF

[RoomSensor] DHT22 initialized successfully

[MattressSensor] DS18B20 initialized successfully (1 device(s))

[HeatingController] Initialized - Heating OFF (safe state)

[HeatingTimer] Timer initialized

=== System Initialization Complete ===
Phase 1: Core Hardware Control System - READY

System is in safe state:
  - Heating: OFF (safe boot state)
  - Temperature Setpoint: 25.0°C
  - Timer Duration: 15 minutes
```

**Pass Criteria**:
- ✅ All modules initialized without errors
- ✅ Watchdog enabled
- ✅ Sensors detected
- ✅ Heating OFF on boot

---

### Test 2: Sensor Readings
**Purpose**: Verify accurate temperature and humidity readings

**Steps**:
1. Monitor serial output for 30 seconds
2. Compare readings with reference thermometer
3. Check for consistent readings

**Expected Output**:
```
[RoomSensor] Temp: 23.5°C, Humidity: 45.2%
[MattressSensor] Mattress Temp: 22.35°C

╔════════════════════════════════════════════════════════════╗
║                    SYSTEM STATUS                           ║
╠════════════════════════════════════════════════════════════╣
║ Room Temp: 23.5°C | Humidity: 45.2% | Status: OK          ║
║ Mattress Temp: 22.35°C | Status: OK                        ║
╚════════════════════════════════════════════════════════════╝
```

**Pass Criteria**:
- ✅ Room temperature within ±2°C of reference
- ✅ Mattress temperature within ±1°C of reference
- ✅ Humidity between 20-80% (depending on environment)
- ✅ Both sensors show "Status: OK"
- ✅ No error messages

---

### Test 3: Settings Persistence
**Purpose**: Verify NVS storage works across reboots

**Manual Code Modification Required** (in `setup()` function):
```cpp
// Add after settings.begin()
settings.setTemperatureSetpoint(30.0f);
settings.setTimerDuration(60);  // 60 minutes
settings.save();
```

**Steps**:
1. Modify code as above
2. Upload and observe serial output
3. Note the saved values
4. Press reset button (do NOT re-upload)
5. Verify values are restored

**Expected Output** (after reset):
```
[SettingsStorage] Loaded settings - Temp: 30.0°C, Timer: 60 min, Heating: OFF
```

**Pass Criteria**:
- ✅ Settings saved successfully
- ✅ Settings restored after reset
- ✅ Values match what was saved

---

### Test 4: Heating Control - Hysteresis Algorithm
**Purpose**: Verify heating control with hysteresis

**Setup**:
- Setpoint: 25°C
- Hysteresis: 2°C
- Initial mattress temp: ~22°C (room temperature)

**Manual Code Modification** (in `setup()`, after initialization):
```cpp
// Enable heating for testing
heatingController.enable();
heatingController.setSetpoint(25.0f);
heatingTimer.start(30);  // 30 minute timer
```

**Steps**:
1. Monitor serial output
2. Observe heating state changes
3. Track temperature progression

**Expected Behavior**:
```
Initial State:
- Mattress Temp: 22.0°C
- Setpoint: 25.0°C
- Heating: OFF

When temp < 23.0°C (25 - 2):
[HeatingController] Heating ON - Temp: 22.50°C < (25.0°C - 2.0°C)
- Relay turns ON
- Element heating status: ON

As temperature rises:
- Heating stays ON until reaching 25.0°C

When temp >= 25.0°C:
[HeatingController] Heating OFF - Temp: 25.10°C >= 25.0°C
- Relay turns OFF
- Element heating status: OFF
```

**Pass Criteria**:
- ✅ Heating turns ON when temp < (setpoint - hysteresis)
- ✅ Heating turns OFF when temp >= setpoint
- ✅ No rapid cycling (ON/OFF within seconds)
- ✅ Relay GPIO changes state correctly

**Safety Note**: Monitor temperature carefully. Disconnect heating element if temperature rises unexpectedly.

---

### Test 5: Timer Auto-Shutoff
**Purpose**: Verify timer expires and disables heating

**Manual Code Modification** (in `setup()`):
```cpp
// Start heating with 1-minute timer for quick testing
heatingController.enable();
heatingTimer.start(1);  // 1 minute
settings.setHeatingEnabled(true);
```

**Steps**:
1. Upload and start monitoring
2. Observe timer countdown
3. Wait for timer expiration
4. Verify heating disables

**Expected Output**:
```
[HeatingTimer] Timer started - Duration: 1 minutes (60 seconds)

... (countdown in status updates) ...

║ Timer: RUNNING | Remaining: 0:45                           ║

... (when timer expires) ...

[HeatingTimer] *** TIMER EXPIRED - AUTO SHUTOFF ***

╔════════════════════════════════════════════════════════════╗
║              ⏰ TIMER EXPIRED - AUTO SHUTOFF               ║
╚════════════════════════════════════════════════════════════╝

[HeatingController] Heating control DISABLED
[HeatingController] Heating FORCED OFF
✓ Heating disabled
✓ Settings saved
✓ System in safe state
```

**Pass Criteria**:
- ✅ Timer counts down correctly
- ✅ Timer expiration triggers auto-shutoff
- ✅ Heating disabled automatically
- ✅ Settings saved with heating=OFF

---

### Test 6: Sensor Failure Detection (DS18B20)
**Purpose**: Verify emergency shutoff on critical sensor failure

**Steps**:
1. Start with heating enabled (use Test 4 setup)
2. While system is running, disconnect DS18B20 DATA wire
3. Observe serial output

**Expected Output**:
```
[MattressSensor] CRITICAL: Failed to read DS18B20 after 3 attempts
[MattressSensor] ERROR: Sensor disconnected
[SafetyMonitor] *** WARNING: Critical sensor (mattress) unhealthy ***
[HeatingController] *** SAFETY SHUTOFF: Sensor unhealthy ***
[HeatingController] Heating FORCED OFF
```

**Pass Criteria**:
- ✅ Sensor failure detected within 3 read attempts
- ✅ Heating immediately disabled
- ✅ Safety monitor flags unsafe state
- ✅ Warning messages displayed
- ✅ System does NOT crash

**Recovery Test**:
- Reconnect DS18B20
- Verify sensor recovery message
- System should return to safe state

---

### Test 7: Watchdog Timer
**Purpose**: Verify watchdog resets system on hang

**⚠️ WARNING**: This test intentionally crashes the system!

**Manual Code Modification** (add to `loop()` for testing):
```cpp
// TEMPORARY TEST CODE - REMOVE AFTER TESTING
static bool watchdogTestDone = false;
if (!watchdogTestDone && millis() > 30000) {  // After 30 seconds
    Serial.println("*** WATCHDOG TEST: Simulating infinite loop ***");
    watchdogTestDone = true;
    while(true) {
        // Infinite loop - do NOT feed watchdog
        delay(100);
    }
}
```

**Steps**:
1. Upload code with test modification
2. Monitor serial output
3. Wait for test to trigger (30 seconds)
4. Observe system behavior

**Expected Output**:
```
*** WATCHDOG TEST: Simulating infinite loop ***

... (system hangs for ~10 seconds) ...

--- System resets ---

[SafetyMonitor] Last reset reason: *** TASK WATCHDOG TIMEOUT ***
[HeatingController] Initialized - Heating OFF (safe state)
```

**Pass Criteria**:
- ✅ System resets after ~10 seconds
- ✅ Reset reason indicates watchdog timeout
- ✅ System boots in safe state (heating OFF)
- ✅ All modules reinitialize correctly

**IMPORTANT**: Remove test code after verification!

---

### Test 8: Temperature Range Validation
**Purpose**: Verify rejection of out-of-range values

**Manual Code Modification** (in `setup()`):
```cpp
// Test invalid values
settings.setTemperatureSetpoint(50.0f);  // Above max (45°C)
settings.setTemperatureSetpoint(15.0f);  // Below min (20°C)
settings.setTimerDuration(10);           // Below min (15 min)
settings.setTimerDuration(500);          // Above max (480 min)

// Test valid values
settings.setTemperatureSetpoint(30.0f);  // Valid
settings.setTimerDuration(60);           // Valid
```

**Expected Output**:
```
[SettingsStorage] ERROR: Invalid temperature 50.0°C (range: 20-45°C)
[SettingsStorage] ERROR: Invalid temperature 15.0°C (range: 20-45°C)
[SettingsStorage] ERROR: Invalid timer 10 min (range: 15-480 min)
[SettingsStorage] ERROR: Invalid timer 500 min (range: 15-480 min)
[SettingsStorage] Temperature setpoint set to 30.0°C
[SettingsStorage] Timer duration set to 60 minutes
```

**Pass Criteria**:
- ✅ Invalid values rejected with error messages
- ✅ Valid values accepted
- ✅ System remains stable after invalid inputs

---

## Performance Tests

### CPU Usage
Monitor over 5 minutes:
- Expected: <5% CPU usage
- Verify: No lag in serial output
- Check: Sensor readings stay current

### Memory Usage
Check compilation output:
```
RAM:   [=         ]   6.6% (used 21688 bytes from 327680 bytes)
Flash: [==        ]  22.4% (used 293821 bytes from 1310720 bytes)
```

**Pass Criteria**:
- ✅ RAM usage < 10%
- ✅ Flash usage < 30%
- ✅ Plenty of headroom for Phase 2

---

## Safety Checklist

Before deploying to actual hardware:

- [ ] All sensors read accurate values
- [ ] Heating control responds correctly to setpoint
- [ ] Timer auto-shutoff works reliably
- [ ] Sensor failure triggers immediate shutoff
- [ ] Watchdog timer works (system recovers from hang)
- [ ] Settings persist across power cycles
- [ ] Temperature limits cannot be exceeded
- [ ] Heating is OFF on every boot
- [ ] Relay operates safely (no sparks, proper isolation)
- [ ] No unusual warming of components
- [ ] Serial output shows no recurring errors

---

## Troubleshooting

### DHT22 Not Detected
- Check wiring (VCC, GND, DATA)
- Verify pull-up resistor (if not on module)
- Try different GPIO pin
- Check power supply (3.3V or 5V depending on module)

### DS18B20 Not Detected
- **Most common**: Missing 4.7kΩ pull-up resistor
- Check wiring
- Verify sensor is genuine (many clones exist)
- Test with simple DS18B20 example first

### Relay Not Switching
- Check GPIO 2 connection
- Verify relay module power
- Test relay with manual HIGH/LOW
- Check relay isolation (optocoupler)

### Watchdog Resets Too Often
- Increase `WATCHDOG_TIMEOUT_SEC` in RodiConfig.h
- Check for blocking code in loop()
- Verify sensors don't timeout too long

### Settings Not Persisting
- Check NVS partition in platformio.ini
- Try `settings.resetToDefaults()`
- Erase flash: `pio run --target erase`

---

## Success Criteria Summary

Phase 1 is complete when:
1. ✅ All 8 test scenarios pass
2. ✅ Performance criteria met
3. ✅ Safety checklist complete
4. ✅ No recurring errors in serial output
5. ✅ System stable for >1 hour continuous operation

---

**Next**: Phase 2 will add button and OLED display interface!


