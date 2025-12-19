/**
 * WiFiManager.h - WiFi network management for RODI Smart Mattress
 * 
 * Manages WiFi connectivity in multiple modes:
 * - SoftAP (Access Point) mode for direct device connection
 * - Station mode for home network integration
 * - Dual-mode operation (SoftAP + Station simultaneously)
 * 
 * Features:
 * - Automatic WiFi initialization and mode switching
 * - WiFi credentials storage and retrieval
 * - Connection status monitoring
 * - Event-driven architecture
 * 
 * Phase 3: WiFi Network and Web Panel
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include "config/RodiConfig.h"

// WiFi mode enumeration --- represents current WiFi operational state -------------------
enum class WiFiMode {
    OFF,        // WiFi disabled
    AP_ONLY,    // SoftAP mode only
    STA_ONLY,   // Station mode only
    AP_STA      // Dual mode (both AP and Station)
};

// WiFi connection status --- represents Station mode connection state -------------------
enum class WiFiStatus {
    DISCONNECTED,   // Not connected to any network
    CONNECTING,     // Attempting to connect
    CONNECTED,      // Successfully connected
    FAILED          // Connection failed
};

class WiFiManager {
public:
    // Constructor --- initializes WiFi manager with default state -------------------------
    WiFiManager();
    
    // Destructor --- cleanup WiFi resources -----------------------------------------------
    ~WiFiManager();
    
    // Initialize WiFi --- setup WiFi radio and load saved credentials ---------------------
    // Returns: true if initialization successful, false otherwise
    bool begin();
    
    // Enable SoftAP mode --- start Access Point with configured SSID and password ---------
    // Returns: true if SoftAP started successfully, false otherwise
    bool enableSoftAP();
    
    // Disable SoftAP mode --- stop Access Point -------------------------------------------
    void disableSoftAP();
    
    // Toggle SoftAP --- switch SoftAP on/off (used by power button long press) -----------
    // Returns: new SoftAP state (true = enabled, false = disabled)
    bool toggleSoftAP();
    
    // Connect to WiFi network --- attempt Station mode connection -------------------------
    // Parameters:
    //   ssid: WiFi network name (1-32 characters)
    //   password: WiFi password (8-63 characters for WPA2, empty for open network)
    //   saveCredentials: if true, store credentials in NVS for auto-reconnect
    // Returns: true if connection initiated, false if parameters invalid
    bool connectToNetwork(const char* ssid, const char* password, bool saveCredentials = true);
    
    // Disconnect from WiFi network --- disconnect Station mode ----------------------------
    void disconnectStation();
    
    // Update WiFi state --- call periodically to handle reconnection and events -----------
    void update();
    
    // Get current WiFi mode --- returns current operational mode --------------------------
    WiFiMode getMode() const { return currentMode; }
    
    // Get Station connection status --- returns current connection state ------------------
    WiFiStatus getStationStatus() const { return stationStatus; }
    
    // Check if SoftAP is enabled --- returns true if Access Point is active --------------
    bool isSoftAPEnabled() const { return softAPEnabled; }
    
    // Check if Station is connected --- returns true if connected to network -------------
    bool isStationConnected() const { return stationStatus == WiFiStatus::CONNECTED; }
    
    // Get SoftAP SSID --- returns Access Point network name ------------------------------
    String getSoftAPSSID() const { return softAPSSID; }
    
    // Get SoftAP IP address --- returns Access Point IP (e.g., "192.168.4.1") -----------
    String getSoftAPIP() const;
    
    // Get Station SSID --- returns connected network name (empty if not connected) -------
    String getStationSSID() const { return stationSSID; }
    
    // Get Station IP address --- returns assigned IP from router (empty if not connected) -
    String getStationIP() const;
    
    // Get Station MAC address --- returns device MAC address as string -------------------
    String getStationMAC() const;
    
    // Get number of connected clients --- returns count of devices connected to SoftAP ---
    uint8_t getConnectedClients() const;
    
    // Check if WiFi credentials are saved --- returns true if SSID stored in NVS ---------
    bool hasStoredCredentials() const;
    
    // Clear stored WiFi credentials --- remove SSID and password from NVS ----------------
    void clearStoredCredentials();

private:
    // WiFi state variables
    WiFiMode currentMode;           // Current WiFi operational mode
    WiFiStatus stationStatus;       // Station connection status
    bool softAPEnabled;             // SoftAP enabled flag
    bool stationEnabled;            // Station enabled flag
    
    // SoftAP configuration
    String softAPSSID;              // SoftAP network name
    String softAPPassword;          // SoftAP password
    
    // Station configuration
    String stationSSID;             // Station network name
    String stationPassword;         // Station password
    
    // Connection management
    unsigned long lastConnectAttempt;   // Timestamp of last connection attempt
    uint8_t connectRetryCount;          // Number of connection retry attempts
    bool autoReconnect;                 // Auto-reconnect enabled flag
    
    // Generate SoftAP SSID --- create unique SSID using MAC address suffix ---------------
    // Returns: SSID string in format "RODI_XXXX" where XXXX = last 4 MAC digits
    String generateSoftAPSSID();
    
    // Load WiFi credentials from NVS --- retrieve stored SSID and password ---------------
    // Returns: true if credentials found, false otherwise
    bool loadCredentials();
    
    // Save WiFi credentials to NVS --- store SSID and password ---------------------------
    // Returns: true if save successful, false otherwise
    bool saveCredentials();
    
    // Update WiFi mode --- reconfigure WiFi based on enabled flags -----------------------
    void updateMode();
    
    // Handle Station events --- process WiFi connection events ---------------------------
    void handleStationEvents();
    
    // Attempt Station reconnection --- retry connection with exponential backoff ---------
    void attemptReconnect();
    
    // WiFi event handlers --- static callbacks for ESP32 WiFi events ---------------------
    static void onWiFiEvent(WiFiEvent_t event);
};

#endif // WIFI_MANAGER_H

