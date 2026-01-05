/**
 * WebServer.cpp - HTTP and WebSocket server implementation
 * 
 * Implements web-based control interface with REST API and real-time WebSocket.
 * Serves static web panel files and handles device control commands.
 * 
 * Phase 3: WiFi Network and Web Panel
 */

#include "WebServer.h"
#include "heating/HeatingController.h"
#include "timer/HeatingTimer.h"
#include "storage/SettingsStorage.h"
#include "sensors/RoomSensor.h"
#include "sensors/MattressSensor.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>

// Static instance for callbacks
RodiWebServer* RodiWebServer::s_instance = nullptr;

// Constructor --- initialize web server with component references ------------------------
RodiWebServer::RodiWebServer(
    HeatingController& heating,
    HeatingTimer& timer,
    SettingsStorage& settings,
    RoomSensor& roomSensor,
    MattressSensor& mattressSensor
) : heatingController(heating),
    heatingTimer(timer),
    settingsStorage(settings),
    roomSensor(roomSensor),
    mattressSensor(mattressSensor),
    httpServer(nullptr),
    wsServer(nullptr),
    dnsServer(nullptr),
    serverRunning(false),
    lastWSUpdate(0),
    lastRoomTemp(0),
    lastRoomHumidity(0),
    lastMattressTemp(0),
    lastSetpoint(0),
    lastHeatingState(false),
    lastHeatingEnabled(false),
    lastTimerRemaining(0)
{
    s_instance = this;
}

// Destructor --- cleanup server resources ------------------------------------------------
RodiWebServer::~RodiWebServer() {
    stop();
    s_instance = nullptr;
}

// Initialize server --- start HTTP and WebSocket servers --------------------------------
bool RodiWebServer::begin() {
    Serial.println("[WebServer] Initializing web server...");
    
    // Initialize SPIFFS for static files
    Serial.println("[WebServer] Mounting SPIFFS (this may take 10-15 seconds)...");
    
    // Note: SPIFFS.begin() can take a long time on first boot
    // We'll try to mount it, but if it fails, we continue with API only
    bool spiffsOk = false;
    
    // Try to mount SPIFFS (format on fail = true)
    // This can take up to 15 seconds on first boot
    Serial.println("[WebServer] Attempting SPIFFS mount...");
    spiffsOk = SPIFFS.begin(true);
    
    if (!spiffsOk) {
        Serial.println("[WebServer] ⚠ Failed to mount SPIFFS");
        Serial.println("[WebServer] Web panel files not available");
        Serial.println("[WebServer] Please run: pio run --target uploadfs");
        Serial.println("[WebServer] Continuing without web panel...");
        // Don't return false - continue with API only
    } else {
        Serial.println("[WebServer] ✓ SPIFFS mounted successfully");
        
        // List files in SPIFFS for debugging
        File root = SPIFFS.open("/");
        if (root) {
            File file = root.openNextFile();
            Serial.println("[WebServer] Files in SPIFFS:");
            while (file) {
                Serial.printf("[WebServer]   - %s (%d bytes)\n", file.name(), file.size());
                file = root.openNextFile();
            }
        }
    }
    
    // Create HTTP server
    httpServer = new WebServer(HTTP_SERVER_PORT);
    
    // Register static file handlers
    httpServer->on("/", HTTP_GET, handleRoot);
    httpServer->on("/index.html", HTTP_GET, handleRoot);
    httpServer->on("/style.css", HTTP_GET, handleCSS);
    httpServer->on("/app.js", HTTP_GET, handleJS);
    
    // Register favicon handler (return 204 No Content to avoid 404)
    httpServer->on("/favicon.ico", HTTP_GET, []() {
        if (s_instance && s_instance->httpServer) {
            s_instance->httpServer->send(204, "image/x-icon", "");
        }
    });
    
    // Register captive portal handlers (for automatic redirect)
    httpServer->on("/generate_204", HTTP_GET, handleCaptivePortal);  // Android captive portal check
    httpServer->on("/gen_204", HTTP_GET, handleCaptivePortal);      // Android alternative
    httpServer->on("/hotspot-detect.html", HTTP_GET, handleCaptivePortal);  // iOS captive portal check
    httpServer->on("/canonical.html", HTTP_GET, handleCaptivePortal);       // iOS alternative
    httpServer->on("/success.txt", HTTP_GET, handleCaptivePortal);           // Windows captive portal check
    httpServer->on("/ncsi.txt", HTTP_GET, handleCaptivePortal);            // Windows alternative
    
    // Register REST API handlers
    httpServer->on("/api/status", HTTP_GET, handleGetStatus);
    httpServer->on("/api/heating/enable", HTTP_POST, handlePostHeatingEnable);
    httpServer->on("/api/heating/disable", HTTP_POST, handlePostHeatingDisable);
    httpServer->on("/api/temperature", HTTP_POST, handlePostTemperature);
    httpServer->on("/api/timer", HTTP_POST, handlePostTimer);
    httpServer->on("/api/wifi", HTTP_GET, handleGetWiFi);
    httpServer->on("/api/wifi", HTTP_POST, handlePostWiFi);
    httpServer->on("/api/softap", HTTP_POST, handlePostSoftAP);
    
    // Register 404 handler
    httpServer->onNotFound(handleNotFound);
    
    // CRITICAL: Verify WiFi is active before starting HTTP server
    // This prevents "tcpip_send_msg_wait_sem (Invalid mbox)" error
    wifi_mode_t wifiMode = WiFi.getMode();
    if (wifiMode == WIFI_OFF) {
        Serial.println("[WebServer] ✗ CRITICAL ERROR: WiFi mode is OFF!");
        Serial.println("[WebServer]   HTTP server cannot start without WiFi");
        Serial.println("[WebServer]   This will cause a crash - aborting server start");
        return false; // Don't start server if WiFi is off
    }
    
    Serial.printf("[WebServer] WiFi mode: %d (should not be 0=WIFI_OFF)\n", wifiMode);
    
    // Additional delay to ensure TCP/IP stack is fully ready
    // Even if WiFi mode is set, TCP/IP stack needs time to initialize
    delay(1000);
    
    // Start HTTP server
    Serial.println("[WebServer] Starting HTTP server...");
    httpServer->begin();
    Serial.printf("[WebServer] ✓ HTTP server started on port %d\n", HTTP_SERVER_PORT);
    
    // Create and start DNS server for captive portal
    // This redirects all DNS queries to our IP address
    dnsServer = new DNSServer();
    IPAddress apIP(192, 168, 4, 1);
    if (dnsServer->start(53, "*", apIP)) {
        Serial.println("[WebServer] ✓ DNS server started on port 53 (Captive Portal)");
    } else {
        Serial.println("[WebServer] ⚠ Failed to start DNS server");
    }
    
    // Create WebSocket server
    wsServer = new WebSocketsServer(81); // WebSocket on port 81
    wsServer->begin();
    wsServer->onEvent(onWebSocketEvent);
    Serial.println("[WebServer] ✓ WebSocket server started on port 81");
    
    serverRunning = true;
    
    Serial.println("[WebServer] Web server initialized successfully");
    return true;
}

