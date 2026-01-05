/**
 * WiFiManager.cpp - WiFi network management implementation
 * 
 * Implements dual-mode WiFi connectivity with SoftAP and Station modes.
 * Handles WiFi events, auto-reconnection, and credential persistence.
 * 
 * Phase 3: WiFi Network and Web Panel
 */

#include "WiFiManager.h"
#include <Preferences.h>

// Static instance for event callbacks
static WiFiManager* s_instance = nullptr;

// Constructor --- initialize WiFi manager with default state ------------------------------
WiFiManager::WiFiManager() 
    : currentMode(WiFiMode::OFF),
      stationStatus(WiFiStatus::DISCONNECTED),
      softAPEnabled(false),
      stationEnabled(false),
      softAPPassword(WIFI_AP_PASSWORD),
      lastConnectAttempt(0),
      connectRetryCount(0),
      autoReconnect(true)
{
    s_instance = this;
}

// Destructor --- cleanup WiFi resources ---------------------------------------------------
WiFiManager::~WiFiManager() {
    disableSoftAP();
    disconnectStation();
    WiFi.mode(WIFI_OFF);
    s_instance = nullptr;
}

// Initialize WiFi --- setup WiFi radio and load saved credentials -------------------------
bool WiFiManager::begin() {
    Serial.println("[WiFi] Initializing WiFi Manager...");
    
    // Set WiFi mode to off initially
    WiFi.mode(WIFI_OFF);
    delay(100);
    
    // Generate unique SoftAP SSID
    softAPSSID = generateSoftAPSSID();
    Serial.printf("[WiFi] Generated SoftAP SSID: %s\n", softAPSSID.c_str());
    
    // Register WiFi event handler
    WiFi.onEvent(onWiFiEvent);
    
    // Load stored credentials
    if (loadCredentials()) {
        Serial.println("[WiFi] Loaded stored WiFi credentials");
        Serial.printf("[WiFi] Stored SSID: %s\n", stationSSID.c_str());
    } else {
        Serial.println("[WiFi] No stored credentials found");
    }
    
    // Load SoftAP enabled state from NVS
    Preferences prefs;
    if (prefs.begin(SETTINGS_NAMESPACE, true)) {
        softAPEnabled = prefs.getBool(SETTINGS_KEY_SOFTAP_ENABLED, true); // Default: enabled
        prefs.end();
        Serial.printf("[WiFi] SoftAP enabled state: %s\n", softAPEnabled ? "true" : "false");
    }
    
    // Enable SoftAP by default on first boot
    if (softAPEnabled) {
        enableSoftAP();
    }
    
    // Auto-connect to saved network if credentials exist
    if (!stationSSID.isEmpty()) {
        Serial.println("[WiFi] Auto-connecting to saved network...");
        connectToNetwork(stationSSID.c_str(), stationPassword.c_str(), false);
    }
    
    Serial.println("[WiFi] WiFi Manager initialized successfully");
    return true;
}

// Enable SoftAP mode --- start Access Point with configured SSID and password -------------
bool WiFiManager::enableSoftAP() {
    Serial.println("[WiFi] Enabling SoftAP mode...");
    
    // CRITICAL: Set WiFi mode first (required before softAPConfig/softAP)
    // If Station is also enabled, use AP_STA mode, otherwise use AP mode
    if (stationEnabled) {
        WiFi.mode(WIFI_AP_STA);
    } else {
        WiFi.mode(WIFI_AP);
    }
    delay(100); // Give WiFi time to change mode
    
    // Configure IP address
    IPAddress local_ip;
    IPAddress gateway;
    IPAddress subnet;
    
    local_ip.fromString(WIFI_AP_IP);
    gateway.fromString(WIFI_AP_GATEWAY);
    subnet.fromString(WIFI_AP_SUBNET);
    
    // Configure SoftAP
    if (!WiFi.softAPConfig(local_ip, gateway, subnet)) {
        Serial.println("[WiFi] ✗ Failed to configure SoftAP IP");
        return false;
    }
    
    // Start SoftAP
    bool success = WiFi.softAP(
        softAPSSID.c_str(),
        softAPPassword.c_str(),
        WIFI_AP_CHANNEL,
        WIFI_AP_HIDDEN,
        WIFI_AP_MAX_CONNECTIONS
    );
    
    if (success) {
        softAPEnabled = true;
        updateMode(); // Update internal state
        
        Serial.println("[WiFi] ✓ SoftAP enabled successfully");
        Serial.printf("[WiFi]   SSID: %s\n", softAPSSID.c_str());
        Serial.printf("[WiFi]   Password: %s\n", softAPPassword.c_str());
        Serial.printf("[WiFi]   IP: %s\n", WiFi.softAPIP().toString().c_str());
        Serial.printf("[WiFi]   Channel: %d\n", WIFI_AP_CHANNEL);
        
        // Save SoftAP state to NVS
        Preferences prefs;
        if (prefs.begin(SETTINGS_NAMESPACE, false)) {
            prefs.putBool(SETTINGS_KEY_SOFTAP_ENABLED, true);
            prefs.end();
        }
        
        return true;
    } else {
        Serial.println("[WiFi] ✗ Failed to start SoftAP");
        return false;
    }
}

