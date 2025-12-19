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
    , m_wifiConnected(false)
    , m_animState(ANIM_NONE)
    , m_animElement(ELEM_NONE)
    , m_animStartTime(0)
    , m_animIdleStart(0)
    , m_animScale(1.0f)
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
    
    // Stage 1: Rodiset.ir (with non-blocking delays)
    drawBootStage1();
    
    // Break delay into smaller chunks to allow watchdog feeding
    for (int i = 0; i < BOOT_STAGE1_DURATION_MS / 100; i++) {
        delay(100);
        yield();  // Allow other tasks to run
    }
    
    // Stage 2: RODI logo (with non-blocking delays)
    drawBootStage2();
    
    // Break delay into smaller chunks
    for (int i = 0; i < BOOT_STAGE2_DURATION_MS / 100; i++) {
        delay(100);
        yield();  // Allow other tasks to run
    }
    
    // Clear for main interface
    m_display.clearDisplay();
    m_display.display();
    
    Serial.println("✓ Boot sequence complete");
}

// Boot stage 1 --- Rodiset.ir website display --------------------------------------------------------------
void DisplayManager::drawBootStage1() {
    m_display.clearDisplay();
    
    // Draw "Rodiset.ir" centered
    m_display.setTextSize(2);
    centerText("Rodiset.ir", DISPLAY_HEIGHT / 2 - 8, 2);
    
    m_display.display();
}

// Boot stage 2 --- RODI logo display -----------------------------------------------------------------------
void DisplayManager::drawBootStage2() {
    m_display.clearDisplay();
    
    // Draw "RODI" in large font
    m_display.setTextSize(3);
    centerText("RODI", DISPLAY_HEIGHT / 2 - 12, 3);
    
    m_display.display();
}

