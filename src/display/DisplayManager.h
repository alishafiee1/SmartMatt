#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>
#include "config/RodiConfig.h"

// Display element IDs --- identifies which UI element needs update/animation -------------------------------
enum DisplayElement {
    ELEM_NONE = 0,
    ELEM_ROOM_TEMP,
    ELEM_ROOM_HUMIDITY,
    ELEM_SETPOINT,
    ELEM_TIMER,
    ELEM_HEATING_ICON,
    ELEM_WIFI_ICON
};

// Animation states --- tracks zoom animation lifecycle ----------------------------------------------------
enum AnimationState {
    ANIM_NONE,          // No animation active
    ANIM_ZOOM_IN,       // Zooming in (enlarging element)
    ANIM_HOLD,          // Holding zoomed state
    ANIM_ZOOM_OUT       // Zooming out (returning to normal)
};

// DisplayManager class --- manages OLED display, layout, and animations ----------------------------------
class DisplayManager {
public:
    // Constructor --- initialize display manager ----------------------------------------------------------
    DisplayManager();
    
    // Initialization --- setup I2C and display hardware ---------------------------------------------------
    bool begin();
    
    // Boot sequence --- animated branding display ----------------------------------------------------------
    void showBootSequence();
    
    // Update method --- refresh display with current data -------------------------------------------------
    void update(float roomTemp, float roomHumidity, float mattressTemp, float setpoint, 
                uint32_t timerSeconds, bool heatingActive, bool wifiConnected);
    
    // Immediate update --- force instant display refresh --------------------------------------------------
    void forceUpdate();
    
    // Animation triggers --- start zoom animation for specific element -------------------------------------
    void animateElement(DisplayElement element);
    
    // Error display --- show error messages ---------------------------------------------------------------
    void showError(const char* message);
    void clearError();
    
    // Display control --- turn display on/off -------------------------------------------------------------
    void displayOn();
    void displayOff();
    
    // State queries --- check display state ---------------------------------------------------------------
    bool isInitialized() const { return m_initialized; }
    bool isAnimating() const { return m_animState != ANIM_NONE; }
    
private:
    // Display hardware --- Adafruit SH1106 display object ----------------------------------------------------
    Adafruit_SH1106G m_display;
    
    // State flags --- display and animation state ----------------------------------------------------------
    bool m_initialized;
    bool m_errorActive;
    char m_errorMessage[64];
    
    // Display data cache --- current values to display ----------------------------------------------------
    float m_roomTemp;
    float m_roomHumidity;
    float m_mattressTemp;
    float m_setpoint;
    uint32_t m_timerSeconds;
    bool m_heatingActive;
    bool m_wifiConnected;
    
    // Animation state --- current animation parameters -----------------------------------------------------
    AnimationState m_animState;
    DisplayElement m_animElement;
    uint32_t m_animStartTime;
    uint32_t m_animIdleStart;
    float m_animScale;
    
    // Timing control --- for periodic updates -------------------------------------------------------------
    uint32_t m_lastUpdateTime;
    
    // Drawing methods --- UI rendering functions -----------------------------------------------------------
    void drawMainLayout();
    void drawRoomConditions();
    void drawSetpoint();
    void drawTimer();
    void drawHeatingIcon();
    void drawWiFiIcon();
    
    // Animation methods --- animation processing -----------------------------------------------------------
    void updateAnimation();
    float calculateAnimationScale(uint32_t elapsed);
    void drawAnimatedElement();
    
    // Boot animation stages --- boot sequence rendering ---------------------------------------------------
    void drawBootStage1();  // Rodiset.ir
    void drawBootStage2();  // RODI logo
    
    // Helper methods --- utility functions -----------------------------------------------------------------
    void centerText(const char* text, int16_t y, uint8_t textSize = 1);
    void drawTextWithScale(const char* text, int16_t x, int16_t y, uint8_t baseSize, float scale);
    String formatTimer(uint32_t seconds);
};

#endif // DISPLAY_MANAGER_H

