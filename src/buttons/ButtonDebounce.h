/**
 * ====================================================================================
 * ButtonDebounce - کلاس مدیریت دکمه فیزیکی با قابلیت Debouncing و تشخیص رویدادها
 * ====================================================================================
 * 
 * این کلاس یک لایه پایه برای مدیریت یک دکمه فیزیکی است که وظایف زیر را انجام می‌دهد:
 * 
 * 1. Debouncing (حذف نویز):
 *    - با استفاده از State Machine، نویزهای الکتریکی و لرزش‌های مکانیکی دکمه را فیلتر می‌کند
 *    - مدت زمان debounce از طریق BTN_DEBOUNCE_MS (پیش‌فرض 50ms) قابل تنظیم است
 * 
 * 2. تشخیص رویدادها:
 *    - Short Press: فشردن و رها کردن سریع دکمه
 *    - Long Press: نگه داشتن دکمه به مدت BTN_LONG_PRESS_MS (پیش‌فرض 3 ثانیه)
 *    - Repeat: نگه داشتن دکمه برای مدت طولانی که باعث تکرار خودکار رویداد می‌شود
 * 
 * 3. State Machine:
 *    - BTN_IDLE: دکمه در حالت عادی و منتظر فشردن
 *    - BTN_PRESSED: دکمه فشرده شده و در حال debounce
 *    - BTN_CONFIRMED: فشردن تایید شده و منتظر رها شدن یا long press
 *    - BTN_HELD: دکمه نگه داشته شده (بعد از long press)
 *    - BTN_REPEATING: دکمه در حالت تکرار خودکار
 * 
 * 4. قابلیت Repeat با شتاب:
 *    - در ابتدا با فاصله BTN_REPEAT_START_MS (500ms) تکرار می‌شود
 *    - پس از BTN_REPEAT_ACCEL_TIME_MS (3 ثانیه) به BTN_REPEAT_FAST_MS (100ms) تسریع می‌شود
 *    - این قابلیت برای تغییر سریع مقادیر (مثل دما یا تایمر) مفید است
 * 
 * 5. استفاده:
 *    - این کلاس به صورت مستقیم استفاده نمی‌شود
 *    - توسط ButtonManager برای مدیریت 5 دکمه فیزیکی استفاده می‌شود:
 *      * دکمه افزایش دما (Temp Up)
 *      * دکمه کاهش دما (Temp Down)
 *      * دکمه افزایش تایمر (Timer Up)
 *      * دکمه کاهش تایمر (Timer Down)
 *      * دکمه پاور (Power)
 * 
 * 6. Callback System:
 *    - از callback functions برای اتصال رویدادها به handler ها استفاده می‌کند
 *    - هر دکمه می‌تواند callback های جداگانه برای Short Press، Long Press و Repeat داشته باشد
 * 
 * مثال استفاده:
 *    ButtonDebounce btn(GPIO_NUM_14, false);  // دکمه با active LOW
 *    btn.begin();
 *    btn.onShortPress(myShortPressHandler);
 *    btn.onLongPress(myLongPressHandler);
 *    btn.onRepeat(myRepeatHandler);
 *    
 *    // در loop:
 *    btn.update();
 * 
 * ====================================================================================
 */

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

