# Implementation Tasks

## 1. Data Structures and Constants
- [x] 1.1 Define layout constants (column positions, divider lines, icon positions)
- [x] 1.2 Create `BoldAnimation` struct for simple Bold effect
- [x] 1.3 Create `AnimationContext` struct with three animation slots (setTemp, currentTemp, timer)
- [x] 1.4 Add member variables to `DisplayManager` class for animation state

## 2. Animation System
- [x] 2.1 ~~Implement `triggerSetTempBold()` method~~ (REMOVED - animation system removed)
- [x] 2.2 ~~Implement `triggerCurrentTempBold()` method~~ (REMOVED - animation system removed)
- [x] 2.3 ~~Implement `triggerTimerBold()` method~~ (REMOVED - animation system removed)
- [x] 2.4 ~~Implement `updateAnimations()` method~~ (REMOVED - animation system removed)
- [x] 2.5 ~~Implement `updateBoldAnimation()` helper method~~ (REMOVED - animation system removed)
- [x] 2.6 ~~Implement `isBoldActive()` helper method~~ (REMOVED - animation system removed)

## 3. Drawing Methods - Layout Structure
- [x] 3.1 Implement `drawVerticalDividers()` - draw lines at X=32 and X=96 (Y: 0-64)
- [x] 3.2 Implement `drawStatusBar()` - draw 3 icons at top of center column
- [x] 3.3 Implement `drawBottomBar()` - draw room info at bottom of center column

## 4. Drawing Methods - Icon System
- [x] 4.1 Update `drawWiFiIcon()` - 8x8 pixel icon at specified position
- [x] 4.2 Update `drawHeatingIcon()` - 8x8 pixel icon at specified position
- [x] 4.3 Implement `drawSystemIcon()` - NEW 8x8 pixel icon for system status

## 5. Drawing Methods - Data Display
- [x] 5.1 Implement `drawSetTempArea()` - left column with set temperature
- [x] 5.2 ~~Implement `drawSetTempWithBold()`~~ (REMOVED - static display only)
- [x] 5.3 Implement `drawMainDisplayArea()` - center column with current temperature (no degree symbol)
- [x] 5.4 ~~Implement `drawCurrentTempWithBold()`~~ (REMOVED - static display only, Font Size 2)
- [x] 5.5 Implement `drawTimerArea()` - right column with two-line format (number + "min") or "OFF"
- [x] 5.6 ~~Implement `drawTimerWithBold()`~~ (REMOVED - static display only)

## 6. Main Layout Integration
- [x] 6.1 Rewrite `drawMainLayout()` to orchestrate three-column layout
- [x] 6.2 ~~Update `update()` method to call `updateAnimations()`~~ (REMOVED - no animation)
- [x] 6.3 ~~Integrate Bold state checks in drawing calls~~ (REMOVED - static display)
- [x] 6.4 Update refresh rate management (5 FPS = 200ms interval)
- [x] 6.5 Add heatingEnabled parameter to distinguish system state from element state
- [x] 6.6 Implement "OFF" display when system is disabled

## 7. Helper Methods
- [x] 7.1 Implement `getTextWidth()` for text centering calculations
- [x] 7.2 Implement `getTextHeight()` for vertical positioning
- [x] 7.3 Update `formatTimer()` to return minutes only (remove HH:MM:SS format)
- [x] 7.4 Implement `centerTextInArea()` for positioning text in specific regions

## 8. ButtonManager Integration
- [x] 8.1 ~~Add callback function pointers~~ (REMOVED - animation removed)
- [x] 8.2 ~~Implement `onSetTempChanged()` callback registration~~ (REMOVED - animation removed)
- [x] 8.3 ~~Implement `onTimerChanged()` callback registration~~ (REMOVED - animation removed)
- [x] 8.4 ~~Call animation triggers in button event handlers~~ (REMOVED - animation removed)
- [x] 8.5 ~~Register callbacks in `main.cpp` setup~~ (REMOVED - animation removed)
- [x] 8.6 Add system turn-on logic when timer buttons pressed while system off

## 9. Error Display System
- [x] 9.1 Implement `showMattressSensorError()` method
- [x] 9.2 Format three-line error message ("check" / "mattress" / "cable")
- [x] 9.3 Center error message in center column
- [x] 9.4 Preserve layout structure (dividers, icons) during error
- [x] 9.5 Clear error message when sensor reconnects

## 10. Testing and Refinement
- [x] 10.1 Test basic three-column layout rendering
- [x] 10.2 Test vertical divider lines (full height)
- [x] 10.3 Test all three status icons (WiFi, Heating, System)
- [x] 10.4 ~~Test Bold animation for Set Temperature changes~~ (REMOVED - animation removed)
- [x] 10.5 ~~Test Bold animation for Timer changes~~ (REMOVED - animation removed)
- [x] 10.6 ~~Test Bold animation for Current Temperature changes~~ (REMOVED - animation removed)
- [x] 10.7 Test timer format (two-line: number + "min")
- [x] 10.8 Test room information display at bottom of center (without "Room:" prefix)
- [x] 10.9 Test mattress sensor error display (three-line message)
- [x] 10.10 Test heating element auto-disable on sensor error
- [x] 10.11 Test error message clearing when sensor reconnects
- [x] 10.12 ~~Verify 500ms animation duration~~ (REMOVED - animation removed)
- [x] 10.13 Verify 5 FPS refresh rate (200ms interval)
- [x] 10.14 Test "OFF" display when system is disabled
- [x] 10.15 Test system icon visibility (only when system enabled)
- [x] 10.16 Test timer buttons turning system on when pressed while off
- [x] 10.17 Test timer increment (5 minutes) and minimum (10 minutes)
- [x] 10.18 Test current temperature display without degree symbol

## 11. Cleanup
- [x] 11.1 Remove old debug layout code
- [x] 11.2 Remove unused zoom animation code and enums
- [x] 11.3 Remove Bold animation system (simplified to static display)
- [x] 11.4 Update header file comments
- [x] 11.5 Verify no memory leaks or performance issues

## 12. Documentation
- [x] 12.1 Update code comments to reference `docs/UI_DESIGN_SIMPLE.md`
- [x] 12.2 Document layout coordinates in header file
- [x] 12.3 Add usage examples for animation triggers
- [x] 12.4 Document error message format ("check" / "mattress" / "cable")