// Stop server --- shutdown HTTP and WebSocket servers -----------------------------------
void RodiWebServer::stop() {
    if (!serverRunning) {
        return;
    }
    
    Serial.println("[WebServer] Stopping web server...");
    
    if (wsServer) {
        wsServer->close();
        delete wsServer;
        wsServer = nullptr;
    }
    
    if (httpServer) {
        httpServer->stop();
        delete httpServer;
        httpServer = nullptr;
    }
    
    if (dnsServer) {
        dnsServer->stop();
        delete dnsServer;
        dnsServer = nullptr;
    }
    
    SPIFFS.end();
    
    serverRunning = false;
    Serial.println("[WebServer] ✓ Web server stopped");
}

// Update server --- call periodically to handle client requests -------------------------
void RodiWebServer::update() {
    if (!serverRunning) {
        return;
    }
    
    // Handle DNS requests (for captive portal)
    if (dnsServer) {
        dnsServer->processNextRequest();
    }
    
    // Handle HTTP requests
    if (httpServer) {
        httpServer->handleClient();
    }
    
    // Handle WebSocket events
    if (wsServer) {
        wsServer->loop();
    }
    
    // Send periodic WebSocket updates (only if values changed)
    unsigned long now = millis();
    if (now - lastWSUpdate >= 1000) { // Check every second
        lastWSUpdate = now;
        
        if (hasValuesChanged()) {
            sendWebSocketUpdate(WSMessageType::UPDATE, false);
        }
    }
}