// Disable SoftAP mode --- stop Access Point -----------------------------------------------
void WiFiManager::disableSoftAP() {
    if (!softAPEnabled) {
        return;
    }
    
    Serial.println("[WiFi] Disabling SoftAP mode...");
    
    WiFi.softAPdisconnect(true);
    softAPEnabled = false;
    updateMode();
    
    // Save SoftAP state to NVS
    Preferences prefs;
    if (prefs.begin(SETTINGS_NAMESPACE, false)) {
        prefs.putBool(SETTINGS_KEY_SOFTAP_ENABLED, false);
        prefs.end();
    }
    
    Serial.println("[WiFi] ✓ SoftAP disabled");
}

// Toggle SoftAP --- switch SoftAP on/off (used by power button long press) ---------------
bool WiFiManager::toggleSoftAP() {
    if (softAPEnabled) {
        // Allow disabling SoftAP even if Station not connected (user may want to save power at night)
        // But warn user about potential lockout
        if (!isStationConnected()) {
            Serial.println("[WiFi] ⚠ WARNING: Disabling SoftAP without Station connection");
            Serial.println("[WiFi]   Device will be inaccessible until SoftAP is re-enabled");
            Serial.println("[WiFi]   To re-enable: Hold power button for 3 seconds again");
        }
        disableSoftAP();
        return false;
    } else {
        enableSoftAP();
        return true;
    }
}

// Connect to WiFi network --- attempt Station mode connection -----------------------------
bool WiFiManager::connectToNetwork(const char* ssid, const char* password, bool saveCredentials) {
    // Validate SSID
    if (ssid == nullptr || strlen(ssid) == 0 || strlen(ssid) > 32) {
        Serial.println("[WiFi] ✗ Invalid SSID (must be 1-32 characters)");
        return false;
    }
    
    // Validate password (8-63 chars for WPA2, or empty for open network)
    size_t passLen = (password != nullptr) ? strlen(password) : 0;
    if (passLen > 0 && (passLen < 8 || passLen > 63)) {
        Serial.println("[WiFi] ✗ Invalid password (must be 8-63 characters or empty)");
        return false;
    }
    
    Serial.printf("[WiFi] Connecting to network: %s\n", ssid);
    
    // Store credentials
    stationSSID = String(ssid);
    stationPassword = (password != nullptr) ? String(password) : "";
    
    // Save credentials if requested
    if (saveCredentials) {
        this->saveCredentials();
    }
    
    // Enable Station mode
    stationEnabled = true;
    updateMode();
    
    // Configure WiFi
    WiFi.begin(ssid, password);
    
    stationStatus = WiFiStatus::CONNECTING;
    lastConnectAttempt = millis();
    connectRetryCount = 0;
    
    Serial.println("[WiFi] Connection initiated...");
    return true;
}

// Disconnect from WiFi network --- disconnect Station mode --------------------------------
void WiFiManager::disconnectStation() {
    if (!stationEnabled) {
        return;
    }
    
    Serial.println("[WiFi] Disconnecting Station mode...");
    
    WiFi.disconnect(true);
    stationEnabled = false;
    stationStatus = WiFiStatus::DISCONNECTED;
    updateMode();
    
    Serial.println("[WiFi] ✓ Station disconnected");
}

