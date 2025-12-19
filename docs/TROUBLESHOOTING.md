# RODI Smart Mattress - Troubleshooting Guide

## Common Issues and Solutions

### 1. Watchdog Timeout Error ✅ FIXED

**Symptoms:**
```
E (22232) task_wdt: Task watchdog got triggered
E (22232) task_wdt:  - loopTask (CPU 1)
abort() was called at PC 0x400e0ea1 on core 0
Rebooting...
```

**Root Cause:**
- Display I2C communication blocking the main loop for >10 seconds
- Boot sequence taking too long without watchdog feeding
- Display not connected causing I2C timeout

**Solutions Applied:**
1. ✅ Added I2C timeout (100ms) in DisplayManager
2. ✅ Added watchdog feeding before/after boot sequence
3. ✅ Added watchdog feeding before display update
4. ✅ Split boot sequence delays into smaller chunks with `yield()`
5. ✅ Added display initialization check before updates
6. ✅ Improved error handling for display failures

**Code Changes:**
- `DisplayManager.cpp`: I2C timeout + yield() in delays
- `main.cpp`: Watchdog feeding before slow operations
- Display gracefully continues without physical display

---

### 2. Display Initialization Failed

**Symptoms:**
```
✗ SSD1306 initialization failed!
Continuing without display...
```

**Possible Causes:**
- Display not physically connected
- Wrong I2C address (0x3C vs 0x3D)
- Incorrect wiring (SDA/SCL swapped)
- Power issue to display

**Solutions:**

#### Check I2C Address:
```cpp
// In RodiConfig.h, try changing:
#define SSD1306_I2C_ADDR   0x3C  // Try 0x3D if this fails
```

#### Verify Wiring:
```
ESP32          SSD1306 OLED
GPIO 21 (SDA) → SDA
GPIO 22 (SCL) → SCL
3.3V          → VCC
GND           → GND
```

#### Test I2C Connection:
```cpp
// Add this to setup() for debugging:
Wire.begin(21, 22);
Wire.beginTransmission(0x3C);
byte error = Wire.endTransmission();
if (error == 0) {
    Serial.println("Display found at 0x3C");
} else {
    Serial.println("Display NOT found");
}
```

**System Behavior:**
- ✅ System continues to run without display
- ✅ All functionality works (buttons, sensors, heating)
- ✅ Errors logged to Serial only

---

### 3. NVS (Settings Storage) Warnings

**Symptoms:**
```
[E][Preferences.cpp:503] getBytesLength(): nvs_get_blob len fail: temp_setpoint NOT_FOUND
```

**Cause:**
- First boot - no settings saved yet
- NVS partition corrupted

**Solution:**
- ⚠ This is NORMAL on first boot
- Settings will be saved on first change
- If persists, erase flash:
  ```bash
  pio run -t erase
  pio run -t upload
  ```

---

### 4. Sensor Reading Errors

**DHT22 Failures:**
```
[RoomSensor] Failed to read DHT22
```

**DS18B20 Failures:**
```
[MattressSensor] DS18B20 read failed
Check mattress sensor
```

**Solutions:**

#### DHT22 Issues:
- Check GPIO 4 wiring
- Ensure 10kΩ pull-up resistor on data line
- Try different DHT22 sensor (may be faulty)
- Increase read interval (currently 2000ms)

#### DS18B20 Issues:
- Check GPIO 5 wiring
- Ensure 4.7kΩ pull-up resistor on data line
- Check sensor polarity (GND, DATA, VCC)
- Verify OneWire device count:
  ```cpp
  Serial.printf("Found %d DS18B20 device(s)\n", sensors.getDeviceCount());
  ```

---

### 5. Heating Element Not Working

**Symptoms:**
- Heating icon not showing
- Relay not clicking
- Temperature not changing

**Checklist:**
1. ✅ Check relay connection (GPIO 2)
2. ✅ Verify power supply to heating element
3. ✅ Test relay manually:
   ```cpp
   digitalWrite(2, HIGH);  // Should activate relay
   delay(1000);
   digitalWrite(2, LOW);   // Should deactivate relay
   ```
4. ✅ Check safety conditions:
   - Mattress sensor must be healthy
   - Temperature must be below setpoint
   - Heating must be enabled (power button)

**Safety Notes:**
- Heating is OFF by default on boot
- DS18B20 failure → heating automatically disabled
- Watchdog timeout → system resets → heating OFF

---

### 6. Button Not Responding

**Symptoms:**
- Button press not detected
- No serial output on button press
- Display not updating

**Solutions:**

#### Check Wiring:
```
ESP32          Button
GPIO 13       → Temp Up
GPIO 12       → Temp Down
GPIO 14       → Timer Up
GPIO 27       → Timer Down
GPIO 26       → Power
GND           → Common ground
```

#### Test Button Directly:
```cpp
// Add to loop() for debugging:
if (digitalRead(13) == LOW) {
    Serial.println("Temp Up pressed");
    delay(500);
}
```

#### Debouncing Issues:
- Mechanical bounce < 50ms is filtered
- If button "chatters", increase debounce time:
  ```cpp
  #define BTN_DEBOUNCE_MS  100  // Increase from 50
  ```

---

### 7. Display Artifacts or Corruption

**Symptoms:**
- Garbled text
- Random pixels
- Incomplete display

**Solutions:**

