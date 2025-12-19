# Phase 2: Device Panel Interface - Design Document

## Context
Phase 2 adds physical user interaction capabilities to the RODI Smart Mattress, building on Phase 1's core control system. Users need to adjust temperature, timer, and power settings directly on the device through buttons and receive visual feedback through an OLED display.

### Stakeholders
- End users requiring immediate device control without mobile device
- Product design team requiring intuitive button interface
- Phase 1 integration requirements (must not disrupt core control)

### Constraints
- Limited display real estate (128x64 pixels on SSD1306)
- Button count limited by available GPIO pins
- Must maintain Phase 1 safety guarantees
- Display refresh rate limited by I2C bus speed
- Animation performance on resource-constrained ESP32

## Goals / Non-Goals

### Goals
- Intuitive physical control for all essential functions
- Clear visual feedback of system state
- Professional boot sequence with branding
- Responsive button interface with debouncing
- Smooth animations for setting changes
- Clear status indication (WiFi, heating, sensors)

### Non-Goals
- Touchscreen interface
- Color display
- Configuration menus or settings pages
- Multi-language support (English/symbols only)
- Graph/trend visualization (deferred to web/app)

## Technical Decisions

### 1. Button Interface Architecture
**Decision**: Implement debounced button class with event-driven architecture

**Button Pattern Strategy**:
```
Short Press (<500ms):     Single action (±1°C, ±15min, toggle)
Long Press (≥3s):         Alternate function (SoftAP toggle)
Hold & Repeat (>500ms):   Accelerated changes (rate increases over time)
```

**Rationale**:
- Short press for common adjustments
- Long press for infrequent actions (prevents accidental triggers)
- Hold-and-repeat for rapid changes when needed
- Debouncing prevents false triggers from mechanical bounce

**Implementation**:
- 50ms debounce time (standard for mechanical buttons)
- State machine: IDLE → PRESSED → CONFIRMED → HELD → REPEATING
- Acceleration curve: Start 500ms repeat, reduce to 100ms after 3 seconds

**Alternatives considered**:
- Rotary encoder: Rejected due to cost and complexity
- Capacitive touch: Rejected due to reliability concerns in home environment
- Single multi-function button: Rejected as too unintuitive

### 2. Display Layout Design
**Decision**: 3-column vertical layout with status icon row

```
┌────────────────────────────────┐
│ [🔥]              [📶]         │  Status icons row
├────────────────────────────────┤
│        25.5°C  65%             │  Room temp/humidity (center, large)
│  35°C                 2:30     │  Setpoint (left) | Timer (right)
└────────────────────────────────┘
```

**Rationale**:
- Center focus on current conditions (most-viewed information)
- Setpoint and timer accessible but not dominant
- Icons provide at-a-glance status
- Layout maximizes font sizes for readability

**Font Sizes**:
- Room temp: 16pt (most important)
- Setpoint: 12pt (secondary)
- Timer: 10pt (tertiary)
- Icons: 12x12 pixels

**Alternatives considered**:
- Multi-page interface: Rejected as requiring navigation buttons
- Full-screen value on change: Rejected as disorienting
- Text-only (no icons): Rejected as less intuitive

### 3. Animation System
**Decision**: Zoom-in animation on value change with automatic revert

**Animation Behavior**:
```
Button Press → Value changes
↓
Changing value zooms larger (300ms ease-out)
↓
Value remains large while user continues pressing
↓
3 seconds after last press → Zoom back to normal (300ms ease-in)
↓
Return to standard layout
```

**Rationale**:
- Immediate feedback confirms button press registered
- Enlarged value easier to read during adjustment
- Auto-revert maintains clean layout without user action
- Animation provides professional polish

**Performance Targets**:
- 30 FPS during animation (33ms frame time)
- No lag in button responsiveness during animation
- Smooth easing curves (bezier or quadratic)

**Alternatives considered**:
- Blink/highlight instead of zoom: Rejected as less elegant
- Permanent large display: Rejected as reduces information density
- No animation: Rejected as less polished user experience

### 4. Boot Sequence Design
**Decision**: Two-stage branded boot animation

**Stage 1 (3 seconds)**: Rodiset.ir website animation
- Horizontal scroll or fade-in effect
- Medium font, centered

**Stage 2 (3 seconds)**: RODI branding
- Large bold font, centered
- Optional: Pulse or fade effect

**Total boot time**: ~6 seconds (animation) + hardware initialization

**Rationale**:
- Brand visibility during every power-on
- Provides visual feedback during hardware init
- Professional appearance
- 6 seconds allows Phase 1 initialization to complete