// Update WiFi state --- call periodically to handle reconnection and events ---------------
void WiFiManager::update() {
    // Handle auto-reconnection
    if (stationEnabled && stationStatus == WiFiStatus::DISCONNECTED && autoReconnect) {
        if (!stationSSID.isEmpty()) {
            attemptReconnect();
        }
    }
    
    // Handle connection timeout
    if (stationStatus == WiFiStatus::CONNECTING) {
        if (millis() - lastConnectAttempt > WIFI_STA_CONNECT_TIMEOUT_MS) {
            Serial.println("[WiFi] ⚠ Connection timeout");
            stationStatus = WiFiStatus::FAILED;
            
            // Retry if under max retries
            if (connectRetryCount < WIFI_STA_MAX_RETRIES) {
                Serial.printf("[WiFi] Retrying connection (%d/%d)...\n", 
                             connectRetryCount + 1, WIFI_STA_MAX_RETRIES);
                attemptReconnect();
            } else {
                Serial.println("[WiFi] ✗ Max retries reached, giving up");
                stationStatus = WiFiStatus::DISCONNECTED;
            }
        }
    }
}

// Get SoftAP IP address --- returns Access Point IP ---------------------------------------
String WiFiManager::getSoftAPIP() const {
    if (softAPEnabled) {
        return WiFi.softAPIP().toString();
    }
    return "";
}

// Get Station IP address --- returns assigned IP from router ------------------------------
String WiFiManager::getStationIP() const {
    if (isStationConnected()) {
        return WiFi.localIP().toString();
    }
    return "";
}

// Get Station MAC address --- returns device MAC address as string ------------------------
String WiFiManager::getStationMAC() const {
    return WiFi.macAddress();
}

// Get number of connected clients --- returns count of devices connected to SoftAP --------
uint8_t WiFiManager::getConnectedClients() const {
    if (softAPEnabled) {
        return WiFi.softAPgetStationNum();
    }
    return 0;
}

// Check if WiFi credentials are saved --- returns true if SSID stored in NVS -------------
bool WiFiManager::hasStoredCredentials() const {
    Preferences prefs;
    if (prefs.begin(SETTINGS_NAMESPACE, true)) {
        bool hasSSID = prefs.isKey(SETTINGS_KEY_WIFI_SSID);
        prefs.end();
        return hasSSID;
    }
    return false;
}

// Clear stored WiFi credentials --- remove SSID and password from NVS --------------------
void WiFiManager::clearStoredCredentials() {
    Serial.println("[WiFi] Clearing stored credentials...");
    
    Preferences prefs;
    if (prefs.begin(SETTINGS_NAMESPACE, false)) {
        prefs.remove(SETTINGS_KEY_WIFI_SSID);
        prefs.remove(SETTINGS_KEY_WIFI_PASS);
        prefs.end();
        
        stationSSID = "";
        stationPassword = "";
        
        Serial.println("[WiFi] ✓ Credentials cleared");
    }
}

// Generate SoftAP SSID --- create unique SSID using MAC address suffix -------------------
String WiFiManager::generateSoftAPSSID() {
    // Get MAC address using WiFi.macAddress() which works even when WiFi is off
    String macString = WiFi.macAddress();
    
    // Extract last 4 hex digits from MAC (format: "XX:XX:XX:XX:XX:XX")
    // Last 4 digits are positions 12-15 and 15-17 (skip colons)
    String last4 = macString.substring(12, 14) + macString.substring(15, 17);
    last4.toUpperCase();
    
    // Create SSID with last 4 MAC digits
    char ssid[32];
    snprintf(ssid, sizeof(ssid), "%s%s", WIFI_AP_SSID_PREFIX, last4.c_str());
    
    return String(ssid);
}

// Load WiFi credentials from NVS --- retrieve stored SSID and password -------------------
bool WiFiManager::loadCredentials() {
    Preferences prefs;
    if (prefs.begin(SETTINGS_NAMESPACE, true)) {
        if (prefs.isKey(SETTINGS_KEY_WIFI_SSID)) {
            stationSSID = prefs.getString(SETTINGS_KEY_WIFI_SSID, "");
            stationPassword = prefs.getString(SETTINGS_KEY_WIFI_PASS, "");
            prefs.end();
            return !stationSSID.isEmpty();
        }
        prefs.end();
    }
    return false;
}

