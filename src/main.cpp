/**
 * RODI Smart Mattress - Phase 3: WiFi Network and Web Panel
 * 
 * Feature-based architecture implementing:
 * Phase 1:
 * - Temperature monitoring (DHT11 + DS18B20)
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
 * Phase 3:
 * - WiFi connectivity (SoftAP + Station modes)
 * - HTTP web server for control panel
 * - WebSocket for real-time data streaming
 * - REST API for device control
 * - Responsive web panel UI
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

// Phase 3 Components
#include "network/WiFiManager.h"
#include "server/WebServer.h"

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

// Phase 3 Components
WiFiManager wifiManager;
RodiWebServer* webServer = nullptr;  // Initialized after Phase 1 & 2 components

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
    
    // Initialize buttons (requires Phase 1 & 3 components)
    Serial.println("\n=== Initializing Button Interface ===");
    buttonManager = new ButtonManager(heatingController, heatingTimer, settings, wifiManager);
    buttonManager->begin();
    
    // ===== PHASE 3 INITIALIZATION =====
    
    // Initialize WiFi Manager
    Serial.println("\n=== Initializing WiFi Manager ===");
    safetyMonitor.feedWatchdog();  // Feed watchdog before WiFi init
    if (wifiManager.begin()) {
        Serial.println("✓ WiFi Manager initialized");
        Serial.printf("  - SoftAP SSID: %s\n", wifiManager.getSoftAPSSID().c_str());
        if (wifiManager.isSoftAPEnabled()) {
            Serial.printf("  - SoftAP IP: %s\n", wifiManager.getSoftAPIP().c_str());
        }
        if (wifiManager.isStationConnected()) {
            Serial.printf("  - Station IP: %s\n", wifiManager.getStationIP().c_str());
        }
    } else {
        Serial.println("⚠ WiFi Manager initialization failed");
    }
    safetyMonitor.feedWatchdog();  // Feed watchdog after WiFi init
    
    // Initialize Web Server (requires Phase 1 & 2 components)
    Serial.println("\n=== Initializing Web Server ===");
    Serial.println("Note: SPIFFS initialization may take 10-15 seconds...");
    
    // CRITICAL: Ensure WiFi is fully initialized before starting WebServer
    // WebServer requires WiFi to be active (SoftAP or Station) to initialize TCP/IP stack
    Serial.println("Ensuring WiFi is active for WebServer...");
    
    // Check if SoftAP or Station is enabled
    bool wifiActive = wifiManager.isSoftAPEnabled() || wifiManager.isStationConnected();
    
    // If WiFi is not active, force enable SoftAP (required for WebServer)
    if (!wifiActive) {
        Serial.println("[WebServer] ⚠ WiFi not active - enabling SoftAP for WebServer...");
        if (wifiManager.enableSoftAP()) {
            Serial.println("[WebServer] ✓ SoftAP enabled, waiting for it to start...");
            // Wait for SoftAP to actually start (can take a few seconds)
            int waitCount = 0;
            while (!wifiManager.isSoftAPEnabled() && waitCount < 30) {
                delay(100);
                wifiManager.update(); // Allow WiFi to process events
                waitCount++;
                yield(); // Allow other tasks to run
            }
            if (wifiManager.isSoftAPEnabled()) {
                Serial.println("[WebServer] ✓ SoftAP is now active");
                wifiActive = true;
            } else {
                Serial.println("[WebServer] ✗ SoftAP failed to start after timeout");
            }
        } else {
            Serial.println("[WebServer] ✗ Failed to enable SoftAP - WebServer may not work");
        }
    }
    
    // CRITICAL: Additional delay to ensure TCP/IP stack is fully initialized
    // This prevents "tcpip_send_msg_wait_sem (Invalid mbox)" error
    // The TCP/IP stack needs time to initialize after WiFi mode changes
    if (wifiActive) {
        Serial.println("[WebServer] ✓ WiFi is active, waiting for TCP/IP stack to initialize...");
        delay(3000); // Give TCP/IP stack sufficient time to initialize (3 seconds)
        Serial.println("[WebServer] TCP/IP stack should be ready now");
    } else {
        Serial.println("[WebServer] ✗ CRITICAL: WiFi is not active - WebServer cannot start safely");
        Serial.println("[WebServer]   System will continue but WebServer may fail");
        delay(2000); // Still wait a bit
    }
    
    webServer = new RodiWebServer(
        heatingController,
        heatingTimer,
        settings,
        roomSensor,
        mattressSensor
    );
    
    // Feed watchdog before potentially long SPIFFS operation
    safetyMonitor.feedWatchdog();
    Serial.println("Watchdog fed before web server initialization");
    
    if (webServer->begin()) {
        Serial.println("✓ Web Server started");
        Serial.println("  - HTTP server: http://192.168.4.1");
        Serial.println("  - WebSocket server: ws://192.168.4.1:81");
    } else {
        Serial.println("⚠ Web Server initialization failed");
    }
    
    // Feed watchdog after server init
    safetyMonitor.feedWatchdog();
    Serial.println("Watchdog fed after web server initialization");
    
    // Load and apply saved settings
    if (settings.isHeatingEnabled() && settings.getTimerDuration() > 0) {
        Serial.println("\n=== Restoring Previous Session ===");
        Serial.println("Note: Heating is OFF on boot for safety (will auto-enable when timer starts)");
    }
    
    Serial.println("\n=== System Initialization Complete ===");
    Serial.println("Phase 1: Core Hardware Control System - READY");
    Serial.println("Phase 2: Device Panel Interface - READY");
    Serial.println("Phase 3: WiFi Network and Web Panel - READY");
    Serial.println("\nSystem is in safe state:");
    Serial.println("  - Heating: OFF (safe boot state)");
    Serial.printf("  - Temperature Setpoint: %.1f°C\n", settings.getTemperatureSetpoint());
    Serial.printf("  - Timer Duration: %u minutes\n", settings.getTimerDuration());
    Serial.printf("  - WiFi Mode: %s\n", 
                 wifiManager.isSoftAPEnabled() && wifiManager.isStationConnected() ? "AP+STA" :
                 wifiManager.isSoftAPEnabled() ? "AP" :
                 wifiManager.isStationConnected() ? "STA" : "OFF");
    Serial.println("\nAccess web panel:");
    if (wifiManager.isSoftAPEnabled()) {
        Serial.printf("  - Direct: http://%s\n", wifiManager.getSoftAPIP().c_str());
    }
    if (wifiManager.isStationConnected()) {
        Serial.printf("  - LAN: http://%s\n", wifiManager.getStationIP().c_str());
    }
    Serial.println("\nStarting main loop...\n");
    
    delay(1000);
}

// ============================
// Main Loop
// ============================

void loop() {
    // Feed watchdog timer
    safetyMonitor.feedWatchdog();
    
    // ===== PHASE 3: WiFi Update =====
    wifiManager.update();
    
    // ===== PHASE 3: Web Server Update =====
    if (webServer) {
        webServer->update();
    }
    
    // ===== PHASE 2: Button Processing =====
    if (buttonManager) {
        buttonManager->update();
        
        // Trigger display animation on button activity
        if (buttonManager->isAnyButtonPressed()) {
            lastDisplayUpdateTime = 0;  // Force display update
            
            // Force WebSocket update when button is pressed (for real-time sync)
            if (webServer && webServer->isRunning() && webServer->getWebSocketClientCount() > 0) {
                webServer->sendWebSocketUpdate(WSMessageType::UPDATE, true);
            }
        }
    }
    
    // ===== PHASE 1: Sensor Reading =====
    if (millis() - lastSensorReadTime >= SENSOR_READ_INTERVAL_MS) {
        lastSensorReadTime = millis();
        
        // Read room sensor (DHT11)
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
        
        // Update WiFi status for display
        bool wifiConnected = wifiManager.isSoftAPEnabled() || wifiManager.isStationConnected();
        
        displayManager.update(
            roomSensor.getTemperature(),
            roomSensor.getHumidity(),
            mattressSensor.getTemperature(),
            settings.getTemperatureSetpoint(),
            heatingTimer.getRemainingSeconds(),
            heatingController.isHeating(),
            heatingController.isEnabled(),
            wifiConnected  // WiFi status (Phase 3)
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
    Serial.println("  Phase 3: WiFi Network & Web Panel ");
    Serial.println("=====================================");
    Serial.println("Version: 3.0.0");
    Serial.println("Framework: ESP-IDF (Arduino)");
    Serial.println("Hardware: ESP32 + SSD1306 OLED");
    Serial.println("Features:");
    Serial.println("  - Temperature Control");
    Serial.println("  - Timer Management");
    Serial.println("  - WiFi Connectivity");
    Serial.println("  - Web Control Panel");
    Serial.println("  - Real-time Monitoring");
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
    
    Serial.println("╠════════════════════════════════════════════════════════════╣");
    
    // WiFi status
    if (wifiManager.isSoftAPEnabled()) {
        Serial.printf("║ SoftAP: ENABLED | SSID: %s                        \n",
                     wifiManager.getSoftAPSSID().c_str());
        Serial.printf("║         IP: %s | Clients: %d                      \n",
                     wifiManager.getSoftAPIP().c_str(),
                     wifiManager.getConnectedClients());
    } else {
        Serial.println("║ SoftAP: DISABLED                                          ");
    }
    
    if (wifiManager.isStationConnected()) {
        Serial.printf("║ Station: CONNECTED | SSID: %s                     \n",
                     wifiManager.getStationSSID().c_str());
        Serial.printf("║          IP: %s                                   \n",
                     wifiManager.getStationIP().c_str());
    } else {
        Serial.println("║ Station: DISCONNECTED                                     ");
    }
    
    // Web server status
    if (webServer && webServer->isRunning()) {
        Serial.printf("║ Web Server: RUNNING | WebSocket Clients: %d              \n",
                     webServer->getWebSocketClientCount());
    } else {
        Serial.println("║ Web Server: STOPPED                                       ");
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
