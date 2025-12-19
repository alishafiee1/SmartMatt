# Phase 2 Implementation Tasks

## 1. Button Hardware Integration
- [ ] 1.1 Define button GPIO pins in configuration header
- [ ] 1.2 Configure GPIO pins with internal pull-up resistors
- [ ] 1.3 Test raw button press detection on all buttons
- [ ] 1.4 Document button hardware connections

## 2. Button Debouncing System
- [ ] 2.1 Create ButtonDebounce class with configurable debounce time (50ms default)
- [ ] 2.2 Implement press detection (rising/falling edge)
- [ ] 2.3 Implement long press detection (configurable threshold, 3s default)
- [ ] 2.4 Implement hold-for-acceleration (repeat rate increases over time)
- [ ] 2.5 Test debouncing with physical buttons under various conditions

## 3. Button Event Handling
- [ ] 3.1 Create button event system (press, long-press, release, hold-repeat)
- [ ] 3.2 Implement temperature up/down button handlers (±1°C per press)
- [ ] 3.3 Implement timer up/down button handlers (±15 min per press)
- [ ] 3.4 Implement power button short press handler (toggle heating)
- [ ] 3.5 Implement power button long press handler (3s = toggle SoftAP, Phase 3)
- [ ] 3.6 Add acceleration logic for held buttons (faster changes)
- [ ] 3.7 Test all button combinations and edge cases

## 4. OLED Display Driver
- [ ] 4.1 Initialize I2C bus (SDA=GPIO21, SCL=GPIO22)
- [ ] 4.2 Integrate SSD1306 OLED library
- [ ] 4.3 Create DisplayManager class for OLED control
- [ ] 4.4 Implement display initialization and test pattern
- [ ] 4.5 Create font management (small, medium, large fonts)
- [ ] 4.6 Test basic text and graphics rendering

## 5. Boot Sequence Animation
- [ ] 5.1 Create Rodiset.ir scrolling/animation (3 seconds)
- [ ] 5.2 Create RODI logo large font display (3 seconds)
- [ ] 5.3 Implement smooth transitions between boot screens
- [ ] 5.4 Add boot progress indication
- [ ] 5.5 Test complete boot sequence timing

## 6. Main Display Layout
- [ ] 6.1 Design 3-column layout (left: setpoint, center: room temp/humidity, right: timer)
- [ ] 6.2 Implement room temperature display (large font, center)
- [ ] 6.3 Implement room humidity display (with icon, center)
- [ ] 6.4 Implement setpoint display (left side, medium font)
- [ ] 6.5 Implement timer remaining display (right side, small font, MM:SS format)
- [ ] 6.6 Add temperature and humidity icons/symbols
- [ ] 6.7 Test layout at various data values

## 7. Status Icons
- [ ] 7.1 Create WiFi status icon (top-right corner)
- [ ] 7.2 Implement WiFi icon states (connected, AP mode, disconnected)
- [ ] 7.3 Create heating element icon (top-left corner)
- [ ] 7.4 Implement heating icon (show when element ON)
- [ ] 7.5 Test icon visibility and positioning
- [ ] 7.6 Add sensor error indicators (if needed)

## 8. Display Animations
- [ ] 8.1 Create zoom animation for changing values
- [ ] 8.2 Implement animation trigger on button press
- [ ] 8.3 Create reverse zoom animation for return to normal
- [ ] 8.4 Implement 3-second idle timeout for animation reset
- [ ] 8.5 Ensure smooth frame rate (30+ FPS) during animations
- [ ] 8.6 Test animations for temperature and timer changes

## 9. Display Update Logic
- [ ] 9.1 Implement periodic display refresh (every 1 second)
- [ ] 9.2 Add display dirty flag for on-demand updates
- [ ] 9.3 Optimize display updates (only redraw changed regions if possible)
- [ ] 9.4 Handle display update during animations
- [ ] 9.5 Test display responsiveness during system load

## 10. Integration with Phase 1
- [ ] 10.1 Connect button handlers to heating controller APIs
- [ ] 10.2 Connect display to sensor reading APIs
- [ ] 10.3 Connect display to timer remaining time API
- [ ] 10.4 Connect display to heating element state
- [ ] 10.5 Test end-to-end: button press → Phase 1 action → display update

## 11. Error Display
- [ ] 11.1 Create error message display system
- [ ] 11.2 Display DS18B20 sensor error ("Check mattress sensor")
- [ ] 11.3 Display DHT22 sensor warning
- [ ] 11.4 Display settings storage errors
- [ ] 11.5 Test error display persistence and clearing

## 12. Testing & Validation
- [ ] 12.1 Test all button press patterns (short, long, hold)
- [ ] 12.2 Verify temperature setpoint changes (20-45°C range)
- [ ] 12.3 Verify timer changes (15min-8hr range)
- [ ] 12.4 Test power button toggle functionality
- [ ] 12.5 Verify boot sequence completes correctly
- [ ] 12.6 Test display animations for smoothness
- [ ] 12.7 Validate icon display under all states
- [ ] 12.8 Test display readability at various angles
- [ ] 12.9 Long-term stability test (24 hour operation)

