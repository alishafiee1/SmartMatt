#include "DisplayManager.h"

// Constructor --- initialize display manager with default values ------------------------------------------
DisplayManager::DisplayManager()
    : m_display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, SH1106_I2C_ADDR)
    , m_initialized(false)
    , m_errorActive(false)
    , m_roomTemp(0.0f)
    , m_roomHumidity(0.0f)
    , m_mattressTemp(0.0f)
    , m_setpoint(0.0f)
    , m_timerSeconds(0)
    , m_heatingActive(false)
    , m_heatingEnabled(false)
    , m_wifiConnected(false)
    , m_lastUpdateTime(0)
{
    memset(m_errorMessage, 0, sizeof(m_errorMessage));
}

// Initialization --- setup I2C bus and initialize OLED display --------------------------------------------
bool DisplayManager::begin() {
    Serial.println("Initializing Display Manager...");
    
    // Initialize I2C with custom pins
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(I2C_FREQ_HZ);
    
    // Set I2C timeout to prevent watchdog issues (100ms timeout)
    Wire.setTimeOut(100);
    
    Serial.printf("I2C initialized: SDA=%d, SCL=%d, Freq=%dHz\n", 
                  I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQ_HZ);
    
    // Initialize SH1106 display (address already set in constructor)
    if (!m_display.begin()) {
        Serial.println("✗ SH1106 initialization failed!");
        Serial.println("  Possible causes:");
        Serial.println("  - Display not connected");
        Serial.println("  - Wrong I2C address (try 0x7A if jumper changed)");
        Serial.println("  - I2C wiring issue");
        Serial.println("  Continuing without display...");
        m_initialized = false;
        return false;
    }
    
    // Configure display for optimal performance
    m_display.clearDisplay();
    m_display.setTextColor(SH110X_WHITE);
    m_display.setRotation(0);  // Normal orientation
    m_display.display();
    
    m_initialized = true;
    Serial.println("✓ Display Manager initialized successfully");
    
    return true;
}

// Boot sequence --- show animated branding on startup -----------------------------------------------------
void DisplayManager::showBootSequence() {
    if (!m_initialized) return;
    
    Serial.println("Starting boot sequence...");
    
    // Combined stage: Show TODI logo with Rodiset.ir underneath
    drawBootStage();
    
    // Break delay into smaller chunks to allow watchdog feeding
    // uint32_t totalDuration = BOOT_STAGE_DURATION_MS;
    // for (int i = 0; i < totalDuration / 100; i++) {
    //     delay(100);
    //     yield();  // Allow other tasks to run
    // }
    
    // Clear for main interface
    //m_display.clearDisplay();
    //m_display.display();
    
    Serial.println("✓ Boot sequence complete");
}

// Boot stage 1 --- Combined TODI logo and Rodiset.ir display ----------------------------------------------
void DisplayManager::drawBootStage() {
    m_display.clearDisplay();
    
    // Draw "TODI" in large font at top-center
    m_display.setTextSize(3);
    centerText("TODI", 16, 3);
    
    // Draw "Rodiset.ir" in small font below, centered
    m_display.setTextSize(1);
    centerText("Rodiset.ir", 44, 1);
    
    m_display.display();
}


// Update method --- refresh display with current sensor data -----------------------------------------------
void DisplayManager::update(float roomTemp, float roomHumidity, float mattressTemp, float setpoint,
                           uint32_t timerSeconds, bool heatingActive, bool heatingEnabled, bool wifiConnected) {
    if (!m_initialized) return;
    
    // Update cached values
    m_roomTemp = roomTemp;
    m_roomHumidity = roomHumidity;
    m_mattressTemp = mattressTemp;
    m_setpoint = setpoint;
    m_timerSeconds = timerSeconds;
    m_heatingActive = heatingActive;
    m_heatingEnabled = heatingEnabled;
    m_wifiConnected = wifiConnected;
    
    // Check if periodic update needed (5 Hz = 200ms for smooth updates)
    uint32_t now = millis();
    if (now - m_lastUpdateTime < DISPLAY_REFRESH_MS) {
        return;
    }
    m_lastUpdateTime = now;
    
    // Clear display buffer (fast, no I2C communication)
    m_display.clearDisplay();
    
    // Show error message if active, otherwise draw main layout
    if (m_errorActive) {
        // Draw three-column layout structure even in error mode
        drawVerticalDividers();
        drawStatusBar();
        
        // Draw three-line error message in center column
        m_display.setTextSize(1);
        int16_t msgY = 28;
        centerTextInArea("check", COL_CENTER_X, msgY, COL_CENTER_WIDTH, 1);
        centerTextInArea("mattress", COL_CENTER_X, msgY + 10, COL_CENTER_WIDTH, 1);
        centerTextInArea("cable", COL_CENTER_X, msgY + 20, COL_CENTER_WIDTH, 1);
    } else {
        // Draw main layout (all drawing is to buffer, no I2C yet)
        drawMainLayout();
    }
    
    // Update physical display (I2C communication - may take time)
    // NOTE: This is the potentially slow operation
    m_display.display();
}

