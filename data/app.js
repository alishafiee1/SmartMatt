/**
 * RODI Smart Mattress - Web Panel JavaScript
 * 
 * Handles:
 * - WebSocket connection for real-time updates
 * - REST API calls for device control
 * - UI updates and user interactions
 * - Client-side timer countdown
 * 
 * Phase 3: WiFi Network and Web Panel
 */

// ============================
// Global Variables
// ============================

let ws = null;
let wsReconnectAttempts = 0;
const WS_MAX_RECONNECT_ATTEMPTS = 5;
const WS_RECONNECT_DELAY = 3000;

let timerInterval = null;
let timerRemainingSeconds = 0;
let timerDurationMinutes = 0;

// ============================
// Initialization
// ============================

document.addEventListener('DOMContentLoaded', function() {
    console.log('[App] Initializing RODI Web Panel...');
    
    // Initialize WebSocket connection
    initWebSocket();
    
    // Setup event listeners
    setupEventListeners();
    
    // Fetch initial status
    fetchStatus();
    
    console.log('[App] Initialization complete');
});

// ============================
// WebSocket Functions
// ============================

function initWebSocket() {
    const wsProtocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
    const wsHost = window.location.hostname;
    const wsPort = 81; // WebSocket port
    const wsUrl = `${wsProtocol}//${wsHost}:${wsPort}`;
    
    console.log(`[WebSocket] Connecting to ${wsUrl}...`);
    
    try {
        ws = new WebSocket(wsUrl);
        
        ws.onopen = function() {
            console.log('[WebSocket] Connected');
            wsReconnectAttempts = 0;
            updateConnectionStatus(true);
        };
        
        ws.onmessage = function(event) {
            try {
                const data = JSON.parse(event.data);
                handleWebSocketMessage(data);
            } catch (error) {
                console.error('[WebSocket] Failed to parse message:', error);
            }
        };
        
        ws.onerror = function(error) {
            console.error('[WebSocket] Error:', error);
        };
        
        ws.onclose = function() {
            console.log('[WebSocket] Disconnected');
            updateConnectionStatus(false);
            
            // Attempt reconnection
            if (wsReconnectAttempts < WS_MAX_RECONNECT_ATTEMPTS) {
                wsReconnectAttempts++;
                console.log(`[WebSocket] Reconnecting in ${WS_RECONNECT_DELAY/1000}s (attempt ${wsReconnectAttempts}/${WS_MAX_RECONNECT_ATTEMPTS})...`);
                setTimeout(initWebSocket, WS_RECONNECT_DELAY);
            } else {
                showError('اتصال WebSocket قطع شد. لطفا صفحه را رفرش کنید.');
            }
        };
        
    } catch (error) {
        console.error('[WebSocket] Failed to create connection:', error);
        showError('خطا در برقراری ارتباط با دستگاه');
    }
}

function handleWebSocketMessage(data) {
    console.log('[WebSocket] Received:', data);
    
    if (data.type === 'snapshot' || data.type === 'update') {
        updateUI(data.data);
    } else if (data.type === 'error') {
        showError(data.message || 'خطای ناشناخته');
    }
}

function updateConnectionStatus(connected) {
    const wifiStatus = document.getElementById('wifi-status');
    const wifiIcon = document.getElementById('wifi-icon');
    
    if (connected) {
        wifiStatus.textContent = 'متصل';
        wifiIcon.textContent = '📶';
    } else {
        wifiStatus.textContent = 'قطع شده';
        wifiIcon.textContent = '📵';
    }
}

// ============================
// REST API Functions
// ============================

async function fetchStatus() {
    try {
        const response = await fetch('/api/status');
        const data = await response.json();
        
        if (data) {
            updateUI(data);
        }
    } catch (error) {
        console.error('[API] Failed to fetch status:', error);
    }
}

async function enableHeating() {
    try {
        const response = await fetch('/api/heating/enable', {
            method: 'POST'
        });
        const data = await response.json();
        
        if (data.success) {
            console.log('[API] Heating enabled');
        } else {
            showError(data.error || 'خطا در فعال‌سازی گرمایش');
        }
    } catch (error) {
        console.error('[API] Failed to enable heating:', error);
        showError('خطا در ارتباط با دستگاه');
    }
}

async function disableHeating() {
    try {
        const response = await fetch('/api/heating/disable', {
            method: 'POST'
        });
        const data = await response.json();
        
        if (data.success) {
            console.log('[API] Heating disabled');
        } else {
            showError(data.error || 'خطا در خاموش کردن گرمایش');
        }
    } catch (error) {
        console.error('[API] Failed to disable heating:', error);
        showError('خطا در ارتباط با دستگاه');
    }
}