// Update method --- refresh display with current sensor data -----------------------------------------------
void DisplayManager::update(float roomTemp, float roomHumidity, float mattressTemp, float setpoint,
                           uint32_t timerSeconds, bool heatingActive, bool wifiConnected) {
    if (!m_initialized) return;
    
    // Update cached values
    m_roomTemp = roomTemp;
    m_roomHumidity = roomHumidity;
    m_mattressTemp = mattressTemp;
    m_setpoint = setpoint;
    m_timerSeconds = timerSeconds;
    m_heatingActive = heatingActive;
    m_wifiConnected = wifiConnected;
    
    // Check if periodic update needed (5 Hz for smooth updates)
    uint32_t now = millis();
    if (now - m_lastUpdateTime < DISPLAY_REFRESH_MS) {
        return;
    }
    m_lastUpdateTime = now;
    
    // Clear display buffer (fast, no I2C communication)
    m_display.clearDisplay();
    
    // Show error message if active
    if (m_errorActive) {
        m_display.setTextSize(1);
        m_display.setCursor(0, 28);
        m_display.println(m_errorMessage);
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

// Animation trigger --- start zoom animation for element ---------------------------------------------------
void DisplayManager::animateElement(DisplayElement element) {
    if (!m_initialized) return;
    
    m_animElement = element;
    m_animState = ANIM_ZOOM_IN;
    m_animStartTime = millis();
    m_animIdleStart = 0;
    
    forceUpdate();
}

// Main layout drawing --- render all UI elements (DEBUG: simple vertical layout) ---------------------------
void DisplayManager::drawMainLayout() {
    // DEBUG MODE: Simple vertical layout with small font for debugging
    m_display.setTextSize(1);
    
    int y = 0;
    int lineHeight = 10;
    
    // Line 1: Heating status
    m_display.setCursor(0, y);
    m_display.print("Heat: ");
    m_display.print(m_heatingActive ? "ON " : "OFF");
    y += lineHeight;
    
    // Line 2: WiFi status
    m_display.setCursor(0, y);
    m_display.print("WiFi: ");
    m_display.print(m_wifiConnected ? "ON " : "OFF");
    y += lineHeight;
    
    // Line 3: Room temperature and humidity in one line
    m_display.setCursor(0, y);
    m_display.print("Room  ");
    if (m_roomTemp < -100) {
        m_display.print("---");
    } else {
        m_display.print(m_roomTemp, 1);
    }
    m_display.print((char)247);  // Degree symbol (°)
    m_display.print("  H:");
    if (m_roomHumidity < 0) {
        m_display.print("---");
    } else {
        m_display.print(m_roomHumidity, 1);  // 1 decimal place for precision
    }
    m_display.print("%");
    y += lineHeight;
    
    // Line 4: Mattress temperature
    m_display.setCursor(0, y);
    m_display.print("Matt: ");
    if (m_mattressTemp < -100) {
        m_display.print("---");
    } else {
        m_display.print(m_mattressTemp, 2);
    }
    m_display.print("C");
    y += lineHeight;
    
    // Line 5: Setpoint
    m_display.setCursor(0, y);
    m_display.print("Set:  ");
    m_display.print(m_setpoint, 0);
    m_display.print("C");
    y += lineHeight;
    
    // Line 6: Timer
    m_display.setCursor(0, y);
    m_display.print("Timer: ");
    m_display.print(formatTimer(m_timerSeconds));
}

// Room conditions drawing --- DEBUG: not used in simple layout ----------------------------------------------
void DisplayManager::drawRoomConditions() {
    // Not used in debug layout - all in drawMainLayout()
}

// Setpoint drawing --- DEBUG: not used in simple layout -----------------------------------------------------
void DisplayManager::drawSetpoint() {
    // Not used in debug layout - all in drawMainLayout()
}

// Timer drawing --- DEBUG: not used in simple layout --------------------------------------------------------
void DisplayManager::drawTimer() {
    // Not used in debug layout - all in drawMainLayout()
}

// Heating icon drawing --- DEBUG: shown as text in main layout ---------------------------------------------
void DisplayManager::drawHeatingIcon() {
    // Not used in debug layout - shown as text in drawMainLayout()
}

// WiFi icon drawing --- DEBUG: shown as text in main layout ------------------------------------------------
void DisplayManager::drawWiFiIcon() {
    // Not used in debug layout - shown as text in drawMainLayout()
}

// Animation update --- process zoom animation state machine ------------------------------------------------
void DisplayManager::updateAnimation() {
    if (m_animState == ANIM_NONE) return;
    
    uint32_t now = millis();
    uint32_t elapsed = now - m_animStartTime;
    
    switch (m_animState) {
        case ANIM_ZOOM_IN:
            if (elapsed >= ANIM_ZOOM_DURATION_MS) {
                m_animState = ANIM_HOLD;
                m_animIdleStart = now;
                m_animScale = ANIM_ZOOM_SCALE;
            } else {
                m_animScale = calculateAnimationScale(elapsed);
            }
            break;
            
        case ANIM_HOLD:
            if (now - m_animIdleStart >= ANIM_IDLE_TIMEOUT_MS) {
                m_animState = ANIM_ZOOM_OUT;
                m_animStartTime = now;
            }
            break;
            
        case ANIM_ZOOM_OUT:
            if (elapsed >= ANIM_ZOOM_DURATION_MS) {
                m_animState = ANIM_NONE;
                m_animScale = 1.0f;
            } else {
                m_animScale = ANIM_ZOOM_SCALE - (calculateAnimationScale(elapsed) * (ANIM_ZOOM_SCALE - 1.0f));
            }
            break;
            
        default:
            break;
    }
}

// Animation scale calculation --- smooth easing curve ------------------------------------------------------
float DisplayManager::calculateAnimationScale(uint32_t elapsed) {
    float progress = (float)elapsed / (float)ANIM_ZOOM_DURATION_MS;
    if (progress > 1.0f) progress = 1.0f;
    
    // Ease-out cubic: 1 - (1 - x)^3
    float eased = 1.0f - pow(1.0f - progress, 3.0f);
    
    return 1.0f + (eased * (ANIM_ZOOM_SCALE - 1.0f));
}

// Animated element drawing --- render zoomed element -------------------------------------------------------
void DisplayManager::drawAnimatedElement() {
    // For now, draw normal layout with scale effect
    // Full implementation would scale specific element
    drawRoomConditions();
    drawSetpoint();
    drawTimer();
    
    // TODO: Apply actual scaling to specific element based on m_animElement
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

// Helper --- format timer seconds as HH:MM:SS string --------------------------------------------------------
String DisplayManager::formatTimer(uint32_t seconds) {
    uint32_t hours = seconds / 3600;
    uint32_t minutes = (seconds % 3600) / 60;
    uint32_t secs = seconds % 60;
    
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%u:%02u:%02u", hours, minutes, secs);
    return String(buffer);
}

