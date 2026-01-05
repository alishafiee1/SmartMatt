# Implementation Tasks

## 1. Data Structures and Constants
- [ ] 1.1 Define layout constants (column positions, divider lines, icon positions)
- [ ] 1.2 Create `BoldAnimation` struct for simple Bold effect
- [ ] 1.3 Create `AnimationContext` struct with three animation slots (setTemp, currentTemp, timer)
- [ ] 1.4 Add member variables to `DisplayManager` class for animation state

## 2. Animation System
- [ ] 2.1 Implement `triggerSetTempBold()` method
- [ ] 2.2 Implement `triggerCurrentTempBold()` method
- [ ] 2.3 Implement `triggerTimerBold()` method
- [ ] 2.4 Implement `updateAnimations()` method (check all animation timers)
- [ ] 2.5 Implement `updateBoldAnimation()` helper method
- [ ] 2.6 Implement `isBoldActive()` helper method

## 3. Drawing Methods - Layout Structure
- [ ] 3.1 Implement `drawVerticalDividers()` - draw lines at X=32 and X=96 (Y: 0-64)
- [ ] 3.2 Implement `drawStatusBar()` - draw 3 icons at top of center column
- [ ] 3.3 Implement `drawBottomBar()` - draw room info at bottom of center column

## 4. Drawing Methods - Icon System
- [ ] 4.1 Update `drawWiFiIcon()` - 8x8 pixel icon at specified position
- [ ] 4.2 Update `drawHeatingIcon()` - 8x8 pixel icon at specified position
- [ ] 4.3 Implement `drawSystemIcon()` - NEW 8x8 pixel icon for system status

## 5. Drawing Methods - Data Display with Bold Support
- [ ] 5.1 Implement `drawSetTempArea()` - left column with set temperature
- [ ] 5.2 Implement `drawSetTempWithBold()` - render with Font Size 1 or 2
- [ ] 5.3 Implement `drawMainDisplayArea()` - center column with current temperature
- [ ] 5.4 Implement `drawCurrentTempWithBold()` - render with Font Size 3 or 4 (largest)
- [ ] 5.5 Implement `drawTimerArea()` - right column with remaining minutes
- [ ] 5.6 Implement `drawTimerWithBold()` - render with Font Size 1 or 2

## 6. Main Layout Integration
- [ ] 6.1 Rewrite `drawMainLayout()` to orchestrate three-column layout
- [ ] 6.2 Update `update()` method to call `updateAnimations()` every cycle
- [ ] 6.3 Integrate Bold state checks in drawing calls
- [ ] 6.4 Update refresh rate management (5 FPS = 200ms interval)

## 7. Helper Methods
- [ ] 7.1 Implement `getTextWidth()` for text centering calculations
- [ ] 7.2 Implement `getTextHeight()` for vertical positioning
- [ ] 7.3 Update `formatTimer()` to return minutes only (remove HH:MM:SS format)
- [ ] 7.4 Implement `centerTextInArea()` for positioning text in specific regions

## 8. ButtonManager Integration (Optional)
- [ ] 8.1 Add callback function pointers to `ButtonManager` class
- [ ] 8.2 Implement `onSetTempChanged()` callback registration
- [ ] 8.3 Implement `onTimerChanged()` callback registration
- [ ] 8.4 Call animation triggers in button event handlers
- [ ] 8.5 Register callbacks in `main.cpp` setup

## 9. Error Display System
- [ ] 9.1 Implement `showMattressSensorError()` method
- [ ] 9.2 Format three-line error message ("check" / "mattress" / "cable")
- [ ] 9.3 Center error message in center column
- [ ] 9.4 Preserve layout structure (dividers, icons) during error
- [ ] 9.5 Clear error message when sensor reconnects

## 10. Testing and Refinement
- [ ] 10.1 Test basic three-column layout rendering
- [ ] 10.2 Test vertical divider lines (full height)
- [ ] 10.3 Test all three status icons (WiFi, Heating, System)
- [ ] 10.4 Test Bold animation for Set Temperature changes
- [ ] 10.5 Test Bold animation for Timer changes
- [ ] 10.6 Test Bold animation for Current Temperature changes
- [ ] 10.7 Test timer format (minutes only)
- [ ] 10.8 Test room information display at bottom of center
- [ ] 10.9 Test mattress sensor error display (three-line message)
- [ ] 10.10 Test heating element auto-disable on sensor error
- [ ] 10.11 Test error message clearing when sensor reconnects
- [ ] 10.12 Verify 500ms animation duration
- [ ] 10.13 Verify 5 FPS refresh rate (200ms interval)

## 11. Cleanup
- [ ] 11.1 Remove old debug layout code
- [ ] 11.2 Remove unused zoom animation code and enums
- [ ] 11.3 Update header file comments
- [ ] 11.4 Verify no memory leaks or performance issues

## 12. Documentation
- [ ] 12.1 Update code comments to reference `docs/UI_DESIGN_SIMPLE.md`
- [ ] 12.2 Document layout coordinates in header file
- [ ] 12.3 Add usage examples for animation triggers
- [ ] 12.4 Document error message format ("check" / "mattress" / "cable")

