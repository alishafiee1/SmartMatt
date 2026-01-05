<style>
body, p, h1, h2, h3, h4, h5, h6, li, ul, ol {
    font-family: 'Segoe UI', Segoe, Tahoma, Geneva, Verdana, sans-serif !important;
    direction: rtl;
    text-align: right;
}

pre, code {
    direction: ltr;
    text-align: left;
}
</style>

# 📱 RODI Smart Mattress - UI Design Proposal
## طراحی رابط کاربری صفحه نمایش OLED

---

## 📐 ساختار کلی صفحه (128×64 پیکسل)

### نمای کلی Layout

```
║              ║                               ║                 ║
║              ║    [WiFi] [Heat] [Sys]        ║                 ║
║              ║                               ║                 ║
║              ║                               ║                 ║
║   SET TEMP   ║      MAIN DISPLAY             ║     TIMER       ║
║   (Left)     ║       (Center)                ║    (Right)      ║
║              ║                               ║                 ║
║   ┌─────┐    ║    ┌───────────────┐          ║    ┌─────┐      ║
║   │27.5°│    ║    │               │          ║    │ 480 │      ║
║   │ SET │    ║    │    25.5°      │          ║    │ min │      ║
║   └─────┘    ║    │               │          ║    └─────┘      ║
║              ║    └───────────────┘          ║                 ║
║              ║                               ║                 ║
║              ║      Room: 21° 23%            ║                 ║
║              ║                               ║                 ║
```

**نکات مهم:**
- خطوط عمودی از بالا تا پایین صفحه (Y: 0 تا 64) کشیده می‌شوند
- خطوط افقی حذف شده‌اند (بدون جداکننده بین Status Bar و Content)
- Status Bar و Bottom Bar فقط در قسمت وسط (Main Display) قرار دارند
- نمادها و اطلاعات اتاق در بالا و پایین قسمت وسط ست می‌شوند

---

## 🎯 تقسیم‌بندی دقیق مختصات

### 1. Status Bar (نوار بالا - نمادها در قسمت وسط)
```
X: 32-96 پیکسل (فقط در قسمت Main Display)
Y: 0-10 پیکسل
─────────────────────────────────────────
نمادها (هر کدام 8×8 پیکسل - کوچک):
  • WiFi Icon      → X: 40,  Y: 1
  • Heating Icon   → X: 52,  Y: 1  
  • System Icon    → X: 64,  Y: 1
  
  (نمادها در مرکز قسمت Main Display ست می‌شوند)
```

**توضیح نمادها:**
- **WiFi Icon**: 
  - روشن: متصل به شبکه (SoftAP یا Station)
  - خاموش: WiFi غیرفعال
  
- **Heating Icon** (آیکون المنت گرمایشی):
  - روشن: المنت در حال گرمایش
  - خاموش: المنت خاموش (ولی ممکن است سیستم روشن باشد)
  
- **System Icon** (آیکون وضعیت سیستم):
  - روشن: سیستم فعال و در حال مانیتورینگ
  - خاموش: سیستم کاملاً خاموش

**نکته مهم:** سیستم می‌تواند روشن باشد (در حال مانیتورینگ دما) ولی المنت خاموش باشد (چون به دمای مطلوب رسیده).

---

### 2. خط جداکننده عمودی اول (Left Divider)
```
X: 32 پیکسل
Y: 0 تا 64 (از بالا تا پایین صفحه)
─────────────────
یک خط عمودی نازک برای جدا کردن قسمت Set Temp از Main Display
```

### 3. خط جداکننده عمودی دوم (Right Divider)
```
X: 96 پیکسل
Y: 0 تا 64 (از بالا تا پایین صفحه)
─────────────────
یک خط عمودی نازک برای جدا کردن قسمت Main Display از Timer
```

---