// Send WebSocket update --- broadcast sensor data to all connected clients --------------
void RodiWebServer::sendWebSocketUpdate(WSMessageType type, bool forceUpdate) {
    if (!serverRunning || !wsServer) {
        return;
    }
    
    // Don't send if no clients connected
    if (getWebSocketClientCount() == 0) {
        return;
    }
    
    // For UPDATE type, check if values changed (unless forced)
    if (type == WSMessageType::UPDATE && !forceUpdate) {
        if (!hasValuesChanged()) {
            return; // No changes, don't send empty update
        }
    }
    
    // Build message
    String message = buildWebSocketMessage(type);
    
    // For UPDATE type, check if data object is empty
    // Simple check: if message doesn't contain any data fields (just "data":{}), don't send
    if (type == WSMessageType::UPDATE) {
        // Check if data object has any content (more than just "data":{})
        int dataStart = message.indexOf("\"data\":{");
        if (dataStart >= 0) {
            int dataEnd = message.indexOf("}", dataStart + 8);
            if (dataEnd >= 0 && dataEnd == dataStart + 8) {
                // Empty data object: "data":{}
                return; // Don't send empty update
            }
        }
    }
    
    // Broadcast to all clients
    wsServer->broadcastTXT(message);
    
    // Update cached values
    updateCachedValues();
}

// Get number of WebSocket clients --- returns count of connected WebSocket clients ------
uint8_t RodiWebServer::getWebSocketClientCount() const {
    if (wsServer) {
        return wsServer->connectedClients();
    }
    return 0;
}

// ============================
// HTTP Request Handlers
// ============================

// Serve index.html --- main web panel page ----------------------------------------------
void RodiWebServer::handleRoot() {
    if (!s_instance || !s_instance->httpServer) return;
    
    Serial.println("[WebServer] Serving index.html");
    
    // Check if SPIFFS file exists (don't remount if already mounted)
    if (!SPIFFS.exists("/index.html")) {
        if (!SPIFFS.begin(true)) {
            String html = "<html><body><h1>RODI Smart Mattress</h1>";
            html += "<p>Web panel files not uploaded.</p>";
            html += "<p>Please run: <code>pio run --target uploadfs</code></p>";
            html += "<p>API endpoints are still available at /api/*</p></body></html>";
            s_instance->httpServer->send(200, "text/html", html);
            return;
        }
    }
    
    File file = SPIFFS.open("/index.html", "r");
    if (!file) {
        s_instance->sendError(500, "Failed to open index.html - please upload SPIFFS files");
        return;
    }
    
    s_instance->httpServer->streamFile(file, "text/html");
    file.close();
}

// Serve style.css --- web panel stylesheet ----------------------------------------------
void RodiWebServer::handleCSS() {
    if (!s_instance || !s_instance->httpServer) return;
    
    Serial.println("[WebServer] Serving style.css");
    
    // Check if SPIFFS file exists (don't remount if already mounted)
    if (!SPIFFS.exists("/style.css")) {
        if (!SPIFFS.begin(true)) {
            s_instance->sendError(404, "SPIFFS not mounted");
            return;
        }
    }
    
    File file = SPIFFS.open("/style.css", "r");
    if (!file) {
        s_instance->sendError(404, "File not found - please upload SPIFFS files");
        return;
    }
    
    s_instance->httpServer->streamFile(file, "text/css");
    file.close();
}

// Serve app.js --- web panel JavaScript -------------------------------------------------
void RodiWebServer::handleJS() {
    if (!s_instance || !s_instance->httpServer) return;
    
    Serial.println("[WebServer] Serving app.js");
    
    // Check if SPIFFS file exists (don't remount if already mounted)
    if (!SPIFFS.exists("/app.js")) {
        if (!SPIFFS.begin(true)) {
            s_instance->sendError(404, "SPIFFS not mounted");
            return;
        }
    }
    
    File file = SPIFFS.open("/app.js", "r");
    if (!file) {
        s_instance->sendError(404, "File not found - please upload SPIFFS files");
        return;
    }
    
    s_instance->httpServer->streamFile(file, "application/javascript");
    file.close();
}

// Handle Captive Portal --- redirect all captive portal checks to main page ----------------
void RodiWebServer::handleCaptivePortal() {
    if (!s_instance || !s_instance->httpServer) return;
    
    Serial.println("[WebServer] Captive portal request - redirecting to main page");
    
    // Redirect to main page (302 Found)
    s_instance->httpServer->sendHeader("Location", "/", true);
    s_instance->httpServer->send(302, "text/plain", "Redirecting...");
}

