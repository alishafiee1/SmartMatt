#include "ButtonManager.h"
#include "heating/HeatingController.h"
#include "timer/HeatingTimer.h"
#include "storage/SettingsStorage.h"

// Static instance pointer --- for callbacks --------------------------------------------------------------
ButtonManager* ButtonManager::s_instance = nullptr;

// Constructor --- initialize with Phase 1 component references -------------------------------------------
ButtonManager::ButtonManager(HeatingController& heatingCtrl, HeatingTimer& timer, SettingsStorage& settings)
    : m_heatingController(heatingCtrl)
    , m_heatingTimer(timer)
    , m_settings(settings)
    , m_btnTempUp((gpio_num_t)BTN_TEMP_UP_PIN)
    , m_btnTempDown((gpio_num_t)BTN_TEMP_DOWN_PIN)
    , m_btnTimerUp((gpio_num_t)BTN_TIMER_UP_PIN)
    , m_btnTimerDown((gpio_num_t)BTN_TIMER_DOWN_PIN)
    , m_btnPower((gpio_num_t)BTN_POWER_PIN)
    , m_lastActivityTime(0)
{
    s_instance = this;
}

// Initialization --- setup all buttons and register callbacks --------------------------------------------
void ButtonManager::begin() {
    Serial.println("Initializing Button Manager...");
    
    // Initialize all button hardware
    m_btnTempUp.begin();
    m_btnTempDown.begin();
    m_btnTimerUp.begin();
    m_btnTimerDown.begin();
    m_btnPower.begin();
    
    // Register temperature button callbacks
    m_btnTempUp.onShortPress(onTempUpPress);
    m_btnTempUp.onRepeat(onTempUpPress);
    
    m_btnTempDown.onShortPress(onTempDownPress);
    m_btnTempDown.onRepeat(onTempDownPress);
    
    // Register timer button callbacks
    m_btnTimerUp.onShortPress(onTimerUpPress);
    m_btnTimerUp.onRepeat(onTimerUpPress);
    
    m_btnTimerDown.onShortPress(onTimerDownPress);
    m_btnTimerDown.onRepeat(onTimerDownPress);
    
    // Register power button callbacks
    m_btnPower.onShortPress(onPowerShortPress);
    m_btnPower.onLongPress(onPowerLongPress);
    
    m_lastActivityTime = millis();
    
    Serial.println("✓ Button Manager initialized");
}

// Update method --- process all buttons ------------------------------------------------------------------
void ButtonManager::update() {
    m_btnTempUp.update();
    m_btnTempDown.update();
    m_btnTimerUp.update();
    m_btnTimerDown.update();
    m_btnPower.update();
}

// Query method --- check if any button is currently pressed ----------------------------------------------
bool ButtonManager::isAnyButtonPressed() const {
    return m_btnTempUp.isPressed() 
        || m_btnTempDown.isPressed()
        || m_btnTimerUp.isPressed()
        || m_btnTimerDown.isPressed()
        || m_btnPower.isPressed();
}

// Static callback --- temperature up button ---------------------------------------------------------------
void ButtonManager::onTempUpPress() {
    if (s_instance) {
        s_instance->handleTempUp();
    }
}

// Static callback --- temperature down button -------------------------------------------------------------
void ButtonManager::onTempDownPress() {
    if (s_instance) {
        s_instance->handleTempDown();
    }
}

// Static callback --- timer up button ---------------------------------------------------------------------
void ButtonManager::onTimerUpPress() {
    if (s_instance) {
        s_instance->handleTimerUp();
    }
}

// Static callback --- timer down button -------------------------------------------------------------------
void ButtonManager::onTimerDownPress() {
    if (s_instance) {
        s_instance->handleTimerDown();
    }
}

// Static callback --- power button short press ------------------------------------------------------------
void ButtonManager::onPowerShortPress() {
    if (s_instance) {
        s_instance->handlePowerToggle();
    }
}

// Static callback --- power button long press -------------------------------------------------------------
void ButtonManager::onPowerLongPress() {
    if (s_instance) {
        s_instance->handlePowerLongPress();
    }
}

// Handler --- increase temperature setpoint ---------------------------------------------------------------
void ButtonManager::handleTempUp() {
    float current = getTemperatureSetpoint();
    float newTemp = current + BTN_TEMP_INCREMENT;
    
    if (newTemp <= TEMP_MAX_C) {
        setTemperatureSetpoint(newTemp);
        updateActivityTime();
        Serial.printf("Temperature UP: %.1f°C → %.1f°C\n", current, newTemp);
    } else {
        Serial.printf("Temperature at maximum (%.1f°C)\n", TEMP_MAX_C);
    }
}