### 4. قسمت چپ - Set Temperature (دمای تنظیم شده)
```
X: 0-32 پیکسل
Y: 0-64 پیکسل (کل ارتفاع صفحه)
─────────────────────────
Center Point: X=16, Y=32

محتوا (مرکز عمودی):
  ┌─────────┐
  │  27.5°  │  ← Font Size 1 (normal)
  │   SET   │  ← Label کوچک
  └─────────┘

حالت Active (هنگام تغییر - 500ms):
  ┌─────┐
  │28.5°│  ← Font Size 2 (Bold)
  │ SET │
  └─────┘
```

---

### 5. قسمت وسط - Main Display (دمای فعلی)
```
X: 32-96 پیکسل (64 پیکسل عرض)
Y: 0-64 پیکسل (کل ارتفاع صفحه)
─────────────────────────────────
Center Point: X=64, Y=32

محتوا:
  ┌─────────────────┐
  │  [WiFi][H][Sys] │  ← نمادها در بالا (کوچک)
  │                 │
  │     25.5°       │  ← Font Size 3 (بزرگ) - مرکز
  │                 │
  │   Room: 21° 23% │  ← اطلاعات اتاق در پایین (کوچک)
  └─────────────────┘

این بزرگترین عدد روی صفحه است
```

---

### 6. قسمت راست - Timer (تایمر باقیمانده)
```
X: 96-128 پیکسل
Y: 0-64 پیکسل (کل ارتفاع صفحه)
─────────────────────────
Center Point: X=112, Y=32

محتوا (مرکز عمودی):
  ┌─────────┐
  │   480   │  ← Font Size 1 (normal)
  │   min   │  ← Label کوچک
  └─────────┘

حالت Active (هنگام تغییر - 500ms):
  ┌─────┐
  │ 495 │  ← Font Size 2 (Bold)
  │ min │
  └─────┘
```

---

### 7. Bottom Bar (نوار پایین - اطلاعات اتاق در قسمت وسط)
```
X: 32-96 پیکسل (فقط در قسمت Main Display)
Y: 54-64 پیکسل
─────────────────────────
محتوا (متن کوچک - Font Size 1):
  Room: 21° 23%
  
  21°  → دمای اتاق (از سنسور DHT22)
  23%  → رطوبت اتاق
  
  (در پایین قسمت Main Display و در مرکز افقی ست می‌شود)
```

---

## 🎨 انیمیشن Bold برای تغییرات

### انیمیشن ساده Bold

**مفهوم:**
هر عددی که در صفحه نمایش تغییر کند، به مدت **500ms (نیم ثانیه)** با فونت **Bold** نمایش داده می‌شود.

**اعداد قابل تغییر:**
- **Set Temperature** (27.5°)
- **Current Temperature** (25.5°)
- **Timer** (480 min)

**نحوه کار:**
```
حالت عادی:          حالت Bold (500ms):        بازگشت به عادی:
┌─────┐            ┌─────┐                   ┌─────┐
│27.5°│    →       │27.5°│    (500ms) →      │27.5°│
│ SET │            │ SET │                   │ SET │
└─────┘            └─────┘                   └─────┘
Font: Normal       Font: Bold                Font: Normal
```

**پارامترهای انیمیشن:**
- مدت زمان Bold: 500ms (نیم ثانیه)
- روش نمایش: فونت Bold (یا Font Size بزرگتر)
- خودکار: پس از 500ms به حالت عادی برمی‌گردد

**زمان فعال‌سازی:**
- هنگام تغییر Set Temperature (از دکمه یا Web Panel)
- هنگام تغییر Timer (از دکمه یا Web Panel)
- هنگام تغییر Current Temperature (از سنسور)

---

## 🏗️ معماری کلاس‌ها و متدها

### کلاس DisplayManager

#### Enums و Structures

```cpp
// انیمیشن Bold برای هر عدد
struct BoldAnimation {
    bool isActive;          // آیا انیمیشن فعال است
    uint32_t startTime;     // زمان شروع انیمیشن
    uint32_t duration;      // مدت زمان انیمیشن (500ms)
};

// انیمیشن‌های جداگانه برای هر عدد
struct AnimationContext {
    BoldAnimation setTemp;      // انیمیشن Set Temperature
    BoldAnimation currentTemp;   // انیمیشن Current Temperature
    BoldAnimation timer;         // انیمیشن Timer
};
```

