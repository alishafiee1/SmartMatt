# چک‌لیست اصلاح نمایش تایمر در وب‌اپلیکیشن

## هدف
تبدیل نمایش زمان باقیمانده تایمر از فرمت `HH:MM:SS` به نمایش فقط مجموع دقیقه‌ها (مثل OLED)

## وضعیت فعلی
- ✅ **OLED Display**: در حال حاضر فقط دقیقه نمایش می‌دهد (مثلاً "150 min") - **درست است**
- ✅ **دکمه‌های سخت‌افزاری**: با هر فشار 5 دقیقه تغییر می‌کند - **درست است**
- ❌ **وب‌اپلیکیشن**: در حال حاضر `HH:MM:SS` نمایش می‌دهد (مثلاً "02:30:25") - **نیاز به تغییر**
- ❌ **دکمه‌های وب‌اپلیکیشن**: با هر کلیک 15 دقیقه تغییر می‌کند - **نیاز به تغییر به 5 دقیقه**

---

## چک‌لیست تغییرات

### 1. اصلاح تابع `updateTimerDisplay()` در `app.js`
- [ ] **فایل**: `data/app.js`
- [ ] **خط**: 337-344
- [ ] **تغییرات مورد نیاز**:
  - حذف محاسبه `hours` و `seconds`
  - تبدیل `timerRemainingSeconds` به دقیقه (با round up)
  - تغییر فرمت نمایش از `HH:MM:SS` به فقط عدد دقیقه
  - اضافه کردن متن "دقیقه" یا "min" بعد از عدد (اختیاری - برای هماهنگی با OLED)