// Handle 404 Not Found -------------------------------------------------------------------
void RodiWebServer::handleNotFound() {
    if (!s_instance || !s_instance->httpServer) return;
    
    Serial.printf("[WebServer] 404 Not Found: %s\n", 
                 s_instance->httpServer->uri().c_str());
    
    s_instance->sendError(404, "Not Found");
}

// GET /api/status --- Get full system status --------------------------------------------
void RodiWebServer::handleGetStatus() {
    if (!s_instance) return;
    
    Serial.println("[WebServer] GET /api/status");
    
    String json = s_instance->buildStatusJSON();
    s_instance->sendJSON(200, json);
}

// POST /api/heating/enable --- Enable heating system ------------------------------------
void RodiWebServer::handlePostHeatingEnable() {
    if (!s_instance) return;
    
    Serial.println("[WebServer] POST /api/heating/enable");
    
    s_instance->heatingController.enable();
    s_instance->settingsStorage.setHeatingEnabled(true);
    s_instance->settingsStorage.save();
    
    String json = "{\"success\":true,\"heating_on\":true}";
    s_instance->sendJSON(200, json);
    
    // Broadcast update
    s_instance->sendWebSocketUpdate(WSMessageType::UPDATE, true);
}

// POST /api/heating/disable --- Disable heating system ----------------------------------
void RodiWebServer::handlePostHeatingDisable() {
    if (!s_instance) return;
    
    Serial.println("[WebServer] POST /api/heating/disable");
    
    s_instance->heatingController.disable();
    s_instance->settingsStorage.setHeatingEnabled(false);
    s_instance->settingsStorage.save();
    
    String json = "{\"success\":true,\"heating_on\":false}";
    s_instance->sendJSON(200, json);
    
    // Broadcast update
    s_instance->sendWebSocketUpdate(WSMessageType::UPDATE, true);
}

// POST /api/temperature --- Set temperature setpoint ------------------------------------
void RodiWebServer::handlePostTemperature() {
    if (!s_instance || !s_instance->httpServer) return;
    
    Serial.println("[WebServer] POST /api/temperature");
    
    // Parse JSON body
    String body = s_instance->httpServer->arg("plain");
    
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);
    
    if (error) {
        s_instance->sendError(400, "Invalid JSON");
        return;
    }
    
    if (!doc.containsKey("setpoint")) {
        s_instance->sendError(400, "Missing 'setpoint' field");
        return;
    }
    
    float setpoint = doc["setpoint"];
    
    // Validate range
    if (setpoint < TEMP_MIN_C || setpoint > TEMP_MAX_C) {
        String errorMsg = "Temperature out of range (" + String(TEMP_MIN_C) + "-" + String(TEMP_MAX_C) + "°C)";
        s_instance->sendError(400, errorMsg);
        return;
    }
    
    // Apply setpoint
    s_instance->heatingController.setSetpoint(setpoint);
    s_instance->settingsStorage.setTemperatureSetpoint(setpoint);
    s_instance->settingsStorage.save();
    
    String json = "{\"success\":true,\"setpoint\":" + String(setpoint, 1) + "}";
    s_instance->sendJSON(200, json);
    
    // Broadcast update
    s_instance->sendWebSocketUpdate(WSMessageType::UPDATE, true);
}

// POST /api/timer --- Set timer duration -------------------------------------------------
void RodiWebServer::handlePostTimer() {
    if (!s_instance || !s_instance->httpServer) return;
    
    Serial.println("[WebServer] POST /api/timer");
    
    // Parse JSON body
    String body = s_instance->httpServer->arg("plain");
    
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);
    
    if (error) {
        s_instance->sendError(400, "Invalid JSON");
        return;
    }
    
    if (!doc.containsKey("duration_min")) {
        s_instance->sendError(400, "Missing 'duration_min' field");
        return;
    }
    
    uint32_t duration = doc["duration_min"];
    
    // Validate range
    if (duration < TIMER_MIN_MIN || duration > TIMER_MAX_MIN) {
        String errorMsg = "Timer out of range (" + String(TIMER_MIN_MIN) + "-" + String(TIMER_MAX_MIN) + " minutes)";
        s_instance->sendError(400, errorMsg);
        return;
    }
    
    // Apply timer
    s_instance->heatingTimer.setDuration(duration);
    s_instance->settingsStorage.setTimerDuration(duration);
    s_instance->settingsStorage.save();
    
    String json = "{\"success\":true,\"duration_min\":" + String(duration) + "}";
    s_instance->sendJSON(200, json);
    
    // Broadcast update
    s_instance->sendWebSocketUpdate(WSMessageType::UPDATE, true);
}