**Alternatives considered**:
- Instant to main screen: Rejected as missing branding opportunity
- Longer animation: Rejected as delays user access
- Logo graphics: Rejected due to display memory constraints

### 5. I2C Bus Configuration
**Decision**: Use standard I2C pins with 400kHz fast mode

**Configuration**:
```c
#define I2C_SDA_PIN  GPIO_NUM_21
#define I2C_SCL_PIN  GPIO_NUM_22
#define I2C_FREQ_HZ  400000  // 400kHz fast mode
```

**Rationale**:
- GPIO 21/22 are standard ESP32 I2C pins
- 400kHz provides adequate refresh rate for display
- Leaves room for future I2C devices on same bus
- Standard pins simplify hardware design

### 6. Display Update Strategy
**Decision**: Hybrid update approach - periodic refresh with immediate updates on change

**Update Logic**:
```
Periodic (1 Hz):     Temperature, humidity, timer (normal operation)
Immediate:           Button press, heating state change, errors
Dirty Flag:          Mark regions needing update
Partial Update:      Only redraw changed regions when possible
```

**Rationale**:
- 1 Hz refresh adequate for slowly-changing temperature
- Immediate updates for user interactions feel responsive
- Dirty flags minimize unnecessary I2C traffic
- Partial updates improve animation smoothness

**Performance**:
- Full screen refresh: ~16ms @ 400kHz I2C
- Partial refresh: ~5-10ms depending on region
- Animation frame: <33ms (30 FPS)

**Alternatives considered**:
- Fixed 10Hz refresh: Rejected as excessive I2C traffic
- Update only on change: Rejected as complex change detection
- Double buffering: Rejected due to memory constraints (1KB buffer)

## Pin Assignments

```c
// Buttons (active LOW with internal pull-up)
#define BTN_TEMP_UP_PIN    GPIO_NUM_13
#define BTN_TEMP_DOWN_PIN  GPIO_NUM_12
#define BTN_TIMER_UP_PIN   GPIO_NUM_14
#define BTN_TIMER_DOWN_PIN GPIO_NUM_27
#define BTN_POWER_PIN      GPIO_NUM_26

// I2C for OLED
#define I2C_SDA_PIN        GPIO_NUM_21
#define I2C_SCL_PIN        GPIO_NUM_22

// I2C Address
#define SSD1306_I2C_ADDR   0x3C
```

## Constants and Thresholds

```c
// Button Timing
#define BTN_DEBOUNCE_MS       50
#define BTN_LONG_PRESS_MS     3000
#define BTN_REPEAT_START_MS   500
#define BTN_REPEAT_FAST_MS    100
#define BTN_REPEAT_ACCEL_MS   3000

// Display
#define DISPLAY_WIDTH         128
#define DISPLAY_HEIGHT        64
#define DISPLAY_REFRESH_HZ    1
#define ICON_SIZE             12

// Animation
#define ANIM_ZOOM_DURATION_MS 300
#define ANIM_IDLE_TIMEOUT_MS  3000
#define ANIM_TARGET_FPS       30
```

## Data Structures

### Button State Machine
```c
enum ButtonState {
    BTN_IDLE,
    BTN_PRESSED,
    BTN_CONFIRMED,
    BTN_HELD,
    BTN_REPEATING
};

struct ButtonDebounce {
    gpio_num_t pin;
    ButtonState state;
    uint32_t press_time_ms;
    uint32_t last_repeat_ms;
    bool is_pressed;
    void (*on_press)(void);
    void (*on_long_press)(void);
    void (*on_repeat)(void);
};
```

### Display Context
```c
struct DisplayContext {
    bool dirty_flags[4];  // [temp, humidity, setpoint, timer]
    float room_temp;
    float room_humidity;
    float setpoint;
    uint16_t timer_remaining_sec;
    bool heating_active;
    bool wifi_connected;
    bool sensor_error;
};

enum AnimationState {
    ANIM_NONE,
    ANIM_ZOOM_IN,
    ANIM_HOLD,
    ANIM_ZOOM_OUT
};

struct AnimationContext {
    AnimationState state;
    uint8_t target_element;  // Which element is animating
    uint32_t start_time_ms;
    float scale_factor;  // 1.0 = normal, 1.5 = zoomed
};
```

## Error Handling Strategy

### Button Errors
- **Simultaneous press**: Ignored, only process first detected
- **Stuck button**: Watchdog will reset if button ISR hangs
- **Bounce beyond debounce**: Filtered by state machine

### Display Errors
- **I2C communication failure**: Retry up to 3 times, then skip frame
- **Display init failure**: Log error, continue without display
- **Memory allocation failure**: Use static buffers (no dynamic allocation)

