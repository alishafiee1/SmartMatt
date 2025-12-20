# Web Panel Icons Guide

این فایل لیست ایکون‌های موردنیاز برای پنل وب را مشخص می‌کند.

## ایکون‌های موردنیاز

### 1. Heating Status Icon (`heating-on.svg`, `heating-off.svg`)
- **استفاده**: نمایش وضعیت گرمایش (روشن/خاموش)
- **محل استفاده**: Status Bar (خط 316 در design.md)
- **ویژگی‌ها**: 
  - `heating-on.svg`: رنگ نارنجی (#FF5722) - وقتی گرمایش روشن است
  - `heating-off.svg`: رنگ خاکستری - وقتی گرمایش خاموش است
  - اندازه: 24x24px

### 2. WiFi Icon (`wifi-connected.svg`, `wifi-disconnected.svg`, `wifi-ap.svg`)
- **استفاده**: نمایش وضعیت اتصال WiFi
- **محل استفاده**: Status Bar (خط 316 در design.md)
- **ویژگی‌ها**:
  - `wifi-connected.svg`: سبز (#4CAF50) - متصل به شبکه خانگی
  - `wifi-disconnected.svg`: قرمز (#F44336) - قطع اتصال
  - `wifi-ap.svg`: آبی (#2196F3) - فقط SoftAP فعال
  - اندازه: 24x24px

### 3. Plus Button Icon (`plus.svg`)
- **استفاده**: دکمه افزایش دما و تایمر
- **محل استفاده**: 
  - Temperature Control (خط 325)
  - Timer Control (خط 328)
- **ویژگی‌ها**: 
  - ساده و واضح
  - اندازه: 32x32px (برای لمس راحت در موبایل)

### 4. Minus Button Icon (`minus.svg`)
- **استفاده**: دکمه کاهش دما و تایمر
- **محل استفاده**: 
  - Temperature Control (خط 325)
  - Timer Control (خط 328)
- **ویژگی‌ها**: 
  - ساده و واضح
  - اندازه: 32x32px

### 5. Power Icon (`power-on.svg`, `power-off.svg`)
- **استفاده**: دکمه اصلی روشن/خاموش
- **محل استفاده**: Main Control (خط 330)
- **ویژگی‌ها**:
  - `power-on.svg`: سبز (#4CAF50) - وقتی سیستم روشن است
  - `power-off.svg`: خاکستری - وقتی سیستم خاموش است
  - اندازه: 48x48px (بزرگتر از بقیه)

### 6. Loading Spinner (`loading.svg`)
- **استفاده**: نمایش بارگذاری (WiFi provisioning، API calls)
- **محل استفاده**: هنگام درخواست‌های API
- **ویژگی‌ها**: 
  - انیمیشن چرخشی (با CSS)
  - اندازه: 24x24px

### 7. WebSocket Connection Status (`ws-connected.svg`, `ws-disconnected.svg`)
- **استفاده**: نمایش وضعیت اتصال WebSocket
- **محل استفاده**: Status Bar یا گوشه صفحه
- **ویژگی‌ها**:
  - `ws-connected.svg`: سبز - اتصال برقرار
  - `ws-disconnected.svg`: قرمز - قطع اتصال
  - اندازه: 16x16px (کوچک)

### 8. Success Icon (`success.svg`)
- **استفاده**: نمایش موفقیت عملیات
- **محل استفاده**: پیام‌های موفقیت
- **ویژگی‌ها**: 
  - سبز (#4CAF50)
  - اندازه: 20x20px

### 9. Error Icon (`error.svg`)
- **استفاده**: نمایش خطا
- **محل استفاده**: پیام‌های خطا
- **ویژگی‌ها**: 
  - قرمز (#F44336)
  - اندازه: 20x20px

## ساختار فایل‌ها

```
data/icons/
├── heating-on.svg          (297 bytes)
├── heating-off.svg         (297 bytes)
├── wifi-connected.svg      (401 bytes)
├── wifi-disconnected.svg   (569 bytes)
├── wifi-ap.svg             (389 bytes)
├── plus.svg                (304 bytes)
├── minus.svg               (261 bytes)
├── power-on.svg            (308 bytes)
├── power-off.svg           (308 bytes)
├── loading.svg             (614 bytes)
├── ws-connected.svg        (258 bytes)
├── ws-disconnected.svg     (258 bytes)
├── success.svg             (328 bytes)
├── error.svg               (356 bytes)
└── clock.svg               (304 bytes)
```

**حجم کل: 5.13 KB** ✅

## محدودیت‌ها

- **حجم کل**: حداکثر 5KB برای همه ایکون‌ها (طبق design.md خط 215) - ✅ **تطابق دارد**
- **فرمت**: SVG (برای کیفیت بالا و حجم کم) - ✅ **Feather Icons**
- **رنگ‌ها**: استفاده از رنگ‌های تعریف شده در Color Scheme (خط 345-351)
- **اندازه**: بهینه برای نمایش در موبایل و دسکتاپ

## وضعیت پیاده‌سازی

✅ **تکمیل شده** - تمام ایکون‌های موردنیاز از پک Feather Icons انتخاب و آماده استفاده هستند.

### ویژگی‌های ایکون‌های انتخاب شده:

- **منبع**: Feather Icons (https://feathericons.com/)
- **سبک**: Minimalist، stroke-based، modern
- **سازگاری**: تمام ایکون‌ها با viewBox="0 0 24 24" و stroke-width="2"
- **رنگ**: استفاده از `stroke="currentColor"` برای تغییر رنگ آسان با CSS
- **حجم**: بهینه شده و کمتر از محدودیت 5KB

### نقشه استفاده:

| ایکون | منبع اصلی | استفاده |
|-------|-----------|----------|
| heating-on.svg | thermometer.svg | گرمایش فعال |
| heating-off.svg | thermometer.svg | گرمایش غیرفعال |
| wifi-connected.svg | wifi.svg | WiFi متصل |
| wifi-disconnected.svg | wifi-off.svg | WiFi قطع |
| wifi-ap.svg | radio.svg | SoftAP فعال |
| plus.svg | plus.svg | افزایش مقدار |
| minus.svg | minus.svg | کاهش مقدار |
| power-on.svg | power.svg | روشن |
| power-off.svg | power.svg | خاموش |
| loading.svg | loader.svg | بارگذاری |
| ws-connected.svg | circle.svg | WebSocket متصل |
| ws-disconnected.svg | circle.svg | WebSocket قطع |
| success.svg | check-circle.svg | موفقیت |
| error.svg | alert-circle.svg | خطا |
| clock.svg | clock.svg | تایمر |

## نکات پیاده‌سازی

1. **تغییر رنگ با CSS**: از `color` یا `stroke` در CSS استفاده کنید
   ```css
   .icon-heating-on { color: #FF5722; }
   .icon-wifi-connected { color: #4CAF50; }
   ```

2. **انیمیشن loading**: برای چرخش ایکون loading از CSS استفاده کنید
   ```css
   @keyframes spin {
     from { transform: rotate(0deg); }
     to { transform: rotate(360deg); }
   }
   .icon-loading { animation: spin 1s linear infinite; }
   ```

3. **اندازه‌گذاری**: تمام ایکون‌ها با width و height قابل تنظیم هستند

