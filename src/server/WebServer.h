/**
 * WebServer.h - HTTP and WebSocket server for RODI Smart Mattress
 * 
 * Provides web-based control interface with:
 * - HTTP server for static files and REST API
 * - WebSocket server for real-time data streaming
 * - CORS support for browser compatibility
 * 
 * Features:
 * - Serves web panel HTML/CSS/JavaScript files
 * - REST API endpoints for device control
 * - Real-time sensor data push via WebSocket
 * - Multiple concurrent client connections
 * 
 * Phase 3: WiFi Network and Web Panel
 */

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <DNSServer.h>
#include "config/RodiConfig.h"

// Forward declarations for Phase 1 & 2 components
class HeatingController;
class HeatingTimer;
class SettingsStorage;
class RoomSensor;
class MattressSensor;

// WebSocket message type --- defines type of data being sent ----------------------------
enum class WSMessageType {
    SNAPSHOT,       // Full system state (sent on connection)
    UPDATE,         // Partial update (only changed values)
    ERROR,          // Error message
    PING            // Keep-alive ping
};

class RodiWebServer {
public:
    // Constructor --- initialize web server with component references --------------------
    // Parameters: references to Phase 1 & 2 components for control and monitoring
    RodiWebServer(
        HeatingController& heating,
        HeatingTimer& timer,
        SettingsStorage& settings,
        RoomSensor& roomSensor,
        MattressSensor& mattressSensor
    );
    
    // Destructor --- cleanup server resources --------------------------------------------
    ~RodiWebServer();
    
    // Initialize server --- start HTTP and WebSocket servers ----------------------------
    // Returns: true if initialization successful, false otherwise
    bool begin();
    
    // Stop server --- shutdown HTTP and WebSocket servers -------------------------------
    void stop();
    
    // Update server --- call periodically to handle client requests ---------------------
    // Should be called in main loop
    void update();
    
    // Send WebSocket update --- broadcast sensor data to all connected clients ----------
    // Parameters:
    //   type: message type (SNAPSHOT or UPDATE)
    //   forceUpdate: if true, send even if values haven't changed
    void sendWebSocketUpdate(WSMessageType type = WSMessageType::UPDATE, bool forceUpdate = false);
    
    // Check if server is running --- returns true if HTTP server is active --------------
    bool isRunning() const { return serverRunning; }
    
    // Get number of WebSocket clients --- returns count of connected WebSocket clients --
    uint8_t getWebSocketClientCount() const;

private:
    // Component references (Phase 1 & 2)
    HeatingController& heatingController;
    HeatingTimer& heatingTimer;
    SettingsStorage& settingsStorage;
    RoomSensor& roomSensor;
    MattressSensor& mattressSensor;
    
    // Server instances
    WebServer* httpServer;              // HTTP server for static files and API
    WebSocketsServer* wsServer;         // WebSocket server for real-time data
    DNSServer* dnsServer;               // DNS server for captive portal
    
    // Server state
    bool serverRunning;                 // Server running flag
    unsigned long lastWSUpdate;         // Last WebSocket update timestamp
    
    // Cached sensor values for change detection
    float lastRoomTemp;
    float lastRoomHumidity;
    float lastMattressTemp;
    float lastSetpoint;
    bool lastHeatingState;
    bool lastHeatingEnabled;
    uint32_t lastTimerRemaining;
    
    // HTTP request handlers --- static functions for HTTP routes ------------------------
    
    // Static file handlers
    static void handleRoot();           // Serve index.html
    static void handleNotFound();       // 404 error page
    static void handleCSS();            // Serve style.css
    static void handleJS();             // Serve app.js
    static void handleCaptivePortal(); // Captive portal redirect
    
    // REST API handlers
    static void handleGetStatus();      // GET /api/status - Get system status
    static void handlePostHeatingEnable();   // POST /api/heating/enable
    static void handlePostHeatingDisable();  // POST /api/heating/disable
    static void handlePostTemperature();     // POST /api/temperature
    static void handlePostTimer();           // POST /api/timer
    static void handlePostWiFi();            // POST /api/wifi - WiFi provisioning
    static void handleGetWiFi();             // GET /api/wifi - Get WiFi status
    static void handlePostSoftAP();          // POST /api/softap - Toggle SoftAP
    
    // Helper functions
    
    // Send CORS headers --- add Cross-Origin Resource Sharing headers -------------------
    void sendCORSHeaders();
    
    // Send JSON response --- send HTTP response with JSON content -----------------------
    // Parameters:
    //   code: HTTP status code
    //   json: JSON string to send
    void sendJSON(int code, const String& json);
    
    // Send error response --- send HTTP error with JSON error message -------------------
    // Parameters:
    //   code: HTTP status code
    //   message: error message
    void sendError(int code, const String& message);
    
    // Build status JSON --- create JSON string with full system status ------------------
    // Returns: JSON string with all sensor values and states
    String buildStatusJSON();
    
    // Build WebSocket message --- create JSON message for WebSocket ----------------------
    // Parameters:
    //   type: message type (SNAPSHOT or UPDATE)
    // Returns: JSON string for WebSocket transmission
    String buildWebSocketMessage(WSMessageType type);
    
    // Check if values changed --- compare current values with cached values -------------
    // Returns: true if any value has changed since last update
    bool hasValuesChanged();
    
    // Update cached values --- store current values for change detection ----------------
    void updateCachedValues();
    
    // WebSocket event handler --- handle WebSocket events (connect, disconnect, etc.) ---
    // Parameters:
    //   num: client number
    //   type: event type
    //   payload: event data
    //   length: payload length
    static void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
    
    // Static instance for callbacks
    static RodiWebServer* s_instance;
};

#endif // WEB_SERVER_H