// Handler --- decrease temperature setpoint ---------------------------------------------------------------
void ButtonManager::handleTempDown() {
    float current = getTemperatureSetpoint();
    float newTemp = current - BTN_TEMP_INCREMENT;
    
    if (newTemp >= TEMP_MIN_C) {
        setTemperatureSetpoint(newTemp);
        updateActivityTime();
        Serial.printf("Temperature DOWN: %.1f°C → %.1f°C\n", current, newTemp);
    } else {
        Serial.printf("Temperature at minimum (%.1f°C)\n", TEMP_MIN_C);
    }
}

// Handler --- increase timer duration ---------------------------------------------------------------------
void ButtonManager::handleTimerUp() {
    uint32_t current = getTimerDuration();
    uint32_t newDuration = current + BTN_TIMER_INCREMENT;
    
    if (newDuration <= TIMER_MAX_MIN) {
        setTimerDuration(newDuration);
        updateActivityTime();
        Serial.printf("Timer UP: %u min → %u min\n", current, newDuration);
    } else {
        Serial.printf("Timer at maximum (%u min)\n", TIMER_MAX_MIN);
    }
}

// Handler --- decrease timer duration ---------------------------------------------------------------------
void ButtonManager::handleTimerDown() {
    uint32_t current = getTimerDuration();
    
    if (current > BTN_TIMER_INCREMENT) {
        uint32_t newDuration = current - BTN_TIMER_INCREMENT;
        if (newDuration >= TIMER_MIN_MIN) {
            setTimerDuration(newDuration);
            updateActivityTime();
            Serial.printf("Timer DOWN: %u min → %u min\n", current, newDuration);
        } else {
            Serial.printf("Timer at minimum (%u min)\n", TIMER_MIN_MIN);
        }
    } else {
        Serial.printf("Timer at minimum (%u min)\n", TIMER_MIN_MIN);
    }
}

// Handler --- toggle heating system on/off ----------------------------------------------------------------
void ButtonManager::handlePowerToggle() {
    bool currentlyEnabled = m_heatingController.isEnabled();
    
    if (currentlyEnabled) {
        // Disable heating and stop timer
        m_heatingController.disable();
        m_heatingTimer.stop();
        m_settings.setHeatingEnabled(false);
        Serial.println("Power: HEATING DISABLED");
    } else {
        // Enable heating and start timer
        m_heatingController.enable();
        
        // Start timer with current duration
        uint32_t duration = m_settings.getTimerDuration();
        m_heatingTimer.start(duration);
        
        m_settings.setHeatingEnabled(true);
        Serial.printf("Power: HEATING ENABLED (Timer: %u minutes)\n", duration);
    }
    
    m_settings.save();
    updateActivityTime();
}

// Handler --- power button long press (reserved for Phase 3 SoftAP toggle) --------------------------------
void ButtonManager::handlePowerLongPress() {
    Serial.println("Power LONG PRESS detected (reserved for Phase 3 - SoftAP toggle)");
    updateActivityTime();
    // Phase 3 will implement SoftAP toggle here
}

// Helper --- get current temperature setpoint -------------------------------------------------------------
float ButtonManager::getTemperatureSetpoint() const {
    return m_settings.getTemperatureSetpoint();
}

// Helper --- set new temperature setpoint and save --------------------------------------------------------
void ButtonManager::setTemperatureSetpoint(float temp) {
    m_settings.setTemperatureSetpoint(temp);
    m_heatingController.setSetpoint(temp);
    m_settings.save();
}

// Helper --- get current timer duration -------------------------------------------------------------------
uint32_t ButtonManager::getTimerDuration() const {
    return m_settings.getTimerDuration();
}

// Helper --- set new timer duration and save --------------------------------------------------------------
void ButtonManager::setTimerDuration(uint32_t minutes) {
    m_settings.setTimerDuration(minutes);
    
    // If timer is running, update it
    if (m_heatingTimer.isRunning()) {
        m_heatingTimer.start(minutes);
    }
    
    m_settings.save();
}

// Helper --- update last activity timestamp for display animations ----------------------------------------
void ButtonManager::updateActivityTime() {
    m_lastActivityTime = millis();
}