// Force update --- immediate display refresh regardless of timing ------------------------------------------
void DisplayManager::forceUpdate() {
    m_lastUpdateTime = 0;  // Force next update
}

// Main layout drawing --- render three-column layout with all UI elements ----------------------------------
void DisplayManager::drawMainLayout() {
    // Draw vertical dividers (full height lines at X=32 and X=96)
    drawVerticalDividers();
    
    // Draw status bar (icons at top of center column)
    drawStatusBar();
    
    // Draw bottom bar (room info at bottom of center column)
    drawBottomBar();
    
    // Draw left column (Set Temperature)
    drawSetTempArea();
    
    // Draw center column (Current Mattress Temperature - largest)
    drawMainDisplayArea();
    
    // Draw right column (Timer)
    drawTimerArea();
}

// Vertical dividers drawing --- draw full-height divider lines at X=32 and X=96 ----------------------------
void DisplayManager::drawVerticalDividers() {
    // Left divider at X=32 (Y: 0-64)
    m_display.drawFastVLine(DIVIDER_LEFT_X, 0, DISPLAY_HEIGHT, SH110X_WHITE);
    
    // Right divider at X=96 (Y: 0-64)
    m_display.drawFastVLine(DIVIDER_RIGHT_X, 0, DISPLAY_HEIGHT, SH110X_WHITE);
}

// Status bar drawing --- draw three status icons at top of center column -----------------------------------
void DisplayManager::drawStatusBar() {
    // WiFi icon at X=40
    if (m_wifiConnected) {
        drawWiFiIcon(ICON_WIFI_X, ICON_Y);
    }
    
    // Heating icon at X=52
    if (m_heatingActive) {
        drawHeatingIcon(ICON_HEATING_X, ICON_Y);
    }
    
    // System icon at X=64 (only when system is enabled)
    if (m_heatingEnabled) {
        drawSystemIcon(ICON_SYSTEM_X, ICON_Y);
    }
}

// Bottom bar drawing --- draw room info at bottom of center column -----------------------------------------
void DisplayManager::drawBottomBar() {
    char roomInfo[32];
    
    if (m_roomTemp < -100 || m_roomHumidity < 0) {
        snprintf(roomInfo, sizeof(roomInfo), "--- --%");
    } else {
        snprintf(roomInfo, sizeof(roomInfo), "%.0f%c %.0f%%", 
                 m_roomTemp, (char)247, m_roomHumidity);  // char(247) = degree symbol
    }
    
    m_display.setTextSize(1);
    centerTextInArea(roomInfo, COL_CENTER_X, ROOM_INFO_Y, COL_CENTER_WIDTH, 1);
}

// WiFi icon drawing --- 8×8 pixel WiFi icon at specified position -----------------------------------------
void DisplayManager::drawWiFiIcon(int16_t x, int16_t y) {
    // Simple WiFi icon: three arcs (8×8 pixels)
    m_display.drawPixel(x + 4, y + 7, SH110X_WHITE);  // Base dot
    m_display.drawLine(x + 2, y + 5, x + 6, y + 5, SH110X_WHITE);  // Inner arc
    m_display.drawLine(x + 1, y + 3, x + 7, y + 3, SH110X_WHITE);  // Middle arc
    m_display.drawLine(x, y + 1, x + 8, y + 1, SH110X_WHITE);  // Outer arc
}

// Heating icon drawing --- 8×8 pixel heating/flame icon at specified position ------------------------------
void DisplayManager::drawHeatingIcon(int16_t x, int16_t y) {
    // Simple flame icon (8×8 pixels)
    m_display.drawPixel(x + 4, y, SH110X_WHITE);  // Top tip
    m_display.drawLine(x + 3, y + 1, x + 5, y + 1, SH110X_WHITE);
    m_display.drawLine(x + 2, y + 2, x + 6, y + 2, SH110X_WHITE);
    m_display.drawLine(x + 2, y + 3, x + 6, y + 3, SH110X_WHITE);
    m_display.drawLine(x + 1, y + 4, x + 7, y + 4, SH110X_WHITE);
    m_display.drawLine(x + 1, y + 5, x + 7, y + 5, SH110X_WHITE);
    m_display.drawLine(x + 2, y + 6, x + 6, y + 6, SH110X_WHITE);
    m_display.drawLine(x + 3, y + 7, x + 5, y + 7, SH110X_WHITE);
}

// System icon drawing --- 8×8 pixel power/system icon at specified position --------------------------------
void DisplayManager::drawSystemIcon(int16_t x, int16_t y) {
    // Simple power symbol icon (circle with line on top)
    m_display.drawCircle(x + 4, y + 5, 3, SH110X_WHITE);  // Circle
    m_display.drawLine(x + 4, y + 1, x + 4, y + 4, SH110X_WHITE);  // Vertical line
}