#### متدهای عمومی (Public Methods)

```cpp
class DisplayManager {
public:
    // Trigger animations --- فعال‌سازی انیمیشن Bold برای هر عدد
    void triggerSetTempBold();          // فعال‌سازی Bold برای Set Temperature
    void triggerCurrentTempBold();      // فعال‌سازی Bold برای Current Temperature
    void triggerTimerBold();            // فعال‌سازی Bold برای Timer
    
    // Update method --- باید در loop اصلی فراخوانی شود
    void update(
        float roomTemp,
        float roomHumidity, 
        float mattressTemp,
        float setpoint,
        uint32_t remainingSeconds,
        bool isHeating,
        bool wifiConnected
    );
};
```

#### متدهای خصوصی (Private Methods)

```cpp
private:
    // Animation management --- مدیریت انیمیشن‌های Bold
    void updateAnimations();                   // به‌روزرسانی وضعیت تمام انیمیشن‌ها
    void updateBoldAnimation(BoldAnimation& anim);  // به‌روزرسانی یک انیمیشن Bold
    bool isBoldActive(const BoldAnimation& anim) const;  // بررسی فعال بودن انیمیشن
    
    // Drawing methods --- متدهای رسم اجزای مختلف
    void drawStatusBar(bool wifiConnected, bool isHeating, bool systemOn);  // فقط در قسمت وسط
    void drawVerticalDividers();                // رسم خطوط جداکننده عمودی (0 تا 64)
    void drawSetTempArea(float setpoint);       // رسم قسمت Set Temperature
    void drawMainDisplay(float currentTemp);    // رسم قسمت Main Display
    void drawTimerArea(uint32_t remainingSeconds); // رسم قسمت Timer
    void drawBottomBar(float roomTemp, float roomHumidity);  // فقط در قسمت وسط
    
    
    // Drawing with bold --- رسم با فونت Bold
    void drawSetTempWithBold(float setpoint, bool isBold);
    void drawCurrentTempWithBold(float currentTemp, bool isBold);
    void drawTimerWithBold(uint32_t remainingSeconds, bool isBold);
    
    // Icon drawing --- رسم نمادها
    void drawWiFiIcon(int16_t x, int16_t y, bool connected);
    void drawHeatingIcon(int16_t x, int16_t y, bool active);
    void drawSystemIcon(int16_t x, int16_t y, bool systemOn);
    
    // Helper methods --- متدهای کمکی
    int16_t getTextWidth(const char* text, uint8_t textSize);
    int16_t getTextHeight(uint8_t textSize);
```

#### متغیرهای خصوصی (Private Members)

```cpp
private:
    // Animation context --- وضعیت انیمیشن‌های Bold
    AnimationContext m_animations;
    
    // Display state --- وضعیت نمایش
    bool m_systemOn;        // آیا سیستم روشن است (مانیتورینگ فعال)
    bool m_isHeating;       // آیا المنت در حال گرمایش است
    bool m_wifiConnected;   // آیا WiFi متصل است
    
    // Refresh rate management --- مدیریت نرخ رفرش
    uint32_t m_refreshInterval;   // فاصله رفرش (200ms = 5 FPS)
    uint32_t m_lastRefreshTime;    // زمان آخرین رفرش
    
    // Cached values --- مقادیر ذخیره شده برای جلوگیری از رسم مکرر
    float m_lastSetpoint;
    uint32_t m_lastRemainingSeconds;
```

---

## 🔗 یکپارچگی با ButtonManager

### نحوه فعال‌سازی انیمیشن‌ها

