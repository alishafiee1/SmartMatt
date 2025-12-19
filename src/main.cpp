/**
 * RODI Smart Mattress - Phase 2: Device Panel Interface
 * 
 * Feature-based architecture implementing:
 * Phase 1:
 * - Temperature monitoring (DHT22 + DS18B20)
 * - Heating control with hysteresis
 * - Timer management for auto-shutoff
 * - Settings persistence (NVS)
 * - Safety features (watchdog, sensor validation)
 * 
 * Phase 2:
 * - Physical button interface with debouncing
 * - OLED display with animated UI
 * - Boot sequence with branding
 * - Status icons (heating, WiFi)
 * - Display animations on value changes
 * 
 * Hardware: ESP32
 * Framework: Arduino (ESP-IDF)
 * 
 * Team: RODI
 * Developer: AliShafiee
 * HW Designer: H.Rostamizade
 */

#include <Arduino.h>
#include "config/RodiConfig.h"

// Phase 1 Components
#include "sensors/RoomSensor.h"
#include "sensors/MattressSensor.h"
#include "heating/HeatingController.h"
#include "timer/HeatingTimer.h"
#include "storage/SettingsStorage.h"
#include "safety/SafetyMonitor.h"

// Phase 2 Components
#include "buttons/ButtonManager.h"
#include "display/DisplayManager.h"

// ============================
// Global Objects
// ============================

// Phase 1 Components
RoomSensor roomSensor;
MattressSensor mattressSensor;
HeatingController heatingController;
HeatingTimer heatingTimer;
SettingsStorage settings;
SafetyMonitor safetyMonitor;

// Phase 2 Components
DisplayManager displayManager;
ButtonManager* buttonManager = nullptr;  // Initialized after Phase 1 components

// ============================
// Timing Variables
// ============================

unsigned long lastSensorReadTime = 0;
unsigned long lastStatusPrintTime = 0;
unsigned long lastDisplayUpdateTime = 0;

// ============================
// Function Declarations
// ============================

void printSystemStatus();
void printWelcomeBanner();
void handleTimerExpiration();

// ============================
// Setup Function
// ============================

void setup() {
    // Initialize serial communication
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000);  // Allow serial to stabilize
    
    printWelcomeBanner();
    
    // ===== PHASE 1 INITIALIZATION =====
    
    // Initialize safety monitor (includes watchdog)
    Serial.println("\n=== Initializing Safety Monitor ===");
    safetyMonitor.begin();
    
    // Initialize settings storage
    Serial.println("\n=== Initializing Settings Storage ===");
    settings.begin();
    
    // Initialize sensors
    Serial.println("\n=== Initializing Sensors ===");
    roomSensor.begin();
    mattressSensor.begin();
    
    // Initialize heating controller
    Serial.println("\n=== Initializing Heating Controller ===");
    heatingController.begin();
    heatingController.setSetpoint(settings.getTemperatureSetpoint());
    
    // Initialize timer
    Serial.println("\n=== Initializing Timer ===");
    heatingTimer.begin();
    
    // ===== PHASE 2 INITIALIZATION =====
    
    // Initialize display (show boot sequence)
    Serial.println("\n=== Initializing Display ===");
    if (displayManager.begin()) {
        // Feed watchdog before boot sequence (takes 6 seconds)
        safetyMonitor.feedWatchdog();
        displayManager.showBootSequence();
        safetyMonitor.feedWatchdog();  // Feed again after boot sequence
    } else {
        Serial.println("⚠ Display disabled - system will run without display");
    }
    
    // Initialize buttons (requires Phase 1 components)
    Serial.println("\n=== Initializing Button Interface ===");
    buttonManager = new ButtonManager(heatingController, heatingTimer, settings);
    buttonManager->begin();
    
    // Load and apply saved settings
    if (settings.isHeatingEnabled() && settings.getTimerDuration() > 0) {
        Serial.println("\n=== Restoring Previous Session ===");
        Serial.println("Note: Heating is OFF on boot for safety (will auto-enable when timer starts)");
    }
    
    Serial.println("\n=== System Initialization Complete ===");
    Serial.println("Phase 1: Core Hardware Control System - READY");
    Serial.println("Phase 2: Device Panel Interface - READY");
    Serial.println("\nSystem is in safe state:");
    Serial.println("  - Heating: OFF (safe boot state)");
    Serial.printf("  - Temperature Setpoint: %.1f°C\n", settings.getTemperatureSetpoint());
    Serial.printf("  - Timer Duration: %u minutes\n", settings.getTimerDuration());
    Serial.println("\nStarting main loop...\n");
    
    delay(1000);
}

// ============================
// Main Loop
// ============================