// Left column drawing --- Set Temperature display ----------------------------------------------------------
void DisplayManager::drawSetTempArea() {
    char tempStr[16];
    snprintf(tempStr, sizeof(tempStr), "%.1f%c", m_setpoint, (char)247);  // One decimal, degree symbol
    
    m_display.setTextSize(1);
    // Align to left edge of column (not centered)
    m_display.setCursor(COL_LEFT_X, 28);
    m_display.print(tempStr);
    
    // Draw "SET" label below temperature (also left-aligned)
    m_display.setCursor(COL_LEFT_X, 40);
    m_display.print("SET");
}

// Center column drawing --- Current Mattress Temperature display (main) ------------------------------------
void DisplayManager::drawMainDisplayArea() {
    char tempStr[16];
    
    if (m_mattressTemp < -100) {
        // Sensor error - error message shown in update() method
        return;
    }
    
    snprintf(tempStr, sizeof(tempStr), "%.1f", m_mattressTemp);  // One decimal, no degree symbol
    
    m_display.setTextSize(2);
    centerTextInArea(tempStr, COL_CENTER_X, 26, COL_CENTER_WIDTH, 2);
}

// Right column drawing --- Timer display (two lines: number + "min" or "OFF") ---------------------------
void DisplayManager::drawTimerArea() {
    m_display.setTextSize(1);
    
    // If heating system is disabled (not just element off), show "OFF" instead of timer
    if (!m_heatingEnabled) {
        centerTextInArea("OFF", COL_RIGHT_X, 28, COL_RIGHT_WIDTH, 1);
        return;
    }
    
    // Heating system is enabled - show timer (even if element is currently off)
    String timerStr = formatTimer(m_timerSeconds);
    
    // Draw timer number
    centerTextInArea(timerStr.c_str(), COL_RIGHT_X, 26, COL_RIGHT_WIDTH, 1);
    
    // Draw "min" label below
    centerTextInArea("min", COL_RIGHT_X, 36, COL_RIGHT_WIDTH, 1);
}

// Error display --- show error message on screen -----------------------------------------------------------
void DisplayManager::showError(const char* message) {
    if (!m_initialized) {
        // If display not initialized, just log to serial
        Serial.printf("⚠ Error (display unavailable): %s\n", message);
        return;
    }
    
    m_errorActive = true;
    strncpy(m_errorMessage, message, sizeof(m_errorMessage) - 1);
    m_errorMessage[sizeof(m_errorMessage) - 1] = '\0';
    
    Serial.printf("Display Error: %s\n", message);
    forceUpdate();
}

// Mattress sensor error display --- show three-line error message and disable heating --------------------
void DisplayManager::showMattressSensorError() {
    showError("Mattress sensor error");
    // Note: Heating element disable should be handled by SafetyMonitor/HeatingController
}

// Error clearing --- remove error message and resume normal display ----------------------------------------
void DisplayManager::clearError() {
    m_errorActive = false;
    memset(m_errorMessage, 0, sizeof(m_errorMessage));
    forceUpdate();
}

// Display control --- turn display on ----------------------------------------------------------------------
void DisplayManager::displayOn() {
    if (m_initialized) {
        m_display.display();
    }
}

// Display control --- turn display off ---------------------------------------------------------------------
void DisplayManager::displayOff() {
    if (m_initialized) {
        m_display.clearDisplay();
        m_display.display();
    }
}

// Helper --- center text horizontally at given Y position --------------------------------------------------
void DisplayManager::centerText(const char* text, int16_t y, uint8_t textSize) {
    int16_t x1, y1;
    uint16_t w, h;
    
    m_display.setTextSize(textSize);
    m_display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    
    int16_t x = (DISPLAY_WIDTH - w) / 2;
    m_display.setCursor(x, y);
    m_display.print(text);
}

// Helper --- format timer seconds as minutes only (two lines: number + "min") -------------------------------
// Add 1 second to prevent showing 9 in ones digit (e.g., 14:59 -> 15:00 display)
String DisplayManager::formatTimer(uint32_t seconds) {
    uint32_t minutes = (seconds + 10) / 60;  // Add 1 second to round up display
    
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%u", minutes);
    return String(buffer);
}

// Helper --- center text in specific area (column) ---------------------------------------------------------
void DisplayManager::centerTextInArea(const char* text, int16_t x, int16_t y, int16_t width, uint8_t textSize) {
    int16_t textWidth = getTextWidth(text, textSize);
    int16_t centerX = x + (width - textWidth) / 2;
    
    m_display.setTextSize(textSize);
    m_display.setCursor(centerX, y);
    m_display.print(text);
}

// Helper --- get text width for given size ------------------------------------------------------------------
int16_t DisplayManager::getTextWidth(const char* text, uint8_t textSize) {
    int16_t x1, y1;
    uint16_t w, h;
    
    m_display.setTextSize(textSize);
    m_display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    
    return w;
}

// Helper --- get text height for given size -----------------------------------------------------------------
int16_t DisplayManager::getTextHeight(uint8_t textSize) {
    // Standard font height is 8 pixels per textSize
    return 8 * textSize;
}