```cpp
// در ButtonManager::handleTempUp()
void ButtonManager::handleTempUp() {
    float current = getTemperatureSetpoint();
    float newTemp = current + BTN_TEMP_INCREMENT;
    
    if (newTemp <= TEMP_MAX_C) {
        setTemperatureSetpoint(newTemp);
        
        // ✨ فعال‌سازی انیمیشن Bold
        if (m_onSetTempChanged) {
            m_onSetTempChanged();  // Callback به DisplayManager
        }
        
        updateActivityTime();
    }
}

// در ButtonManager::handleTimerUp()
void ButtonManager::handleTimerUp() {
    uint32_t current = getTimerDuration();
    uint32_t newDuration = current + BTN_TIMER_INCREMENT;
    
    if (newDuration <= TIMER_MAX_MIN) {
        setTimerDuration(newDuration);
        
        // ✨ فعال‌سازی انیمیشن Bold
        if (m_onTimerChanged) {
            m_onTimerChanged();  // Callback به DisplayManager
        }
        
        updateActivityTime();
    }
}
```

### اضافه کردن Callbacks به ButtonManager

```cpp
// در ButtonManager.h
class ButtonManager {
public:
    // Callback registration --- ثبت callback ها
    void onSetTempChanged(void (*callback)(void));
    void onTimerChanged(void (*callback)(void));
    
private:
    // Callback pointers --- اشاره‌گرها به callback ها
    void (*m_onSetTempChanged)(void);
    void (*m_onTimerChanged)(void);
};
```

### استفاده در main.cpp

```cpp
void setup() {
    // ... initialization
    
    // ثبت callback ها برای انیمیشن‌های Bold
    buttonManager->onSetTempChanged([]() {
        displayManager.triggerSetTempBold();
    });
    
    buttonManager->onTimerChanged([]() {
        displayManager.triggerTimerBold();
    });
}
```

---

## 📊 مثال‌های بصری حالت‌های مختلف

### حالت 1: سیستم روشن، المنت در حال گرمایش
```
║              ║                               ║                 ║
║              ║  [WiFi:ON] [Heat:ON] [Sys:ON] ║                 ║
║              ║                               ║                 ║
║              ║                               ║                 ║
║   ┌─────┐    ║    ┌───────────────┐          ║    ┌─────┐      ║
║   │27.5°│    ║    │               │          ║    │ 480 │      ║
║   │ SET │    ║    │    24.2°      │          ║    │ min │      ║
║   └─────┘    ║    │               │          ║    └─────┘      ║
║              ║    └───────────────┘          ║                 ║
║              ║                               ║                 ║
║              ║      Room: 21° 23%            ║                 ║
║              ║                               ║                 ║
```

### حالت 2: سیستم روشن، المنت خاموش (دما رسیده)
```
║              ║                               ║                 ║
║              ║ [WiFi:ON] [Heat:OFF] [Sys:ON] ║                 ║
║              ║                               ║                 ║
║              ║                               ║                 ║
║   ┌─────┐    ║    ┌───────────────┐          ║    ┌─────┐      ║
║   │27.5°│    ║    │               │          ║    │ 475 │      ║
║   │ SET │    ║    │    27.8°      │          ║    │ min │      ║
║   └─────┘    ║    │               │          ║    └─────┘      ║
║              ║    └───────────────┘          ║                 ║
║              ║                               ║                 ║
║              ║      Room: 21° 23%            ║                 ║
║              ║                               ║                 ║
```
**توضیح:** سیستم روشن است و دما را مانیتور می‌کند، ولی المنت خاموش است چون دما به Set Point رسیده.

### حالت 3: سیستم خاموش
```
║              ║                               ║                 ║
║              ║ [WiFi:ON] [Heat:OFF] [Sys:OFF]║                 ║
║              ║                               ║                 ║
║              ║                               ║                 ║
║   ┌─────┐    ║    ┌───────────────┐          ║    ┌─────┐      ║
║   │27.5°│    ║    │               │          ║    │  0  │      ║
║   │ SET │    ║    │    23.5°      │          ║    │ min │      ║
║   └─────┘    ║    │               │          ║    └─────┘      ║
║              ║    └───────────────┘          ║                 ║
║              ║                               ║                 ║
║              ║      Room: 21° 23%            ║                 ║
║              ║                               ║                 ║
```