// Save WiFi credentials to NVS --- store SSID and password -------------------------------
bool WiFiManager::saveCredentials() {
    Serial.println("[WiFi] Saving credentials to NVS...");
    
    Preferences prefs;
    if (prefs.begin(SETTINGS_NAMESPACE, false)) {
        prefs.putString(SETTINGS_KEY_WIFI_SSID, stationSSID);
        prefs.putString(SETTINGS_KEY_WIFI_PASS, stationPassword);
        prefs.end();
        
        Serial.println("[WiFi] ✓ Credentials saved");
        return true;
    }
    
    Serial.println("[WiFi] ✗ Failed to save credentials");
    return false;
}

// Update WiFi mode --- reconfigure WiFi based on enabled flags ---------------------------
void WiFiManager::updateMode() {
    if (softAPEnabled && stationEnabled) {
        WiFi.mode(WIFI_AP_STA);
        currentMode = WiFiMode::AP_STA;
        Serial.println("[WiFi] Mode: AP+STA (Dual mode)");
    } else if (softAPEnabled) {
        WiFi.mode(WIFI_AP);
        currentMode = WiFiMode::AP_ONLY;
        Serial.println("[WiFi] Mode: AP only");
    } else if (stationEnabled) {
        WiFi.mode(WIFI_STA);
        currentMode = WiFiMode::STA_ONLY;
        Serial.println("[WiFi] Mode: STA only");
    } else {
        WiFi.mode(WIFI_OFF);
        currentMode = WiFiMode::OFF;
        Serial.println("[WiFi] Mode: OFF");
    }
}

// Attempt Station reconnection --- retry connection with exponential backoff -------------
void WiFiManager::attemptReconnect() {
    unsigned long now = millis();
    unsigned long timeSinceLastAttempt = now - lastConnectAttempt;
    
    // Calculate backoff delay
    unsigned long backoffDelay = WIFI_STA_RETRY_DELAY_MS * (1 << connectRetryCount);
    if (backoffDelay > 60000) backoffDelay = 60000; // Max 60 seconds
    
    if (timeSinceLastAttempt >= backoffDelay) {
        Serial.printf("[WiFi] Reconnecting to %s (attempt %d)...\n", 
                     stationSSID.c_str(), connectRetryCount + 1);
        
        WiFi.begin(stationSSID.c_str(), stationPassword.c_str());
        stationStatus = WiFiStatus::CONNECTING;
        lastConnectAttempt = now;
        connectRetryCount++;
    }
}

// WiFi event handlers --- static callbacks for ESP32 WiFi events -------------------------
void WiFiManager::onWiFiEvent(WiFiEvent_t event) {
    if (s_instance == nullptr) return;
    
    switch (event) {
        case ARDUINO_EVENT_WIFI_STA_START:
            Serial.println("[WiFi Event] Station started");
            break;
            
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            Serial.println("[WiFi Event] Connected to AP");
            break;
            
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            Serial.println("[WiFi Event] ✓ Got IP address");
            Serial.printf("[WiFi]   IP: %s\n", WiFi.localIP().toString().c_str());
            Serial.printf("[WiFi]   Gateway: %s\n", WiFi.gatewayIP().toString().c_str());
            Serial.printf("[WiFi]   Subnet: %s\n", WiFi.subnetMask().toString().c_str());
            s_instance->stationStatus = WiFiStatus::CONNECTED;
            s_instance->connectRetryCount = 0;
            break;
            
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("[WiFi Event] Disconnected from AP");
            if (s_instance->stationStatus == WiFiStatus::CONNECTED) {
                s_instance->stationStatus = WiFiStatus::DISCONNECTED;
            }
            break;
            
        case ARDUINO_EVENT_WIFI_AP_START:
            Serial.println("[WiFi Event] SoftAP started");
            break;
            
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            Serial.println("[WiFi Event] Client connected to SoftAP");
            Serial.printf("[WiFi]   Connected clients: %d\n", s_instance->getConnectedClients());
            break;
            
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            Serial.println("[WiFi Event] Client disconnected from SoftAP");
            Serial.printf("[WiFi]   Connected clients: %d\n", s_instance->getConnectedClients());
            break;
            
        default:
            break;
    }
}