// POST /api/wifi --- WiFi provisioning (Station mode) -----------------------------------
void RodiWebServer::handlePostWiFi() {
    if (!s_instance || !s_instance->httpServer) return;
    
    Serial.println("[WebServer] POST /api/wifi");
    
    // Parse JSON body
    String body = s_instance->httpServer->arg("plain");
    
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);
    
    if (error) {
        s_instance->sendError(400, "Invalid JSON");
        return;
    }
    
    if (!doc.containsKey("ssid")) {
        s_instance->sendError(400, "Missing 'ssid' field");
        return;
    }
    
    String ssid = doc["ssid"].as<String>();
    String password = doc.containsKey("password") ? doc["password"].as<String>() : "";
    
    // Note: WiFi provisioning will be handled by WiFiManager in main.cpp
    // For now, just return success
    
    String json = "{\"success\":true,\"message\":\"WiFi provisioning not yet implemented\"}";
    s_instance->sendJSON(200, json);
}

// GET /api/wifi --- Get WiFi status ------------------------------------------------------
void RodiWebServer::handleGetWiFi() {
    if (!s_instance) return;
    
    Serial.println("[WebServer] GET /api/wifi");
    
    // Note: WiFi status will be provided by WiFiManager in main.cpp
    // For now, return placeholder
    
    String json = "{\"mode\":\"AP\",\"ssid\":\"\",\"connected\":false,\"ip\":\"\"}";
    s_instance->sendJSON(200, json);
}

// POST /api/softap --- Toggle SoftAP mode ------------------------------------------------
void RodiWebServer::handlePostSoftAP() {
    if (!s_instance || !s_instance->httpServer) return;
    
    Serial.println("[WebServer] POST /api/softap");
    
    // Parse JSON body
    String body = s_instance->httpServer->arg("plain");
    
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);
    
    if (error) {
        s_instance->sendError(400, "Invalid JSON");
        return;
    }
    
    if (!doc.containsKey("enabled")) {
        s_instance->sendError(400, "Missing 'enabled' field");
        return;
    }
    
    bool enabled = doc["enabled"];
    
    // Note: SoftAP toggle will be handled by WiFiManager in main.cpp
    // For now, just return success
    
    String json = "{\"success\":true,\"softap_enabled\":" + String(enabled ? "true" : "false") + "}";
    s_instance->sendJSON(200, json);
}

// ============================
// Helper Functions
// ============================

