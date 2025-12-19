#include "SafetyMonitor.h"
#include <esp_system.h>

void SafetyMonitor::begin() {
    bootTimeMillis = millis();
    
    // Print reset reason for diagnostics
    printResetReason();
    
    // Configure and enable watchdog timer
    Serial.printf("[SafetyMonitor] Initializing watchdog timer (%d seconds timeout)...\n", 
                 WATCHDOG_TIMEOUT_SEC);
    
    // Enable task watchdog for current task
    esp_task_wdt_init(WATCHDOG_TIMEOUT_SEC, true);  // Enable panic on timeout
    esp_task_wdt_add(NULL);  // Add current task to watchdog
    
    watchdogEnabled = true;
    safe = true;
    
    Serial.println("[SafetyMonitor] Safety monitor initialized - Watchdog ENABLED");
}

void SafetyMonitor::feedWatchdog() {
    if (watchdogEnabled) {
        esp_task_wdt_reset();
    }
}

void SafetyMonitor::update(bool mattressSensorHealthy, bool roomSensorHealthy) {
    // Update critical sensor status (mattress sensor)
    criticalSensorHealthy = mattressSensorHealthy;
    
    // Determine overall safety state
    // Critical: Mattress sensor must be healthy for heating
    // Non-critical: Room sensor failure is warning only
    safe = criticalSensorHealthy;
    
    if (!mattressSensorHealthy) {
        static uint32_t lastWarning = 0;
        if (millis() - lastWarning > 5000) {  // Warn every 5 seconds
            Serial.println("[SafetyMonitor] *** WARNING: Critical sensor (mattress) unhealthy ***");
            lastWarning = millis();
        }
    }
    
    if (!roomSensorHealthy) {
        static uint32_t lastWarning = 0;
        if (millis() - lastWarning > 10000) {  // Warn every 10 seconds
            Serial.println("[SafetyMonitor] WARNING: Room sensor unhealthy (non-critical)");
            lastWarning = millis();
        }
    }
}

uint32_t SafetyMonitor::getUptimeSeconds() const {
    return (millis() - bootTimeMillis) / 1000;
}

void SafetyMonitor::printResetReason() {
    esp_reset_reason_t reason = esp_reset_reason();
    
    Serial.print("[SafetyMonitor] Last reset reason: ");
    
    switch (reason) {
        case ESP_RST_POWERON:
            Serial.println("Power-on reset");
            break;
        case ESP_RST_SW:
            Serial.println("Software reset");
            break;
        case ESP_RST_PANIC:
            Serial.println("*** PANIC RESET (possible watchdog timeout) ***");
            break;
        case ESP_RST_INT_WDT:
            Serial.println("*** INTERRUPT WATCHDOG TIMEOUT ***");
            break;
        case ESP_RST_TASK_WDT:
            Serial.println("*** TASK WATCHDOG TIMEOUT ***");
            break;
        case ESP_RST_WDT:
            Serial.println("*** OTHER WATCHDOG TIMEOUT ***");
            break;
        case ESP_RST_DEEPSLEEP:
            Serial.println("Deep sleep reset");
            break;
        case ESP_RST_BROWNOUT:
            Serial.println("*** BROWNOUT RESET (power issue) ***");
            break;
        case ESP_RST_SDIO:
            Serial.println("SDIO reset");
            break;
        default:
            Serial.printf("Unknown (%d)\n", reason);
            break;
    }
}