void loop() {
    // Feed watchdog timer
    safetyMonitor.feedWatchdog();
    
    // ===== PHASE 2: Button Processing =====
    if (buttonManager) {
        buttonManager->update();
        
        // Trigger display animation on button activity
        if (buttonManager->isAnyButtonPressed()) {
            lastDisplayUpdateTime = 0;  // Force display update
        }
    }
    
    // ===== PHASE 1: Sensor Reading =====
    if (millis() - lastSensorReadTime >= SENSOR_READ_INTERVAL_MS) {
        lastSensorReadTime = millis();
        
        // Read room sensor (DHT22)
        roomSensor.read();
        
        // Read mattress sensor (DS18B20) - critical for heating control
        bool mattressSensorOk = mattressSensor.read();
        
        // Update safety monitor with sensor health
        safetyMonitor.update(mattressSensor.isHealthy(), roomSensor.isHealthy());
        
        // Update heating control
        heatingController.update(
            mattressSensor.getTemperature(),
            settings.getTemperatureSetpoint(),
            mattressSensor.isHealthy()
        );
        
        // ===== PHASE 2: Error Display =====
        if (!mattressSensor.isHealthy()) {
            displayManager.showError("Check mattress sensor");
        } else {
            displayManager.clearError();
        }
    }
    
    // ===== PHASE 1: Timer Update =====
    heatingTimer.update();
    
    // Handle timer expiration
    if (heatingTimer.hasExpired()) {
        handleTimerExpiration();
    }
    
    // ===== PHASE 2: Display Update =====
    if (displayManager.isInitialized() && 
        millis() - lastDisplayUpdateTime >= DISPLAY_REFRESH_MS) {
        lastDisplayUpdateTime = millis();
        
        // Feed watchdog before potentially slow I2C operation
        safetyMonitor.feedWatchdog();
        
        displayManager.update(
            roomSensor.getTemperature(),
            roomSensor.getHumidity(),
            mattressSensor.getTemperature(),
            settings.getTemperatureSetpoint(),
            heatingTimer.getRemainingSeconds(),
            heatingController.isHeating(),
            false  // WiFi status (Phase 3)
        );
    }
    
    // Print system status periodically (every 10 seconds)
    if (millis() - lastStatusPrintTime >= 10000) {
        lastStatusPrintTime = millis();
        printSystemStatus();
    }
    
    // Small delay to prevent busy-wait
    delay(MAIN_LOOP_DELAY_MS);
}

// ============================
// Helper Functions
// ============================

void printWelcomeBanner() {
    Serial.println("\n");
    Serial.println("=====================================");
    Serial.println("     RODI Smart Mattress System     ");
    Serial.println("   Phase 2: Device Panel Interface  ");
    Serial.println("=====================================");
    Serial.println("Version: 2.0.0");
    Serial.println("Framework: ESP-IDF (Arduino)");
    Serial.println("Hardware: ESP32 + SSD1306 OLED");
    Serial.println("Team: RODI");
    Serial.println("Developer: AliShafiee");
    Serial.println("HW Designer: H.Rostamizade");
    Serial.println("=====================================\n");
}

void printSystemStatus() {
    Serial.println("\n╔════════════════════════════════════════════════════════════╗");
    Serial.println("║                    SYSTEM STATUS                           ║");
    Serial.println("╠════════════════════════════════════════════════════════════╣");
    
    // Uptime
    Serial.printf("║ Uptime: %lu seconds                                    \n", 
                 safetyMonitor.getUptimeSeconds());
    
    // Safety status
    Serial.printf("║ Safety: %s                                              \n",
                 safetyMonitor.isSafe() ? "✓ SAFE   " : "✗ UNSAFE ");
    
    Serial.println("╠════════════════════════════════════════════════════════════╣");
    
    // Room sensor
    Serial.printf("║ Room Temp: %.1f°C | Humidity: %.1f%% | Status: %s    \n",
                 roomSensor.getTemperature(),
                 roomSensor.getHumidity(),
                 roomSensor.isHealthy() ? "OK  " : "FAIL");
    
    // Mattress sensor
    Serial.printf("║ Mattress Temp: %.2f°C | Status: %s                      \n",
                 mattressSensor.getTemperature(),
                 mattressSensor.isHealthy() ? "OK  " : "FAIL");
    
    Serial.println("╠════════════════════════════════════════════════════════════╣");
    
    // Heating control
    Serial.printf("║ Heating: %s | Setpoint: %.1f°C | Element: %s       \n",
                 heatingController.isEnabled() ? "ENABLED " : "DISABLED",
                 settings.getTemperatureSetpoint(),
                 heatingController.isHeating() ? "ON " : "OFF");
    
    // Timer status
    if (heatingTimer.isRunning()) {
        uint32_t remainingMin = heatingTimer.getRemainingMinutes();
        uint32_t remainingSec = heatingTimer.getRemainingSeconds() % 60;
        Serial.printf("║ Timer: RUNNING | Remaining: %u:%02u                       \n",
                     remainingMin, remainingSec);
    } else {
        Serial.printf("║ Timer: STOPPED | Duration: %u minutes                    \n",
                     settings.getTimerDuration());
    }
    
    Serial.println("╚════════════════════════════════════════════════════════════╝\n");
}

void handleTimerExpiration() {
    Serial.println("\n╔════════════════════════════════════════════════════════════╗");
    Serial.println("║              ⏰ TIMER EXPIRED - AUTO SHUTOFF               ║");
    Serial.println("╚════════════════════════════════════════════════════════════╝\n");
    
    // Disable heating
    heatingController.disable();
    settings.setHeatingEnabled(false);
    
    // Clear expired flag
    heatingTimer.clearExpired();
    
    // Save settings
    settings.save();
    
    Serial.println("✓ Heating disabled");
    Serial.println("✓ Settings saved");
    Serial.println("✓ System in safe state\n");
}