### حالت 4: در حال تغییر Set Temperature (انیمیشن فعال)
```
║              ║                               ║                 ║
║              ║  [WiFi:ON] [Heat:ON] [Sys:ON] ║                 ║
║              ║                               ║                 ║
║              ║                               ║                 ║
║   ┌─────┐    ║    ┌───────────────┐          ║    ┌─────┐      ║
║   │28.5°│    ║    │               │          ║    │ 480 │      ║
║   │ SET │    ║    │    25.5°      │          ║    │ min │      ║
║   └─────┘    ║    │               │          ║    └─────┘      ║
║              ║    └───────────────┘          ║                 ║
║              ║                               ║                 ║
║              ║      Room: 21° 23%            ║                 ║
║              ║                               ║                 ║
```
**توضیح:** Set Temperature با فونت Bold نمایش داده می‌شود (500ms).

---

## 🎯 نکات پیاده‌سازی

### 1. رسم خطوط جداکننده عمودی (از بالا تا پایین)
```cpp
void DisplayManager::drawVerticalDividers() {
    // خط جداکننده چپ (بین Set Temp و Main Display)
    // از بالا تا پایین صفحه (Y: 0 تا 64)
    display.drawLine(32, 0, 32, 64, SH110X_WHITE);
    
    // خط جداکننده راست (بین Main Display و Timer)
    // از بالا تا پایین صفحه (Y: 0 تا 64)
    display.drawLine(96, 0, 96, 64, SH110X_WHITE);
}
```

### 2. مدیریت انیمیشن‌های Bold
```cpp
void DisplayManager::updateAnimations() {
    // به‌روزرسانی تمام انیمیشن‌های Bold
    updateBoldAnimation(m_animations.setTemp);
    updateBoldAnimation(m_animations.currentTemp);
    updateBoldAnimation(m_animations.timer);
}

void DisplayManager::updateBoldAnimation(BoldAnimation& anim) {
    if (!anim.isActive) return;
    
    uint32_t elapsed = millis() - anim.startTime;
    
    // اگر زمان انیمیشن تمام شد، غیرفعال کن
    if (elapsed >= anim.duration) {
        anim.isActive = false;
    }
}

bool DisplayManager::isBoldActive(const BoldAnimation& anim) const {
    if (!anim.isActive) return false;
    
    uint32_t elapsed = millis() - anim.startTime;
    return elapsed < anim.duration;
}

// فعال‌سازی انیمیشن Bold
void DisplayManager::triggerSetTempBold() {
    m_animations.setTemp.isActive = true;
    m_animations.setTemp.startTime = millis();
    m_animations.setTemp.duration = 500;  // 500ms
}

void DisplayManager::triggerCurrentTempBold() {
    m_animations.currentTemp.isActive = true;
    m_animations.currentTemp.startTime = millis();
    m_animations.currentTemp.duration = 500;  // 500ms
}

void DisplayManager::triggerTimerBold() {
    m_animations.timer.isActive = true;
    m_animations.timer.startTime = millis();
    m_animations.timer.duration = 500;  // 500ms
}
```

