/**
 * ====================================================================================
 * ButtonDebounce.cpp - پیاده‌سازی کلاس مدیریت دکمه فیزیکی
 * ====================================================================================
 * 
 * این فایل شامل پیاده‌سازی کامل State Machine و منطق debouncing است.
 * 
 * نکات مهم پیاده‌سازی:
 * 
 * 1. State Machine در متد update():
 *    - در هر فراخوانی، وضعیت فعلی پین خوانده می‌شود
 *    - بر اساس وضعیت فعلی و زمان‌های ثبت شده، به حالت بعدی منتقل می‌شود
 *    - رویدادها (callbacks) در زمان مناسب فراخوانی می‌شوند
 * 
 * 2. Debouncing:
 *    - پس از اولین فشردن، وارد حالت BTN_PRESSED می‌شود
 *    - اگر دکمه قبل از اتمام debounce رها شود، به BTN_IDLE برمی‌گردد (false trigger)
 *    - اگر debounce کامل شود، به BTN_CONFIRMED می‌رود
 * 
 * 3. Long Press Detection:
 *    - در حالت BTN_CONFIRMED، اگر دکمه به مدت BTN_LONG_PRESS_MS نگه داشته شود،
 *      رویداد Long Press فراخوانی می‌شود
 *    - برای جلوگیری از فراخوانی مکرر، از فلگ m_longPressTriggered استفاده می‌شود
 * 
 * 4. Repeat Mechanism:
 *    - پس از BTN_REPEAT_START_MS، وارد حالت BTN_REPEATING می‌شود
 *    - در این حالت، با فاصله‌های محاسبه شده (با شتاب)، callback onRepeat فراخوانی می‌شود
 *    - فاصله تکرار با استفاده از getRepeatInterval() محاسبه می‌شود
 * 
 * 5. Pin Reading:
 *    - متد readPin() وضعیت پین را با توجه به activeHigh/activeLow می‌خواند
 *    - برای active LOW (پیش‌فرض)، از INPUT_PULLUP استفاده می‌شود
 *    - برای active HIGH، از INPUT_PULLDOWN استفاده می‌شود
 * 
 * ====================================================================================
 */

#include "ButtonDebounce.h"

// Constructor --- initialize button with pin and active state -------------------------------------------
ButtonDebounce::ButtonDebounce(gpio_num_t pin, bool activeHigh)
    : m_pin(pin)
    , m_activeHigh(activeHigh)
    , m_state(BTN_IDLE)
    , m_isPressed(false)
    , m_pressTime(0)
    , m_lastRepeatTime(0)
    , m_debounceTime(0)
    , m_onShortPress(nullptr)
    , m_onLongPress(nullptr)
    , m_onRepeat(nullptr)
    , m_longPressTriggered(false)
{
}

// Initialization --- configure GPIO with pull resistor based on active level ----------------------------
void ButtonDebounce::begin() {
    // Configure pin as input
    pinMode(m_pin, m_activeHigh ? INPUT_PULLDOWN : INPUT_PULLUP);
    
    // Initialize state by reading current pin state
    m_isPressed = readPin();
    
    // If button is pressed during boot, start in pressed state but ignore until released
    if (m_isPressed) {
        m_state = BTN_PRESSED;
        m_pressTime = millis();
    }
    
    Serial.printf("Button GPIO%d initialized (active %s)\n", 
                  m_pin, m_activeHigh ? "HIGH" : "LOW");
}

// Update method --- state machine processing called every loop iteration ---------------------------------
void ButtonDebounce::update() {
    bool currentlyPressed = readPin();
    uint32_t now = millis();
    
    switch (m_state) {
        case BTN_IDLE:
            // Waiting for button press
            if (currentlyPressed) {
                m_state = BTN_PRESSED;
                m_isPressed = true;
                m_pressTime = now;
                m_debounceTime = now;
                m_longPressTriggered = false;
            }
            break;
            
        case BTN_PRESSED:
            // Debouncing period
            if (!currentlyPressed) {
                // Released before debounce complete - false trigger
                m_state = BTN_IDLE;
                m_isPressed = false;
            } else if (now - m_debounceTime >= BTN_DEBOUNCE_MS) {
                // Debounce complete - confirmed press
                m_state = BTN_CONFIRMED;
            }
            break;
            
        case BTN_CONFIRMED:
            // Confirmed press - waiting for release or long press
            if (!currentlyPressed) {
                // Released - trigger short press
                m_state = BTN_IDLE;
                m_isPressed = false;
                if (m_onShortPress && !m_longPressTriggered) {
                    m_onShortPress();
                }
            } else if (now - m_pressTime >= BTN_LONG_PRESS_MS && !m_longPressTriggered) {
                // Long press threshold reached
                m_longPressTriggered = true;
                if (m_onLongPress) {
                    m_onLongPress();
                }
                m_state = BTN_HELD;
            } else if (now - m_pressTime >= BTN_REPEAT_START_MS) {
                // Enter repeat mode
                m_state = BTN_REPEATING;
                m_lastRepeatTime = now;
            }
            break;
            
        case BTN_HELD:
            // Held after long press - waiting for release
            if (!currentlyPressed) {
                m_state = BTN_IDLE;
                m_isPressed = false;
            }
            break;
            
        case BTN_REPEATING:
            // Repeat mode - fire repeat events at intervals
            if (!currentlyPressed) {
                // Released - stop repeating
                m_state = BTN_IDLE;
                m_isPressed = false;
            } else {
                // Check if repeat interval elapsed
                uint32_t repeatInterval = getRepeatInterval();
                if (now - m_lastRepeatTime >= repeatInterval) {
                    m_lastRepeatTime = now;
                    if (m_onRepeat) {
                        m_onRepeat();
                    }
                }
                
                // Check for long press during repeat
                if (now - m_pressTime >= BTN_LONG_PRESS_MS && !m_longPressTriggered) {
                    m_longPressTriggered = true;
                    if (m_onLongPress) {
                        m_onLongPress();
                    }
                }
            }
            break;
    }
}

// Event registration --- register callback for short press event -----------------------------------------
void ButtonDebounce::onShortPress(void (*callback)(void)) {
    m_onShortPress = callback;
}

// Event registration --- register callback for long press event ------------------------------------------
void ButtonDebounce::onLongPress(void (*callback)(void)) {
    m_onLongPress = callback;
}

// Event registration --- register callback for repeat event ----------------------------------------------
void ButtonDebounce::onRepeat(void (*callback)(void)) {
    m_onRepeat = callback;
}

// Query method --- get duration button has been pressed --------------------------------------------------
uint32_t ButtonDebounce::getPressedDuration() const {
    if (m_isPressed) {
        return millis() - m_pressTime;
    }
    return 0;
}

// Helper method --- read pin state accounting for active level -------------------------------------------
bool ButtonDebounce::readPin() {
    bool pinState = digitalRead(m_pin);
    return m_activeHigh ? pinState : !pinState;
}

// Helper method --- calculate repeat interval with acceleration ------------------------------------------
uint32_t ButtonDebounce::getRepeatInterval() const {
    uint32_t pressDuration = millis() - m_pressTime;
    
    // If held longer than acceleration time, use fast repeat rate
    if (pressDuration >= BTN_REPEAT_ACCEL_TIME_MS) {
        return BTN_REPEAT_FAST_MS;
    }
    
    // Otherwise use initial repeat rate
    return BTN_REPEAT_START_MS;
}

