# Design Document: Simple Three-Column OLED UI

## Context

The RODI Smart Mattress currently uses a temporary debug display layout (simple vertical text list) that was implemented for Phase 2 testing. The system needs a proper UI that efficiently uses the 128×64 OLED screen to display:
- Current mattress temperature (primary information)
- Set temperature target
- Remaining timer duration
- System status (WiFi, Heating, System On/Off)
- Room conditions (temperature and humidity)

**Design Reference**: Full visual specification in `docs/UI_DESIGN_SIMPLE.md`

## Goals / Non-Goals

### Goals
- **Simple and readable**: Clear information hierarchy with largest font for current temperature
- **Efficient layout**: Three-column vertical division maximizing screen real estate
- **User feedback**: Simple Bold animation (500ms) when values change
- **Low complexity**: Avoid complex animation state machines or graphic libraries
- **Performance**: Maintain 5 FPS refresh rate (200ms) without impacting other systems
- **Status clarity**: Distinguish between "system on" vs "heating active" with separate icons

### Non-Goals
- Complex zoom/scale animations (too complex, marginal UX benefit)
- Color/grayscale effects (monochrome display only)
- Touch interaction (button-only interface)
- Custom fonts or graphics (use built-in library fonts)
- Scrolling or multi-page layouts (all info fits on single screen)

## Decisions

### Decision 1: Three-Column Vertical Layout

**Rationale**: 
- Natural information grouping (user input left/right, system state center)
- Vertical dividers provide clear visual separation
- Each column optimally sized for its content type
- Center column (64px) twice as wide for primary information

**Layout**:
```
[  Set Temp  ] | [    Current Temp    ] | [   Timer   ]
[   32px     ] | [       64px         ] | [   32px    ]
     Left              Center                Right
```

**Alternatives considered**:
- Horizontal rows: Would require more scrolling/paging, less efficient use of vertical space
- Single centered layout: Loses information hierarchy, harder to quickly scan data
- Grid layout: Too complex for simple data display needs

### Decision 2: Simple Bold Animation (500ms)

**Rationale**:
- Provides immediate user feedback without complexity
- 500ms duration is noticeable but not distracting
- Uses only font size changes (setTextSize 1→2 or 3→4)
- No interpolation or easing required (simpler implementation)
- Minimal CPU/memory overhead

**Implementation**:
```cpp
struct BoldAnimation {
    bool isActive;
    uint32_t startTime;
    uint32_t duration;  // Always 500ms
};
```

**Alternatives considered**:
- Zoom animation with easing: More complex, marginal UX improvement
- Blink/flash: Can be annoying, less professional
- Color inversion: Not effective on monochrome display
- No animation: Reduces user feedback quality

### Decision 3: System Status Icon (NEW)

**Rationale**:
- Users need to distinguish "system monitoring" vs "heating active"
- System can be ON (monitoring temperature) while heating is OFF (target reached)
- Three small icons (8×8px each) fit comfortably in center column top
- Clear visual state indication

**Icons**:
1. **WiFi**: Connection status (SoftAP or Station mode)
2. **Heating**: Element active/inactive
3. **System** (NEW): System monitoring on/off

**Alternatives considered**:
- Text labels: Take more space, harder to read quickly
- Single status indicator: Doesn't convey full system state
- More icons: Too cluttered for small display

### Decision 4: Timer Format - Minutes Only

**Rationale**:
- Typical heating duration is 4-8 hours (240-480 minutes)
- Minute precision is sufficient for user needs
- Simpler display format (`480 min` vs `8:00:00`)
- Reduces display width requirements

**Format**: `480 min` (integer minutes, no seconds)

**Alternatives considered**:
- HH:MM:SS: Too precise, takes more space
- HH:MM: Better but still less intuitive than plain minutes
- Hours only: Not precise enough (loses sub-hour information)

### Decision 5: Mattress Temperature in Center (Largest)

**Rationale**:
- Mattress temperature is the primary monitored value
- Most important for safety (overheating detection)
- Direct feedback on heating effectiveness
- User's main point of reference during operation

**Font Size**: 3 (normal) or 4 (Bold during change) - largest on screen

**Alternatives considered**:
- Room temperature in center: Less relevant to mattress heating control
- Equal emphasis: Loses information hierarchy
- Set temperature in center: Target, not current state

### Decision 6: Refresh Rate 5 FPS (200ms)

**Rationale**:
- Temperature changes slowly (seconds to minutes)
- Conserves power (important for always-on device)
- Reduces I2C bus usage
- Still responsive enough for user interactions
- Animations checked every cycle, display updated at 5 FPS

**Timing**:
- Animation state updates: Every loop cycle (for precise timing)
- Display refresh: Every 200ms (5 FPS)

**Alternatives considered**:
- 10 FPS: Minimal UX improvement, higher power consumption
- 1 FPS: May feel sluggish for button feedback
- Variable rate: Adds complexity without clear benefit

## Risks / Trade-offs

### Risk: Bold Animation Too Subtle
**Mitigation**: Font size increase (1→2, 3→4) is noticeable on small OLED. If needed, add border or inversion.

### Risk: Three Columns Too Cramped
**Mitigation**: Design tested with actual font sizes - 32px columns sufficient for setpoint/timer, 64px for current temp.

