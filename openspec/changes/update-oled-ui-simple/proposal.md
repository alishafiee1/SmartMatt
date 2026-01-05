# Update OLED UI to Simple Three-Column Layout

## Why

The current OLED display implementation uses a temporary debug layout (simple vertical list) that doesn't utilize the screen efficiently. A new simplified three-column design has been finalized that provides better information hierarchy, clearer visual separation, and improved user experience while remaining simple to implement.

## What Changes

- **NEW**: Three-column vertical layout (Left: Set Temp, Center: Current Temp, Right: Timer)
- **NEW**: System status icon added (WiFi, Heating, System - 3 icons total)
- **NEW**: Simple Bold/font-size animation for value changes (500ms duration)
- **NEW**: Three-line error message for mattress sensor failure ("check" / "mattress" / "cable")
- **MODIFIED**: Timer display format changed from `HH:MM:SS` to minutes only (`480 min`)
- **MODIFIED**: Room information display moved to bottom of center column (`21° 23%` - without "Room:" prefix)
- **MODIFIED**: Display layout changed from debug vertical list to structured three-column design
- **MODIFIED**: Vertical divider lines added (full height Y: 0-64) at X=32 and X=96
- **MODIFIED**: Status icons moved to top of center column only (not spanning full width)
- **MODIFIED**: Current mattress temperature displayed as main number in center with Font Size 2/3 (not room temp)
- **REMOVED**: Zoom animation complexity (replaced with simple Bold effect)
- **REMOVED**: Horizontal divider lines (only vertical dividers remain)

## Impact

### Affected Specs
- `display-interface`: Complete redesign of OLED layout and animation system

### Affected Code
- `src/display/DisplayManager.h`: Add animation structures, new drawing methods, System icon support
- `src/display/DisplayManager.cpp`: Rewrite `drawMainLayout()` and all drawing methods
- `src/buttons/ButtonManager.h`: Add callbacks for animation triggers (optional integration)
- `src/buttons/ButtonManager.cpp`: Integrate animation callbacks
- `src/main.cpp`: Register animation callbacks if ButtonManager integration is implemented

### Breaking Changes
- **NONE** (Internal display implementation only - no external API changes)

### Dependencies
- Uses existing SH110X display library (no new dependencies)
- Compatible with current sensor and button implementations

### Design References
- Full design specification: `docs/UI_DESIGN_SIMPLE.md`
- Previous proposal (superseded): `docs/OLED UI DESIGN_PROPOSAL.md`