### 3. رسم با فونت Bold
```cpp
void DisplayManager::drawSetTempWithBold(float setpoint, bool isBold) {
    // اگر Bold فعال است، از Font Size 2 استفاده کن
    // در غیر این صورت از Font Size 1
    uint8_t fontSize = isBold ? 2 : 1;
    
    display.setTextSize(fontSize);
    display.setTextColor(SH110X_WHITE);
    
    // محاسبه موقعیت برای مرکز
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%.1f", setpoint);
    int16_t textWidth = getTextWidth(buffer, fontSize);
    int16_t x = 16 - (textWidth / 2);  // مرکز افقی Set Temp Area
    
    display.setCursor(x, 28);
    display.print(buffer);
    
    // رسم "SET" label
    display.setTextSize(1);
    display.setCursor(16 - 6, 38);
    display.print("SET");
}

void DisplayManager::drawCurrentTempWithBold(float currentTemp, bool isBold) {
    // اگر Bold فعال است، از Font Size 4 استفاده کن
    // در غیر این صورت از Font Size 3
    uint8_t fontSize = isBold ? 4 : 3;
    
    display.setTextSize(fontSize);
    display.setTextColor(SH110X_WHITE);
    
    // محاسبه موقعیت برای مرکز Main Display
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%.1f", currentTemp);
    int16_t textWidth = getTextWidth(buffer, fontSize);
    int16_t x = 64 - (textWidth / 2);  // مرکز افقی Main Display
    
    display.setCursor(x, 20);
    display.print(buffer);
}

void DisplayManager::drawTimerWithBold(uint32_t remainingSeconds, bool isBold) {
    // اگر Bold فعال است، از Font Size 2 استفاده کن
    // در غیر این صورت از Font Size 1
    uint8_t fontSize = isBold ? 2 : 1;
    
    display.setTextSize(fontSize);
    display.setTextColor(SH110X_WHITE);
    
    // محاسبه موقعیت برای مرکز
    uint32_t minutes = remainingSeconds / 60;
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%u", minutes);
    int16_t textWidth = getTextWidth(buffer, fontSize);
    int16_t x = 112 - (textWidth / 2);  // مرکز افقی Timer Area
    
    display.setCursor(x, 28);
    display.print(buffer);
    
    // رسم "min" label
    display.setTextSize(1);
    display.setCursor(112 - 6, 38);
    display.print("min");
}
```

### 4. استفاده در متد update()
```cpp
void DisplayManager::update(...) {
    uint32_t now = millis();
    
    // به‌روزرسانی انیمیشن‌ها (همیشه)
    updateAnimations();
    
    // فقط اگر زمان رفرش رسیده باشد، صفحه را به‌روزرسانی کن
    if (now - m_lastRefreshTime >= m_refreshInterval) {
        m_lastRefreshTime = now;
        
        // بررسی وضعیت Bold برای هر عدد
        bool setTempBold = isBoldActive(m_animations.setTemp);
        bool currentTempBold = isBoldActive(m_animations.currentTemp);
        bool timerBold = isBoldActive(m_animations.timer);
        
        // رسم تمام اجزا
        display.clearDisplay();
        drawVerticalDividers();
        drawStatusBar(...);
        drawSetTempWithBold(setpoint, setTempBold);
        drawCurrentTempWithBold(mattressTemp, currentTempBold);
        drawTimerWithBold(remainingSeconds, timerBold);
        drawBottomBar(...);
        display.display();
    }
}
```

### 5. رسم Status Bar (نمادها در قسمت وسط - بالا)
```cpp
void DisplayManager::drawStatusBar(bool wifiConnected, bool isHeating, bool systemOn) {
    // نمادها فقط در قسمت Main Display (X: 32-96)
    // و در بالا (Y: 0-10) رسم می‌شوند
    // نمادها کوچک هستند (8×8 پیکسل)
    
    // WiFi Icon - در مرکز افقی قسمت Main Display
    drawWiFiIcon(40, 1, wifiConnected);
    
    // Heating Icon
    drawHeatingIcon(52, 1, isHeating);
    
    // System Icon
    drawSystemIcon(64, 1, systemOn);
}
```

### 6. رسم Bottom Bar (اطلاعات اتاق در قسمت وسط - پایین)
```cpp
void DisplayManager::drawBottomBar(float roomTemp, float roomHumidity) {
    // فقط در قسمت Main Display (X: 32-96)
    // و در پایین (Y: 54-64) رسم می‌شود
    // متن کوچک (Font Size 1) و در مرکز افقی ست می‌شود
    
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    
    // محاسبه مرکز افقی برای ست کردن متن
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "Room: %.0f° %.0f%%", roomTemp, roomHumidity);
    int16_t textWidth = getTextWidth(buffer, 1);
    int16_t x = 64 - (textWidth / 2);  // مرکز افقی Main Display (X=64)
    
    display.setCursor(x, 55);
    display.print(buffer);
}
```

---

## 📏 جدول مختصات کامل