async function setTemperature(setpoint) {
    try {
        const response = await fetch('/api/temperature', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ setpoint: setpoint })
        });
        const data = await response.json();
        
        if (!data.success) {
            showError(data.error || 'خطا در تنظیم دما');
        }
    } catch (error) {
        console.error('[API] Failed to set temperature:', error);
        showError('خطا در ارتباط با دستگاه');
    }
}

async function setTimer(durationMinutes) {
    try {
        const response = await fetch('/api/timer', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ duration_min: durationMinutes })
        });
        const data = await response.json();
        
        if (!data.success) {
            showError(data.error || 'خطا در تنظیم تایمر');
        }
    } catch (error) {
        console.error('[API] Failed to set timer:', error);
        showError('خطا در ارتباط با دستگاه');
    }
}

async function connectWiFi(ssid, password) {
    try {
        const response = await fetch('/api/wifi', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ 
                ssid: ssid, 
                password: password 
            })
        });
        const data = await response.json();
        
        if (data.success) {
            showError('در حال اتصال به شبکه...', 'success');
        } else {
            showError(data.error || 'خطا در اتصال به شبکه');
        }
    } catch (error) {
        console.error('[API] Failed to connect WiFi:', error);
        showError('خطا در ارتباط با دستگاه');
    }
}

// ============================
// UI Update Functions
// ============================

function updateUI(data) {
    // Update room temperature and humidity
    if (data.room_temp !== undefined && data.room_temp !== null && !isNaN(data.room_temp)) {
        document.getElementById('room-temp').textContent = data.room_temp.toFixed(1);
    } else {
        document.getElementById('room-temp').textContent = '--';
    }
    
    if (data.room_humidity !== undefined && data.room_humidity !== null && !isNaN(data.room_humidity)) {
        document.getElementById('room-humidity').textContent = data.room_humidity.toFixed(0);
    } else {
        document.getElementById('room-humidity').textContent = '--';
    }
    
    // Update mattress temperature
    if (data.mattress_temp !== undefined && data.mattress_temp !== null && !isNaN(data.mattress_temp)) {
        document.getElementById('mattress-temp').textContent = data.mattress_temp.toFixed(1);
    } else {
        document.getElementById('mattress-temp').textContent = '--';
    }
    
    // Update temperature setpoint
    if (data.setpoint !== undefined) {
        document.getElementById('setpoint').textContent = data.setpoint.toFixed(0);
    }
    
    // Update heating status
    if (data.heating_on !== undefined) {
        updateHeatingStatus(data.heating_on);
    }
    
    if (data.heating_enabled !== undefined) {
        updatePowerButton(data.heating_enabled);
    }
    
    // Update timer (only on initial load or user change)
    if (data.timer_remaining_sec !== undefined && data.timer_duration_min !== undefined) {
        timerRemainingSeconds = data.timer_remaining_sec;
        timerDurationMinutes = data.timer_duration_min;
        updateTimerDisplay();
        
        // Start client-side countdown if timer is running
        if (data.timer_running && !timerInterval) {
            startTimerCountdown();
        } else if (!data.timer_running && timerInterval) {
            stopTimerCountdown();
        }
    }
    
    // Update sensor status
    if (data.mattress_sensor_ok !== undefined) {
        updateSensorStatus(data.mattress_sensor_ok);
    }
}

function updateHeatingStatus(isHeating) {
    const heatingStatus = document.getElementById('heating-status');
    const heatingIcon = document.getElementById('heating-icon');
    
    if (isHeating) {
        heatingStatus.textContent = 'روشن';
        heatingIcon.textContent = '🔥';
        heatingIcon.style.color = '#FF5722';
    } else {
        heatingStatus.textContent = 'خاموش';
        heatingIcon.textContent = '❄️';
        heatingIcon.style.color = '#2196F3';
    }
}

function updatePowerButton(isEnabled) {
    const powerBtn = document.getElementById('power-btn');
    const powerText = document.getElementById('power-text');
    
    if (isEnabled) {
        powerBtn.classList.add('off');
        powerText.textContent = 'خاموش کردن گرمایش';
    } else {
        powerBtn.classList.remove('off');
        powerText.textContent = 'روشن کردن گرمایش';
    }
}