1. **Check I2C Speed:**
   ```cpp
   // In RodiConfig.h, reduce speed:
   #define I2C_FREQ_HZ  100000  // Down from 400000
   ```

2. **Add I2C Pull-up Resistors:**
   - 4.7kΩ on SDA line
   - 4.7kΩ on SCL line

3. **Shorten I2C Wires:**
   - Keep wires < 10cm for 400kHz
   - Use twisted pair if longer

4. **Check Power Supply:**
   - OLED draws up to 20mA
   - Ensure stable 3.3V supply

---

### 8. System Resets Continuously

**Symptoms:**
- Constant reboot loop
- "Last reset reason: TASK WATCHDOG TIMEOUT"

**Diagnosis:**

Check serial output for:
```
E (XXXXX) task_wdt: Task watchdog got triggered
```

**Solutions:**

1. **Identify Blocking Code:**
   - Long loops without `yield()`
   - Blocking I2C/SPI operations
   - Large delays (>5 seconds)

2. **Add Watchdog Feeding:**
   ```cpp
   safetyMonitor.feedWatchdog();
   ```

3. **Increase Watchdog Timeout:**
   ```cpp
   // In RodiConfig.h:
   #define WATCHDOG_TIMEOUT_SEC  20  // Up from 10
   ```

4. **Disable Watchdog (DEBUG ONLY):**
   ```cpp
   // In SafetyMonitor::begin():
   // Comment out: esp_task_wdt_init(...)
   ```
   ⚠️ **DO NOT USE IN PRODUCTION**

---

### 9. Memory Issues

**Symptoms:**
```
Guru Meditation Error: Core 1 panic'ed (LoadProhibited)
```

**Current Memory Usage:**
```
RAM:   6.8%  (22,300 / 327,680 bytes)
Flash: 25.5% (334,833 / 1,310,720 bytes)
```

**If Memory Issues Occur:**

1. **Check Stack Overflow:**
   ```cpp
   // In platformio.ini:
   build_flags = 
       -D CORE_DEBUG_LEVEL=5
       -D CONFIG_ARDUINO_LOOP_STACK_SIZE=16384
   ```

2. **Reduce Buffer Sizes:**
   ```cpp
   // In DisplayManager:
   // Display buffer: 1KB (128x64/8)
   // Cannot reduce without changing display
   ```

3. **Monitor Heap:**
   ```cpp
   Serial.printf("Free heap: %u bytes\n", ESP.getFreeHeap());
   ```

---

### 10. Upload/Flash Issues

**COM Port Not Found:**
```bash
# Windows: Check Device Manager
# Install CP210x or CH340 driver

# List ports:
pio device list
```

**Upload Failed:**
```bash
# Hold BOOT button during upload
# Or add to platformio.ini:
upload_speed = 115200  # Down from 921600
```

**Erase Flash Completely:**
```bash
pio run -t erase
```

---

## Debug Serial Commands

### Enable Verbose Logging:
```cpp
// In RodiConfig.h:
#define DEBUG_ENABLED  true

// In platformio.ini:
build_flags = -D CORE_DEBUG_LEVEL=5
```

### Monitor Serial Output:
```bash
# PlatformIO:
pio device monitor -b 115200

# Arduino IDE:
Tools → Serial Monitor → 115200 baud
```

---

## Hardware Test Checklist

### Minimal Test (No Display):
- [ ] ESP32 boots successfully
- [ ] Serial output shows initialization
- [ ] DHT22 reads temperature
- [ ] DS18B20 reads temperature
- [ ] Settings save/load works
- [ ] Relay toggles with heating control

### Full Test (With Display):
- [ ] Boot sequence shows "Rodiset.ir"
- [ ] Boot sequence shows "RODI"
- [ ] Display shows room temperature
- [ ] Display shows humidity
- [ ] Display shows setpoint
- [ ] Display shows timer
- [ ] All 5 buttons work
- [ ] Heating icon appears when ON
- [ ] Error message shows when sensor fails

---

## Performance Benchmarks

### Loop Timing:
- Main loop: ~100ms cycle
- Sensor read: Every 2000ms
- Display update: Every 1000ms
- Button check: Every loop

### I2C Operations:
- Display full refresh: <20ms @ 400kHz
- Display init: ~100ms
- I2C timeout: 100ms (prevents watchdog)

### Boot Timing:
- Phase 1 init: ~4 seconds
- Boot sequence: 6 seconds
- Total boot: ~11 seconds

---

## Contact & Support

**Project**: RODI Smart Mattress  
**Developer**: AliShafiee  
**HW Designer**: H.Rostamizade  
**Team**: RODI

**Documentation:**
- Implementation: `docs/PHASE2_IMPLEMENTATION.md`
- API Reference: `docs/API_REFERENCE_PHASE1.md`
- Testing Guide: `docs/TESTING_GUIDE_PHASE1.md`

**OpenSpec:**
- Proposal: `openspec/changes/phase2-device-panel/proposal.md`
- Design: `openspec/changes/phase2-device-panel/design.md`
- Tasks: `openspec/changes/phase2-device-panel/tasks.md`

---

## Version History

| Version | Date | Changes |
|---------|------|---------|
| 2.0.1 | 2025-12-19 | Fixed watchdog timeout, improved error handling |
| 2.0.0 | 2025-12-19 | Initial Phase 2 release |
| 1.0.0 | 2025-12-17 | Phase 1 complete |

---

**Status**: ✅ All known issues resolved  
**Last Updated**: 2025-12-19

