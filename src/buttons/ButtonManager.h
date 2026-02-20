/**
 * ====================================================================================
 * ButtonManager - کلاس مدیریت رابط کاربری فیزیکی (دکمه‌ها)
 * ====================================================================================
 * 
 * این کلاس یک لایه سطح بالا برای مدیریت تمام دکمه‌های فیزیکی سیستم است و
 * ارتباط بین ورودی کاربر و اجزای مختلف سیستم را برقرار می‌کند.
 * 
 * 1. معماری و وابستگی‌ها:
 *    - از 5 نمونه ButtonDebounce برای مدیریت دکمه‌های فیزیکی استفاده می‌کند
 *    - با اجزای Phase 1 (HeatingController, HeatingTimer, SettingsStorage) ارتباط دارد
 *    - با اجزای Phase 3 (WiFiManager) برای کنترل SoftAP ارتباط دارد
 * 
 * 2. دکمه‌های مدیریت شده:
 *    - Temp Up (GPIO14): افزایش دمای تنظیم شده
 *    - Temp Down (GPIO27): کاهش دمای تنظیم شده
 *    - Timer Up (GPIO13): افزایش مدت زمان تایمر
 *    - Timer Down (GPIO12): کاهش مدت زمان تایمر
 *    - Power (GPIO26): 
 *        * Short Press: روشن/خاموش کردن سیستم گرمایش
 *        * Long Press (3 ثانیه): روشن/خاموش کردن SoftAP WiFi
 * 
 * 3. عملکرد دکمه‌های دما:
 *    - با هر فشردن، دما به اندازه BTN_TEMP_INCREMENT (1 درجه) تغییر می‌کند
 *    - نگه داشتن دکمه باعث تکرار خودکار تغییر دما می‌شود
 *    - محدودیت‌های TEMP_MIN_C و TEMP_MAX_C اعمال می‌شود
 *    - تغییرات در SettingsStorage ذخیره می‌شوند
 * 
 * 4. عملکرد دکمه‌های تایمر:
 *    - با هر فشردن، تایمر به اندازه BTN_TIMER_INCREMENT (15 دقیقه) تغییر می‌کند
 *    - نگه داشتن دکمه باعث تکرار خودکار تغییر تایمر می‌شود
 *    - محدودیت‌های TIMER_MIN_MIN و TIMER_MAX_MIN اعمال می‌شود
 *    - اگر تایمر در حال اجرا باشد، با مقدار جدید restart می‌شود
 * 
 * 5. عملکرد دکمه Power:
 *    - Short Press:
 *        * اگر سیستم خاموش باشد: روشن می‌کند و تایمر را شروع می‌کند
 *        * اگر سیستم روشن باشد: خاموش می‌کند و تایمر را متوقف می‌کند
 *    - Long Press (3 ثانیه):
 *        * SoftAP WiFi را toggle می‌کند
 *        * برای صرفه‌جویی در مصرف انرژی یا امنیت استفاده می‌شود
 *        * اگر Station متصل نباشد، هشدار می‌دهد که دستگاه غیرقابل دسترسی می‌شود
 * 
 * 6. Activity Tracking:
 *    - زمان آخرین فعالیت کاربر را ثبت می‌کند
 *    - برای نمایش انیمیشن‌ها و timeout در DisplayManager استفاده می‌شود
 *    - با هر فشردن دکمه، این زمان به‌روزرسانی می‌شود
 * 
 * 7. Callback System:
 *    - از static callbacks برای اتصال به ButtonDebounce استفاده می‌کند
 *    - از static instance pointer (s_instance) برای دسترسی به متدهای instance استفاده می‌کند
 *    - این الگو برای سازگاری با callback های C-style ضروری است
 * 
 * 8. استفاده در main.cpp:
 *    - در setup() ساخته می‌شود و begin() فراخوانی می‌شود
 *    - در loop() متد update() فراخوانی می‌شود تا تمام دکمه‌ها پردازش شوند
 *    - متد isAnyButtonPressed() برای تشخیص فعالیت کاربر استفاده می‌شود
 * 
 * 9. یکپارچگی با سایر اجزا:
 *    - تغییرات دما و تایمر به صورت خودکار در SettingsStorage ذخیره می‌شوند
 *    - تغییرات به HeatingController و HeatingTimer اعمال می‌شوند
 *    - تغییرات SoftAP از طریق WiFiManager انجام می‌شوند
 *    - فعالیت کاربر برای DisplayManager و WebServer گزارش می‌شود
 * 
 * ====================================================================================
 */

#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include <Arduino.h>
#include "ButtonDebounce.h"
#include "config/RodiConfig.h"

// Forward declarations --- for integration with Phase 1 & 3 components ------------------------------------
class HeatingController;
class HeatingTimer;
class SettingsStorage;
class WiFiManager;

// ButtonManager class --- manages all physical buttons and their interactions ----------------------------
class ButtonManager {
public:
    // Constructor --- initialize with references to Phase 1 & 3 components --------------------------------
    ButtonManager(HeatingController& heatingCtrl, HeatingTimer& timer, SettingsStorage& settings, WiFiManager& wifiMgr);
    
    // Initialization --- setup all buttons with GPIO pins and callbacks ----------------------------------
    void begin();
    
    // Update method --- called every loop to process all buttons -----------------------------------------
    void update();
    
    // State queries --- check button states --------------------------------------------------------------
    bool isAnyButtonPressed() const;
    uint32_t getLastActivityTime() const { return m_lastActivityTime; }
    
    // Public API --- expose button handlers for API simulation -------------------------------------------
    // These methods allow WebServer to simulate button presses via API
    void simulateTimerUp();      // Simulate timer up button press
    void simulateTimerDown();    // Simulate timer down button press
    
private:
    // Phase 1 & 3 component references --- for controlling system ---------------------------------------
    HeatingController& m_heatingController;
    HeatingTimer& m_heatingTimer;
    SettingsStorage& m_settings;
    WiFiManager& m_wifiManager;
    
    // Button objects --- one for each physical button ----------------------------------------------------
    ButtonDebounce m_btnTempUp;
    ButtonDebounce m_btnTempDown;
    ButtonDebounce m_btnTimerUp;
    ButtonDebounce m_btnTimerDown;
    ButtonDebounce m_btnPower;
    
    // Activity tracking --- for display animations and timeout -------------------------------------------
    uint32_t m_lastActivityTime;
    
    // Button callback methods --- handlers for each button event -----------------------------------------
    static void onTempUpPress();
    static void onTempDownPress();
    static void onTimerUpPress();
    static void onTimerDownPress();
    static void onPowerShortPress();
    static void onPowerLongPress();
    
    // Static instance pointer --- for callback context ---------------------------------------------------
    static ButtonManager* s_instance;
    
    // Internal methods --- actual implementation of button actions ----------------------------------------
    void handleTempUp();
    void handleTempDown();
    void handleTimerUp();
    void handleTimerDown();
    void handlePowerToggle();
    void handlePowerLongPress();
    
    // Helper methods --- for value validation and updates -------------------------------------------------
    float getTemperatureSetpoint() const;
    void setTemperatureSetpoint(float temp);
    uint32_t getTimerDuration() const;
    void setTimerDuration(uint32_t minutes);
    void updateActivityTime();
};

#endif // BUTTON_MANAGER_H

