# Update OLED UI to Simple Three-Column Layout

## Why

The current OLED display implementation uses a temporary debug layout (simple vertical list) that doesn't utilize the screen efficiently. A new simplified three-column design has been finalized that provides better information hierarchy, clearer visual separation, and improved user experience while remaining simple to implement.

## What Changes

- **NEW**: Three-column vertical layout (Left: Set Temp, Center: Current Temp, Right: Timer)
- **NEW**: System status icon added (WiFi, Heating, System - 3 icons total)
- **NEW**: Three-line error message for mattress sensor failure ("check" / "mattress" / "cable")
- **NEW**: "OFF" display in timer area when system is disabled
- **NEW**: System icon only displays when system is enabled (not always visible)
- **MODIFIED**: Timer display format changed from `HH:MM:SS` to two-line format (number + "min")
- **MODIFIED**: Timer increment changed from 15 minutes to 5 minutes
- **MODIFIED**: Timer minimum changed from 15 minutes to 10 minutes
- **MODIFIED**: Room information display moved to bottom of center column (`21° 23%` - without "Room:" prefix)
- **MODIFIED**: Display layout changed from debug vertical list to structured three-column design
- **MODIFIED**: Vertical divider lines added (full height Y: 0-64) at X=32 and X=96
- **MODIFIED**: Status icons moved to top of center column only (not spanning full width)
- **MODIFIED**: Current mattress temperature displayed as main number in center with Font Size 2 (no degree symbol)
- **MODIFIED**: Timer buttons now turn system on when pressed while system is off
- **MODIFIED**: Display distinguishes between heating enabled (system on) vs heating active (element on)
- **REMOVED**: Bold animation system (simplified to static display)
- **REMOVED**: Zoom animation complexity
- **REMOVED**: Horizontal divider lines (only vertical dividers remain)

## Impact

### Affected Specs
- `display-interface`: Complete redesign of OLED layout and animation system

### Affected Code
- `src/display/DisplayManager.h`: Add new drawing methods, System icon support, heatingEnabled state
- `src/display/DisplayManager.cpp`: Rewrite `drawMainLayout()` and all drawing methods, implement OFF display
- `src/buttons/ButtonManager.cpp`: Add system turn-on logic when timer buttons pressed while system off
- `src/main.cpp`: Pass heatingEnabled state to DisplayManager
- `include/config/RodiConfig.h`: Update timer increment and minimum values

### Breaking Changes
- **NONE** (Internal display implementation only - no external API changes)

### Dependencies
- Uses existing SH110X display library (no new dependencies)
- Compatible with current sensor and button implementations

### Design References
- Full design specification: `docs/UI_DESIGN_SIMPLE.md`
- Previous proposal (superseded): `docs/OLED UI DESIGN_PROPOSAL.md`

## Implementation Status

### Completed ✅
- Three-column layout fully implemented and tested
- All status icons (WiFi, Heating, System) working correctly
- Timer display with two-line format (number + "min") or "OFF"
- System state distinction (enabled vs active) implemented
- Timer buttons turn system on when pressed while off
- Error display system for mattress sensor failures
- All hardware testing completed successfully

### Changes from Original Design
- **Animation system removed**: Simplified to static display for better performance and simplicity
- **Timer format**: Changed to two-line format for better readability
- **System icon**: Only displays when system is enabled (not always visible)
- **OFF display**: Added when system is disabled instead of showing "0 min"
- **Timer increment**: Reduced from 15 to 5 minutes for finer control
- **Current temp display**: Removed degree symbol to fit within column bounds

### Testing Results
All hardware tests passed:
- Layout rendering verified
- Icon visibility and states confirmed
- Timer format and OFF display working correctly
- System state transitions tested
- Error handling verified
- Performance confirmed (5 FPS maintained)