**کد فعلی:**
```javascript
function updateTimerDisplay() {
    const hours = Math.floor(timerRemainingSeconds / 3600);
    const minutes = Math.floor((timerRemainingSeconds % 3600) / 60);
    const seconds = timerRemainingSeconds % 60;
    
    const display = `${hours}:${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`;
    document.getElementById('timer-display').textContent = display;
}
```

**کد پیشنهادی:**
```javascript
function updateTimerDisplay() {
    // Convert seconds to minutes (round up like OLED display)
    const minutes = Math.ceil(timerRemainingSeconds / 60);
    
    const display = `${minutes} دقیقه`;
    document.getElementById('timer-display').textContent = display;
}
```

---

### 2. اصلاح دکمه‌های افزایش/کاهش تایمر در `app.js`
- [ ] **فایل**: `data/app.js`
- [ ] **خط**: 391-399
- [ ] **تغییرات مورد نیاز**:
  - تغییر مقدار افزایش/کاهش از `15` دقیقه به `5` دقیقه (مطابق با دکمه‌های سخت‌افزاری)
  - بررسی محدوده‌ها (حداقل 10 دقیقه، حداکثر 480 دقیقه)

**کد فعلی:**
```javascript
document.getElementById('timer-up').addEventListener('click', function() {
    const newDuration = Math.min(timerDurationMinutes + 15, 480);
    setTimer(newDuration);
});

document.getElementById('timer-down').addEventListener('click', function() {
    const newDuration = Math.max(timerDurationMinutes - 15, 15);
    setTimer(newDuration);
});
```

**کد پیشنهادی:**
```javascript
document.getElementById('timer-up').addEventListener('click', function() {
    const newDuration = Math.min(timerDurationMinutes + 5, 480);
    setTimer(newDuration);
});

document.getElementById('timer-down').addEventListener('click', function() {
    const newDuration = Math.max(timerDurationMinutes - 5, 10);
    setTimer(newDuration);
});
```

**نکته**: حداقل تایمر در `RodiConfig.h` برابر `10` دقیقه است، نه `15` دقیقه.

---

### 3. بررسی و به‌روزرسانی متن راهنما در `index.html` (اختیاری)
- [ ] **فایل**: `data/index.html`
- [ ] **خط**: 77
- [ ] **بررسی**: آیا نیاز به تغییر متن "محدوده: 15 دقیقه - 8 ساعت" به "محدوده: 10 دقیقه - 8 ساعت" هست؟

**کد فعلی:**
```html
<div class="range-info">محدوده: 15 دقیقه - 8 ساعت</div>
```

**کد پیشنهادی:**
```html
<div class="range-info">محدوده: 10 دقیقه - 8 ساعت</div>
```

---

### 4. بررسی عملکرد شمارش معکوس
- [ ] **بررسی**: تابع `startTimerCountdown()` (خط 350-363)
- [ ] **تأیید**: شمارش معکوس باید هر ثانیه `timerRemainingSeconds` را کاهش دهد
- [ ] **تأیید**: با کاهش هر ثانیه، نمایش دقیقه باید به‌روزرسانی شود
- [ ] **نکته**: با فرمت جدید (فقط دقیقه)، نمایش هر دقیقه یک‌بار تغییر می‌کند (نه هر ثانیه)

**کد فعلی (بدون تغییر نیاز دارد):**
```javascript
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
```

**نکته**: این کد درست است و نیازی به تغییر ندارد. فقط تابع `updateTimerDisplay()` تغییر می‌کند.

---

### 5. بررسی هماهنگی با OLED Display
- [ ] **تأیید**: فرمت نمایش در وب‌اپلیکیشن با OLED هماهنگ است
- [ ] **بررسی**: `DisplayManager.cpp` خط 352-358
- [ ] **نکته**: OLED از `formatTimer()` استفاده می‌کند که ثانیه را به دقیقه تبدیل می‌کند با round up

**کد OLED (بدون تغییر نیاز دارد):**
```cpp
String DisplayManager::formatTimer(uint32_t seconds) {
    uint32_t minutes = (seconds + 10) / 60;  // Round up
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%u", minutes);
    return String(buffer);
}
```

**نکته**: در JavaScript باید از `Math.ceil()` استفاده کنیم (مثل OLED که round up می‌کند).

---

## خلاصه تغییرات

### فایل‌های نیازمند تغییر:
1. ✅ `data/app.js` - تابع `updateTimerDisplay()` (خط 337-344)
2. ✅ `data/app.js` - Event listeners برای دکمه‌های تایمر (خط 391-399)
3. ⚠️ `data/index.html` - متن راهنما (خط 77) - **اختیاری**

### فایل‌های بدون تغییر:
- ✅ `src/display/DisplayManager.cpp` - OLED display (درست است)
- ✅ `src/server/WebServer.cpp` - ارسال `timer_remaining_sec` (درست است)
- ✅ `src/timer/HeatingTimer.cpp` - محاسبه زمان باقیمانده (درست است)
- ✅ `include/config/RodiConfig.h` - تنظیمات (درست است)

---

## تست‌های مورد نیاز

### تست 1: نمایش زمان باقیمانده
- [ ] تست با زمان 150 دقیقه (9000 ثانیه) → باید "150 دقیقه" نمایش دهد
- [ ] تست با زمان 1 دقیقه و 30 ثانیه (90 ثانیه) → باید "2 دقیقه" نمایش دهد (round up)
- [ ] تست با زمان 0 ثانیه → باید "0 دقیقه" یا "انقضا" نمایش دهد

### تست 2: شمارش معکوس
- [ ] تست شمارش معکوس از 5 دقیقه → باید هر دقیقه یک‌بار به‌روزرسانی شود
- [ ] تست انقضای تایمر → باید شمارش معکوس متوقف شود

### تست 3: دکمه‌های افزایش/کاهش
- [ ] تست دکمه افزایش → باید 5 دقیقه اضافه کند
- [ ] تست دکمه کاهش → باید 5 دقیقه کم کند
- [ ] تست محدودیت حداکثر (480 دقیقه) → نباید بیشتر از 480 برود
- [ ] تست محدودیت حداقل (10 دقیقه) → نباید کمتر از 10 برود

### تست 4: هماهنگی با OLED
- [ ] تست همزمان نمایش در وب و OLED → باید مقدار یکسان نمایش دهند
- [ ] تست round up → باید هر دو به یک صورت round up کنند

---

## مثال‌های تبدیل

| ثانیه | فرمت قدیم (HH:MM:SS) | فرمت جدید (دقیقه) |
|-------|---------------------|------------------|
| 9000  | 02:30:00            | 150 دقیقه        |
| 150   | 00:02:30            | 3 دقیقه          |
| 90    | 00:01:30            | 2 دقیقه          |
| 59    | 00:00:59            | 1 دقیقه          |
| 0     | 00:00:00            | 0 دقیقه          |

---

## تاریخ و وضعیت
- **تاریخ ایجاد**: 1404/09/27
- **وضعیت**: آماده برای اعمال تغییرات
- **اولویت**: متوسط