// Send CORS headers --- add Cross-Origin Resource Sharing headers -----------------------
void RodiWebServer::sendCORSHeaders() {
    if (!httpServer) return;
    
    httpServer->sendHeader("Access-Control-Allow-Origin", "*");
    httpServer->sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    httpServer->sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

// Send JSON response --- send HTTP response with JSON content ---------------------------
void RodiWebServer::sendJSON(int code, const String& json) {
    if (!httpServer) return;
    
    sendCORSHeaders();
    httpServer->send(code, "application/json", json);
}

// Send error response --- send HTTP error with JSON error message -----------------------
void RodiWebServer::sendError(int code, const String& message) {
    String json = "{\"success\":false,\"error\":\"" + message + "\"}";
    sendJSON(code, json);
}

// Build status JSON --- create JSON string with full system status ----------------------
String RodiWebServer::buildStatusJSON() {
    StaticJsonDocument<512> doc;
    
    doc["room_temp"] = roomSensor.getTemperature();
    doc["room_humidity"] = roomSensor.getHumidity();
    doc["mattress_temp"] = mattressSensor.getTemperature();
    doc["setpoint"] = settingsStorage.getTemperatureSetpoint();
    doc["heating_on"] = heatingController.isHeating();
    doc["heating_enabled"] = heatingController.isEnabled();
    doc["timer_remaining_sec"] = heatingTimer.getRemainingSeconds();
    doc["timer_duration_min"] = settingsStorage.getTimerDuration();
    doc["timer_running"] = heatingTimer.isRunning();
    doc["mattress_sensor_ok"] = mattressSensor.isHealthy();
    doc["room_sensor_ok"] = roomSensor.isHealthy();
    doc["timestamp"] = millis() / 1000;
    
    String json;
    serializeJson(doc, json);
    return json;
}

// Build WebSocket message --- create JSON message for WebSocket --------------------------
String RodiWebServer::buildWebSocketMessage(WSMessageType type) {
    StaticJsonDocument<512> doc;
    
    if (type == WSMessageType::SNAPSHOT) {
        doc["type"] = "snapshot";
        
        // Include all data
        JsonObject data = doc.createNestedObject("data");
        data["room_temp"] = roomSensor.getTemperature();
        data["room_humidity"] = roomSensor.getHumidity();
        data["mattress_temp"] = mattressSensor.getTemperature();
        data["setpoint"] = settingsStorage.getTemperatureSetpoint();
        data["heating_on"] = heatingController.isHeating();
        data["heating_enabled"] = heatingController.isEnabled();
        data["timer_remaining_sec"] = heatingTimer.getRemainingSeconds();
        data["timer_duration_min"] = settingsStorage.getTimerDuration();
        data["timer_running"] = heatingTimer.isRunning();
        data["wifi_mode"] = "AP"; // Placeholder
        
    } else if (type == WSMessageType::UPDATE) {
        doc["type"] = "update";
        
        // Include only changed values
        JsonObject data = doc.createNestedObject("data");
        
        float roomTemp = roomSensor.getTemperature();
        if (roomTemp != lastRoomTemp) {
            data["room_temp"] = roomTemp;
        }
        
        float roomHumidity = roomSensor.getHumidity();
        if (roomHumidity != lastRoomHumidity) {
            data["room_humidity"] = roomHumidity;
        }
        
        float mattressTemp = mattressSensor.getTemperature();
        if (mattressTemp != lastMattressTemp) {
            data["mattress_temp"] = mattressTemp;
        }
        
        float setpoint = settingsStorage.getTemperatureSetpoint();
        if (setpoint != lastSetpoint) {
            data["setpoint"] = setpoint;
        }
        
        bool heatingState = heatingController.isHeating();
        if (heatingState != lastHeatingState) {
            data["heating_on"] = heatingState;
        }
        
        bool heatingEnabled = heatingController.isEnabled();
        if (heatingEnabled != lastHeatingEnabled) {
            data["heating_enabled"] = heatingEnabled;
        }
        
        uint32_t timerRemaining = heatingTimer.getRemainingSeconds();
        if (timerRemaining != lastTimerRemaining) {
            data["timer_remaining_sec"] = timerRemaining;
        }
    }
    
    doc["timestamp"] = millis() / 1000;
    
    String json;
    serializeJson(doc, json);
    return json;
}

// Check if values changed --- compare current values with cached values -----------------
bool RodiWebServer::hasValuesChanged() {
    return (roomSensor.getTemperature() != lastRoomTemp ||
            roomSensor.getHumidity() != lastRoomHumidity ||
            mattressSensor.getTemperature() != lastMattressTemp ||
            settingsStorage.getTemperatureSetpoint() != lastSetpoint ||
            heatingController.isHeating() != lastHeatingState ||
            heatingController.isEnabled() != lastHeatingEnabled ||
            heatingTimer.getRemainingSeconds() != lastTimerRemaining);
}

// Update cached values --- store current values for change detection --------------------
void RodiWebServer::updateCachedValues() {
    lastRoomTemp = roomSensor.getTemperature();
    lastRoomHumidity = roomSensor.getHumidity();
    lastMattressTemp = mattressSensor.getTemperature();
    lastSetpoint = settingsStorage.getTemperatureSetpoint();
    lastHeatingState = heatingController.isHeating();
    lastHeatingEnabled = heatingController.isEnabled();
    lastTimerRemaining = heatingTimer.getRemainingSeconds();
}

// WebSocket event handler --- handle WebSocket events (connect, disconnect, etc.) -------
void RodiWebServer::onWebSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    if (!s_instance) return;
    
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WebSocket] Client #%u disconnected\n", num);
            break;
            
        case WStype_CONNECTED:
            {
                Serial.printf("[WebSocket] Client #%u connected\n", num);
                
                // Send full snapshot to new client
                String snapshot = s_instance->buildWebSocketMessage(WSMessageType::SNAPSHOT);
                s_instance->wsServer->sendTXT(num, snapshot);
            }
            break;
            
        case WStype_TEXT:
            Serial.printf("[WebSocket] Received text from client #%u: %s\n", num, payload);
            // Handle incoming messages if needed
            break;
            
        default:
            break;
    }
}