| بخش | X Start | X End | Y Start | Y End | Width | Height | Center X | Center Y |
|-----|---------|-------|---------|-------|-------|--------|----------|----------|
| Status Bar (وسط) | 32 | 96 | 0 | 10 | 64 | 10 | 64 | 5 |
| WiFi Icon | 40 | 48 | 1 | 9 | 8 | 8 | 44 | 5 |
| Heating Icon | 52 | 60 | 1 | 9 | 8 | 8 | 56 | 5 |
| System Icon | 64 | 72 | 1 | 9 | 8 | 8 | 68 | 5 |
| Left Divider | 32 | 32 | 0 | 64 | 1 | 64 | - | - |
| Set Temp Area | 0 | 32 | 0 | 64 | 32 | 64 | 16 | 32 |
| Main Display | 32 | 96 | 0 | 64 | 64 | 64 | 64 | 32 |
| Right Divider | 96 | 96 | 0 | 64 | 1 | 64 | - | - |
| Timer Area | 96 | 128 | 0 | 64 | 32 | 64 | 112 | 32 |
| Bottom Bar (وسط) | 32 | 96 | 54 | 64 | 64 | 10 | 64 | 59 |

---

## ✅ چک‌لیست پیاده‌سازی

- [ ] ایجاد struct `BoldAnimation`
- [ ] ایجاد struct `AnimationContext` با 3 انیمیشن Bold
- [ ] پیاده‌سازی متد `triggerSetTempBold()`
- [ ] پیاده‌سازی متد `triggerCurrentTempBold()`
- [ ] پیاده‌سازی متد `triggerTimerBold()`
- [ ] پیاده‌سازی متد `updateAnimations()`
- [ ] پیاده‌سازی متد `updateBoldAnimation()`
- [ ] پیاده‌سازی متد `isBoldActive()`
- [ ] پیاده‌سازی متد `drawVerticalDividers()`
- [ ] پیاده‌سازی متد `drawStatusBar()` با 3 نماد
- [ ] پیاده‌سازی متد `drawSystemIcon()` (نماد جدید)
- [ ] پیاده‌سازی متد `drawSetTempWithBold()`
- [ ] پیاده‌سازی متد `drawCurrentTempWithBold()`
- [ ] پیاده‌سازی متد `drawTimerWithBold()`
- [ ] اضافه کردن callback ها به `ButtonManager`
- [ ] ثبت callback ها در `main.cpp`
- [ ] تست انیمیشن Set Temperature
- [ ] تست انیمیشن Timer
- [ ] تست نمایش نمادها در حالت‌های مختلف

---

## 🎨 پالت رنگی (برای صفحه Monochrome)

- **SH110X_WHITE**: برای متن، border، و نمادهای فعال
- **SH110X_BLACK**: برای پس‌زمینه و نمادهای غیرفعال
- **Inverted Colors**: برای highlight کردن (استفاده در انیمیشن pulse)

---

## 📝 یادداشت‌های نهایی

1. **Performance**: انیمیشن Bold ساده و سریع است (500ms)
2. **Refresh Rate**: 5 FPS (200ms interval) - برای صرفه‌جویی در مصرف انرژی
3. **Timing**: مدت زمان انیمیشن Bold: 500ms (نیم ثانیه)
4. **User Feedback**: کاربر باید بلافاصله متوجه تغییر شود
5. **System Status**: تفاوت بین "سیستم روشن" و "المنت فعال" باید واضح باشد
6. **Dividers**: خطوط جداکننده عمودی از بالا تا پایین صفحه (Y: 0 تا 64) کشیده می‌شوند
7. **Icons**: نمادها کوچک هستند (8×8 پیکسل) و فقط در قسمت Main Display نمایش داده می‌شوند
8. **Layout**: Status Bar و Bottom Bar فقط در قسمت وسط (Main Display) قرار دارند
9. **No Horizontal Lines**: خطوط افقی حذف شده‌اند - فقط خطوط عمودی وجود دارند

---

**تاریخ ایجاد:** 2026-01-02  
**نسخه:** 1.0  
**وضعیت:** پروپوزال نهایی - آماده پیاده‌سازی