function updateSensorStatus(isHealthy) {
    const sensorStatus = document.getElementById('sensor-status-text');
    
    if (isHealthy) {
        sensorStatus.textContent = 'سنسور سالم';
        sensorStatus.classList.remove('error');
    } else {
        sensorStatus.textContent = 'خطای سنسور';
        sensorStatus.classList.add('error');
        showError('سنسور تشک خراب است. گرمایش غیرفعال شد.');
    }
}

function updateTimerDisplay() {
    const hours = Math.floor(timerRemainingSeconds / 3600);
    const minutes = Math.floor((timerRemainingSeconds % 3600) / 60);
    const seconds = timerRemainingSeconds % 60;
    
    const display = `${hours}:${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`;
    document.getElementById('timer-display').textContent = display;
}

// ============================
// Timer Countdown (Client-Side)
// ============================

function startTimerCountdown() {
    if (timerInterval) {
        clearInterval(timerInterval);
    }
    
    timerInterval = setInterval(function() {
        if (timerRemainingSeconds > 0) {
            timerRemainingSeconds--;
            updateTimerDisplay();
        } else {
            stopTimerCountdown();
        }
    }, 1000);
}

function stopTimerCountdown() {
    if (timerInterval) {
        clearInterval(timerInterval);
        timerInterval = null;
    }
}

// ============================
// Event Listeners
// ============================

function setupEventListeners() {
    // Temperature control buttons
    document.getElementById('temp-up').addEventListener('click', function() {
        const currentSetpoint = parseFloat(document.getElementById('setpoint').textContent);
        const newSetpoint = Math.min(currentSetpoint + 1, 45);
        setTemperature(newSetpoint);
    });
    
    document.getElementById('temp-down').addEventListener('click', function() {
        const currentSetpoint = parseFloat(document.getElementById('setpoint').textContent);
        const newSetpoint = Math.max(currentSetpoint - 1, 20);
        setTemperature(newSetpoint);
    });
    
    // Timer control buttons
    document.getElementById('timer-up').addEventListener('click', function() {
        const newDuration = Math.min(timerDurationMinutes + 15, 480);
        setTimer(newDuration);
    });
    
    document.getElementById('timer-down').addEventListener('click', function() {
        const newDuration = Math.max(timerDurationMinutes - 15, 15);
        setTimer(newDuration);
    });
    
    // Power button
    document.getElementById('power-btn').addEventListener('click', function() {
        const powerBtn = document.getElementById('power-btn');
        
        if (powerBtn.classList.contains('off')) {
            disableHeating();
        } else {
            enableHeating();
        }
    });
    
    // WiFi connect button
    document.getElementById('wifi-connect').addEventListener('click', function() {
        const ssid = document.getElementById('wifi-ssid').value.trim();
        const password = document.getElementById('wifi-password').value;
        
        if (!ssid) {
            showError('لطفا نام شبکه را وارد کنید');
            return;
        }
        
        if (password && (password.length < 8 || password.length > 63)) {
            showError('رمز عبور باید بین 8 تا 63 کاراکتر باشد');
            return;
        }
        
        connectWiFi(ssid, password);
    });
    
    // Error close button
    document.getElementById('error-close').addEventListener('click', function() {
        hideError();
    });
}

// ============================
// Error Display
// ============================

function showError(message, type = 'error') {
    const errorDiv = document.getElementById('error-message');
    const errorText = document.getElementById('error-text');
    
    errorText.textContent = message;
    errorDiv.style.display = 'flex';
    
    if (type === 'success') {
        errorDiv.style.background = '#4CAF50';
    } else {
        errorDiv.style.background = '#F44336';
    }
    
    // Auto-hide after 5 seconds
    setTimeout(hideError, 5000);
}

function hideError() {
    const errorDiv = document.getElementById('error-message');
    errorDiv.style.display = 'none';
}

// ============================
// Utility Functions
// ============================

function formatTime(seconds) {
    const hours = Math.floor(seconds / 3600);
    const minutes = Math.floor((seconds % 3600) / 60);
    const secs = seconds % 60;
    
    return `${hours}:${minutes.toString().padStart(2, '0')}:${secs.toString().padStart(2, '0')}`;
}

// ============================
// Page Visibility API
// ============================

// Pause/resume WebSocket when page is hidden/visible
document.addEventListener('visibilitychange', function() {
    if (document.hidden) {
        console.log('[App] Page hidden, pausing updates');
    } else {
        console.log('[App] Page visible, resuming updates');
        fetchStatus(); // Refresh data when page becomes visible
    }
});

console.log('[App] RODI Web Panel loaded successfully');