### Integration Errors
- **Phase 1 API failure**: Display error message, continue UI operation
- **Invalid sensor readings**: Display "---" or error icon

## Performance Targets

### Button Responsiveness
- Press detection latency: <50ms (debounce time)
- Action execution: <10ms
- Total press-to-feedback: <100ms

### Display Performance
- Boot animation: 6 seconds ±0.5s
- Screen refresh: <20ms (full), <10ms (partial)
- Animation smoothness: 30 FPS minimum
- Button press to display update: <150ms

### Memory Usage
- Display buffer: 1 KB (128x64 / 8)
- Button state: <100 bytes
- Animation state: <100 bytes
- Total Phase 2: ~5 KB RAM, ~30 KB flash

## Testing Strategy

### Button Testing
- Rapid press test (10 presses/second)
- Long press timing accuracy
- Hold-and-repeat acceleration curve
- Simultaneous press handling
- Button state after reset

### Display Testing
- Boot sequence timing and appearance
- All layout elements visible and correct
- Icon visibility in all states
- Animation smoothness (visual inspection)
- Display readability at various angles
- Long-term burn-in test (72 hours)

### Integration Testing
- Button → Phase 1 → Display round-trip
- Error conditions displayed correctly
- Sensor readings match display values
- Timer countdown accuracy on display
- All status icons correct under all conditions

## Migration Plan

### Phase 1 → Phase 2 Integration
- Phase 1 remains unchanged (no breaking changes)
- Phase 2 wraps Phase 1 APIs with UI layer
- Backward compatibility maintained (Phase 1 works without Phase 2)

**Integration Steps**:
1. Add button and display initialization to main.cpp
2. Create button event handlers calling Phase 1 APIs
3. Add display update task reading Phase 1 state
4. Test isolated (Phase 2 only) before integration
5. Integration test (Phase 1 + Phase 2 together)

### Phase 2 → Phase 3 Preparation
- Power button long press reserved for SoftAP toggle
- Display WiFi icon ready for Phase 3 status
- Button interface complete (no changes needed in Phase 3)

## Risks / Trade-offs

### Risk: I2C Bus Contention
- **Impact**: Display glitches if other I2C devices added
- **Mitigation**: Use I2C mutex/semaphore for future devices
- **Trade-off**: Single device in Phase 2, no issue yet

### Risk: Button Wear Over Time
- **Impact**: Buttons may become unreliable after 100K+ presses
- **Mitigation**: Use quality tactile switches rated for 1M+ cycles
- **Trade-off**: Cost vs. reliability

### Risk: Display Burn-in
- **Impact**: Static elements (icons) may burn into OLED over years
- **Mitigation**: Use screensaver after 10 min idle (future enhancement)
- **Trade-off**: Not implemented in Phase 2

### Risk: Animation Performance
- **Impact**: Animations may stutter under heavy load
- **Mitigation**: Profile and optimize critical paths, reduce animation complexity if needed
- **Trade-off**: Visual polish vs. performance

### Risk: User Confusion on Long Press
- **Impact**: Users may accidentally trigger SoftAP toggle
- **Mitigation**: 3-second threshold is long enough to prevent accidents
- **Trade-off**: Accessibility (harder for some users to hold 3s)

## Open Questions

1. **Q**: Should display have backlight control for power saving?
   **A**: No backlight control on SSD1306, always-on or screensaver only

2. **Q**: Should button beep/click feedback be added?
   **A**: Deferred - requires buzzer hardware (not in Phase 2 BOM)

3. **Q**: How to indicate SoftAP is off (Phase 3 feature)?
   **A**: WiFi icon with "X" or slash-through

4. **Q**: Should boot sequence be skippable (button press)?
   **A**: No - boot animation timing allows hardware init, skipping adds complexity

5. **Q**: Display orientation (180° rotation option)?
   **A**: Deferred - fixed orientation in Phase 2, could be setting in Phase 3

## Dependencies

### Hardware Dependencies
- SSD1306 OLED display (128x64, I2C interface)
- 5x tactile push buttons (rated 1M+ cycles)
- 10kΩ pull-up resistors (if not using internal pull-ups)
- Proper I2C bus termination (if cable length >10cm)

### Software Dependencies
- SSD1306 OLED library (Adafruit or U8g2)
- ESP-IDF GPIO and I2C drivers
- Phase 1 API: HeatingController, SensorMonitor, TimerManager, SettingsStorage
- FreeRTOS for display update task

### External Dependencies
- Phase 1 must be completed and stable
- Pin availability (5 GPIO + 2 I2C = 7 total)
- I2C bus not used by other peripherals (or shared with mutex)

