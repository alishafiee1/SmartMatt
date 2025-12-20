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

