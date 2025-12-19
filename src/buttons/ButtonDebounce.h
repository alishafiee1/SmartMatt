#ifndef BUTTON_DEBOUNCE_H
#define BUTTON_DEBOUNCE_H

#include <Arduino.h>
#include "config/RodiConfig.h"

// Button state machine --- tracks button press lifecycle from idle to repeat -----------------------------
enum ButtonState {
    BTN_IDLE,        // Button not pressed, waiting for input
    BTN_PRESSED,     // Button just pressed, waiting for debounce
    BTN_CONFIRMED,   // Press confirmed after debounce
    BTN_HELD,        // Button held beyond short press duration
    BTN_REPEATING    // Button held long enough to trigger repeat events
};

// Button event types --- callback event types for button actions ----------------------------------------
enum ButtonEvent {
    BTN_EVENT_NONE,
    BTN_EVENT_SHORT_PRESS,
    BTN_EVENT_LONG_PRESS,
    BTN_EVENT_REPEAT
};

// ButtonDebounce class --- handles debouncing and event detection for single button --------------------
class ButtonDebounce {
public:
    // Constructor --- initialize button with GPIO pin and optional active state --------------------------
    ButtonDebounce(gpio_num_t pin, bool activeHigh = false);
    
    // Initialization --- configure GPIO pin with pull resistor -------------------------------------------
    void begin();
    
    // Update method --- called frequently to process button state machine --------------------------------
    void update();
    
    // Event registration --- register callbacks for button events ----------------------------------------
    void onShortPress(void (*callback)(void));
    void onLongPress(void (*callback)(void));
    void onRepeat(void (*callback)(void));
    
    // State queries --- check current button state -------------------------------------------------------
    bool isPressed() const { return m_isPressed; }
    ButtonState getState() const { return m_state; }
    uint32_t getPressedDuration() const;
    
private:
    // Pin configuration --- hardware pin and logic level -------------------------------------------------
    gpio_num_t m_pin;
    bool m_activeHigh;
    
    // State machine --- current state and timing ----------------------------------------------------------
    ButtonState m_state;
    bool m_isPressed;
    uint32_t m_pressTime;
    uint32_t m_lastRepeatTime;
    uint32_t m_debounceTime;
    
    // Callbacks --- user-defined event handlers -----------------------------------------------------------
    void (*m_onShortPress)(void);
    void (*m_onLongPress)(void);
    void (*m_onRepeat)(void);
    
    // Flags --- event processing state --------------------------------------------------------------------
    bool m_longPressTriggered;
    
    // Helper methods --- internal state processing --------------------------------------------------------
    bool readPin();
    uint32_t getRepeatInterval() const;
};

#endif // BUTTON_DEBOUNCE_H