### Risk: Icon Clarity at 8×8 Pixels
**Mitigation**: SH110X library provides clear icons at this size. Test with actual users if concerns arise.

### Trade-off: Simplicity vs Features
**Decision**: Prioritize simplicity. Features like multi-page layouts or complex animations can be added later if needed.

### Trade-off: Minutes-Only Timer vs Precision
**Decision**: Minute precision is sufficient for multi-hour heating cycles. Users don't need second-level accuracy.

## Implementation Strategy

### Phase 1: Core Layout (Tasks 1-3)
- Define constants and data structures
- Implement basic three-column drawing
- Add vertical dividers

**Deliverable**: Static three-column layout visible on display

### Phase 2: Icon System (Task 4)
- Implement/update WiFi, Heating, System icons
- Position in center column top

**Deliverable**: Status icons displaying correctly

### Phase 3: Bold Animation (Tasks 2, 5)
- Implement animation state machine
- Add Bold drawing variants for each value
- Wire up animation triggers

**Deliverable**: Values animate Bold when changed

### Phase 4: Integration (Tasks 6, 8)
- Integrate all components in `drawMainLayout()`
- Add callbacks to ButtonManager
- Register callbacks in main.cpp

**Deliverable**: Complete UI with button feedback

### Phase 5: Testing & Cleanup (Tasks 9-11)
- Test all display states and animations
- Remove old debug code
- Update documentation

**Deliverable**: Production-ready display implementation

## Open Questions

1. **System Icon Design**: What visual should represent "system on"? Options:
   - Power symbol
   - Checkmark
   - Gear/settings icon
   - **Recommendation**: Power symbol (most intuitive)

2. **Bold Animation Trigger for Current Temp**: Should current temp animate on every sensor reading or only on significant changes (e.g., ≥0.5°C)?
   - **Recommendation**: Only on significant changes to avoid constant animation

3. **Error State Display**: How to show sensor errors in new layout?
   - **Decision**: Display three-line message in center column: "check" / "mattress" / "cable"
   - Layout structure (dividers, icons) preserved during error state

4. **WiFi Icon States**: Should we show different icons for SoftAP vs Station mode?
   - **Recommendation**: Yes - adds clarity about connection type

## Migration Plan

### Pre-Implementation
1. Review and approve this design
2. Ensure `docs/UI_DESIGN_SIMPLE.md` is the authoritative reference
3. Confirm no conflicts with Phase 3 network implementation

### Implementation
1. Create feature branch: `feature/simple-oled-ui`
2. Implement in phases (see Implementation Strategy)
3. Test each phase before proceeding
4. Keep debug layout code temporarily (commented) for rollback

### Testing
1. Visual inspection of layout at power-on
2. Button interaction testing (temp/timer changes trigger Bold)
3. Sensor reading display accuracy
4. Animation timing verification (500ms)
5. Refresh rate verification (200ms)

### Rollback Plan
If display becomes non-functional:
1. Revert to debug layout code (commented in same file)
2. Restore original `drawMainLayout()` method
3. Remove animation system temporarily
4. Debug and retry with smaller changes

## Error Display Example

### Mattress Sensor Error State

When DS18B20 sensor fails or cable disconnects:

```
║              ║                               ║                 ║
║              ║  [WiFi:ON] [Heat:OFF][Sys:ON] ║                 ║
║              ║                               ║                 ║
║              ║                               ║                 ║
║   ┌─────┐    ║    ┌───────────────┐          ║    ┌─────┐      ║
║   │27.5°│    ║    │     check     │          ║    │  0  │      ║
║   │ SET │    ║    │   mattress    │          ║    │ min │      ║
║   └─────┘    ║    │     cable     │          ║    └─────┘      ║
║              ║    └───────────────┘          ║                 ║
║              ║                               ║                 ║
║              ║      Room: 21° 23%            ║                 ║
║              ║                               ║                 ║
```

**Key points**:
- Three-line error message centered in center column
- Heating icon OFF (element automatically disabled)
- Layout structure preserved (dividers, icons, room info)
- Set temperature and timer remain visible
- System icon ON (system still monitoring)

## Success Criteria

- [ ] Three-column layout displays correctly
- [ ] All three status icons visible and update correctly
- [ ] Set temperature, current temperature, timer displayed in correct positions
- [ ] Room info displayed at bottom of center column
- [ ] Bold animation triggers on value changes
- [ ] Bold animation duration is 500ms ± 50ms
- [ ] Display refresh rate is 5 FPS (200ms) ± 20ms
- [ ] Timer displays minutes only (no HH:MM:SS)
- [ ] Mattress sensor error displays three-line message ("check" / "mattress" / "cable")
- [ ] Heating element auto-disables on sensor error
- [ ] Error message clears when sensor reconnects
- [ ] No watchdog resets or performance degradation
- [ ] Code is readable and well-commented

## References

- **Design Specification**: `docs/UI_DESIGN_SIMPLE.md` (authoritative visual reference)
- **Previous Proposal**: `docs/OLED UI DESIGN_PROPOSAL.md` (superseded, kept for history)
- **Library Documentation**: Adafruit SH110X OLED library
- **Current Implementation**: `src/display/DisplayManager.cpp` (debug layout to be replaced)

