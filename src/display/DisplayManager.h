#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>
#include "config/RodiConfig.h"

// Layout constants --- three-column vertical layout coordinates ------------------------------------------
#define COL_LEFT_X        0      // Left column start (Set Temperature)
#define COL_LEFT_WIDTH    32     // Left column width
#define COL_CENTER_X      32     // Center column start (Current Temperature)
#define COL_CENTER_WIDTH  64     // Center column width
#define COL_RIGHT_X       96     // Right column start (Timer)
#define COL_RIGHT_WIDTH   32     // Right column width

#define DIVIDER_LEFT_X    32     // Left divider line position
#define DIVIDER_RIGHT_X   96     // Right divider line position

// Icon positions --- status icons at top of center column ----------------------------------------------
#define ICON_WIFI_X       40     // WiFi icon X position
#define ICON_HEATING_X    52     // Heating icon X position  
#define ICON_SYSTEM_X     64     // System icon X position
#define ICON_Y            1      // All icons Y position
#define ICON_SIZE_PX      8      // Icon size in pixels (8×8)

// Room info position --- bottom of center column -------------------------------------------------------
#define ROOM_INFO_Y       54     // Room information Y position

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
                uint32_t timerSeconds, bool heatingActive, bool heatingEnabled, bool wifiConnected);
    
    // Immediate update --- force instant display refresh --------------------------------------------------
    void forceUpdate();
    
    // Error display --- show error messages ---------------------------------------------------------------
    void showError(const char* message);
    void clearError();
    void showMattressSensorError();
    
    // Display control --- turn display on/off -------------------------------------------------------------
    void displayOn();
    void displayOff();
    
    // State queries --- check display state ---------------------------------------------------------------
    bool isInitialized() const { return m_initialized; }
    
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
    bool m_heatingActive;      // Heating element is currently active (heating)
    bool m_heatingEnabled;      // Heating system is enabled (on/off state)
    bool m_wifiConnected;
    
    // Timing control --- for periodic updates -------------------------------------------------------------
    uint32_t m_lastUpdateTime;
    
    // Drawing methods --- UI rendering functions -----------------------------------------------------------
    void drawMainLayout();
    void drawVerticalDividers();
    void drawStatusBar();
    void drawBottomBar();
    
    // Icon drawing methods --- 8×8 pixel status icons ------------------------------------------------------
    void drawWiFiIcon(int16_t x, int16_t y);
    void drawHeatingIcon(int16_t x, int16_t y);
    void drawSystemIcon(int16_t x, int16_t y);
    
    // Column drawing methods --- data display --------------------------------------------------------------
    void drawSetTempArea();
    void drawMainDisplayArea();
    void drawTimerArea();
    
    // Boot animation stages --- boot sequence rendering ---------------------------------------------------
    void drawBootStage();  // Rodiset.ir
    
    // Helper methods --- utility functions -----------------------------------------------------------------
    void centerText(const char* text, int16_t y, uint8_t textSize = 1);
    void centerTextInArea(const char* text, int16_t x, int16_t y, int16_t width, uint8_t textSize = 1);
    int16_t getTextWidth(const char* text, uint8_t textSize);
    int16_t getTextHeight(uint8_t textSize);
    String formatTimer(uint32_t seconds);  // Now returns minutes only
};

#endif // DISPLAY_MANAGER_H

