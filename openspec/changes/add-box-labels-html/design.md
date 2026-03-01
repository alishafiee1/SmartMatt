## Context

محصول تشک هوشمند RODI نیاز به دو لیبل چاپی برای جعبه محصول دارد:
1. **لیبل پلمب** (Tamper Evident Seal): روی درز بازشوی جعبه
2. **لیبل اطلاعات فنی** (Product Info Label): روی سطح صاف جعبه

این لیبل‌ها باید قابل چاپ مستقیم از مرورگر با اندازه دقیق میلیمتری باشند و برای هر دسته تولید (Batch) به راحتی قابل به‌روزرسانی باشند.

**Stakeholders:**
- طراح سخت‌افزار: H.Rostamizade (ابعاد جعبه)
- طراح نرم‌افزار: AliShafiee (محتوا و کدنویسی)

## Goals / Non-Goals

**Goals:**
- لیبل‌های HTML با اندازه دقیق چاپی (mm-based CSS)
- جداسازی داده از طراحی: `label-config.js` برای تمام مقادیر متغیر
- پشتیبانی از RTL (فارسی) و LTR (انگلیسی) در یک لیبل
- چاپ مستقیم از Chrome/Edge بدون نیاز به نرم‌افزار خاص
- قابلیت تولید QR Code به صورت inline (بدون وابستگی اینترنت)

**Non-Goals:**
- سیستم خودکار تولید لیبل (batch printing automation)
- اتصال به database برای SN/Batch
- طراحی دفترچه راهنما (فاز بعد)

## Decisions

### تصمیم 1: ساختار فایل‌ها

```
Documents/box-labels/
├── label-config.js      # تمام داده‌های متغیر — فقط این فایل برای هر batch تغییر می‌کند
├── label-styles.css     # استایل‌های مشترک + print media queries
├── label-seal.html      # لیبل پلمب (Tamper Evident)
├── label-info.html      # لیبل اطلاعات فنی
└── assets/
    ├── logo-rodi.png    # لوگوی RODI (پیشنهاد: 200x80px، PNG با پس‌زمینه شفاف)
    ├── icon-warning.svg # آیکون هشدار (مثلث زرد/قرمز)
    ├── icon-water.svg   # آیکون عدم تماس با آب
    ├── icon-wifi.svg    # آیکون WiFi
    └── qr-placeholder.png  # QR Code (تولید شده با ابزار خارجی یا inline SVG)
```

**دلیل انتخاب:**
- `label-config.js` به عنوان single source of truth برای داده‌ها
- CSS مشترک از تکرار کد جلوگیری می‌کند
- assets جداگانه برای امکان جایگزینی آسان تصاویر

### تصمیم 2: سیستم اندازه‌گذاری

از واحد `mm` در CSS استفاده می‌شود:
```css
@page { size: 100mm 30mm; margin: 0; }  /* لیبل پلمب */
@page { size: 100mm 150mm; margin: 0; } /* لیبل اطلاعات */
```

**دلیل:** مستقیم‌ترین روش برای چاپ با اندازه دقیق فیزیکی.

**ابعاد جعبه تأیید شده:** 190mm × 160mm × 60mm

بر این اساس اندازه‌های پیشنهادی لیبل‌ها:
- **لیبل پلمب:** 160mm × 25mm (روی درز طولانی‌تر جعبه)
- **لیبل اطلاعات:** 150mm × 100mm (روی سطح 190×160mm)

```css
@page { size: 160mm 25mm; margin: 0; }   /* لیبل پلمب */
@page { size: 150mm 100mm; margin: 0; }  /* لیبل اطلاعات */
```

### تصمیم 3: QR Code

**گزینه A (توصیه شده):** استفاده از کتابخانه `qrcode.js` به صورت inline در HTML
- بدون نیاز به اینترنت
- QR به صورت SVG تولید می‌شود (کیفیت بالا در هر اندازه)
- URL در `label-config.js` تعریف می‌شود

**گزینه B:** تصویر PNG از پیش تولید شده
- ساده‌تر اما نیاز به regenerate در صورت تغییر URL

**انتخاب:** گزینه A — کتابخانه `qrcode.min.js` به صورت local در `assets/` قرار می‌گیرد.

### تصمیم 4: فونت فارسی

از فونت سیستمی استفاده می‌شود تا وابستگی به اینترنت نداشته باشیم:
```css
font-family: 'Segoe UI', Tahoma, 'B Nazanin', 'Vazir', sans-serif;
```

اگر فونت Vazir یا B Nazanin روی سیستم نصب باشد، کیفیت بهتری دارد. در غیر این صورت Tahoma جایگزین می‌شود.

### تصمیم 5: ساختار label-config.js

```javascript
const LABEL_CONFIG = {
  // Product Identity
  product: {
    name: "تشک هوشمند",
    nameEn: "Smart Mattress",
    version: "V1.0",
    model: "Smart Mattress V1.0",
  },
  
  // Per-unit data (change for each batch/unit)
  unit: {
    serialNumber: "SM-1404-001",   // SN — تغییر برای هر دستگاه
    batchLot: "B1404-01",          // Batch/LOT — تغییر برای هر دسته
    productionDate: "1404/09/23",  // تاریخ تولید
    sealId: "SEAL-1404-001",       // شماره پلمب — تغییر برای هر دستگاه
  },
  
  // Electrical specs — CONFIRMED
  electrical: {
    inputVoltage: "220",   // VAC — confirmed
    frequency: "50",       // Hz — confirmed
    power: "400",          // W — confirmed
    current: "1.8",        // A — calculated: 400W / 220V ≈ 1.82A
    wifiFreq: "2.4GHz",
  },
  
  // Brand & Support
  brand: {
    name: "RODI",
    website: "rodiset.ir",
    supportPhone: "",      // شماره پشتیبانی — باید تکمیل شود
    supportTelegram: "",   // تلگرام — باید تکمیل شود
  },
  
  // QR Codes — provided as image files by user
  qr: {
    // QR تصویر سایت: assets/qr-website.png  (QR → rodiset.ir)
    // QR اتصال به RODI: assets/qr-connect.png  (QR → اتصال WiFi به دستگاه)
    websiteQrImage: "assets/qr-website.png",
    connectQrImage: "assets/qr-connect.png",
  },
  
  // Designer info (for internal label)
  meta: {
    designedBy: "Ali Shafiee",
    designDate: "1404/09/23",
    docTitle: "پروپوزال فنی برای تشک هوشمند",
  }
};
```

## Risks / Trade-offs

| ریسک | احتمال | تأثیر | راه‌حل |
|------|--------|-------|--------|
| اندازه لیبل با جعبه مطابقت نداشته باشد | پایین | بالا | اندازه‌ها در CSS قابل تنظیم هستند — جعبه 190×160×60 تأیید شد |
| چاپگر مارجین اضافه اضافه کند | بالا | متوسط | در README راهنمای چاپ، تنظیم "No Margins" ذکر شود |
| فونت فارسی روی سیستم چاپ نصب نباشد | پایین | پایین | fallback به Tahoma که روی همه Windows نصب است |
| تصویر QR تغییر کند | پایین | پایین | فایل‌های `qr-website.png` و `qr-connect.png` جایگزین می‌شوند |

## تصاویر مورد نیاز — لیست نهایی

> **توجه:** تمام تصاویر باید در پوشه `Documents/box-labels/assets/` قرار گیرند.

### تصاویری که شما تهیه می‌کنید:

| نام فایل | نوع | مشخصات پیشنهادی | محتوا |
|----------|-----|-----------------|-------|
| `logo-rodi.png` | PNG | پس‌زمینه شفاف، حداقل 400×160px | لوگوی برند RODI |
| `qr-website.png` | PNG | حداقل 200×200px، کیفیت بالا | QR Code → rodiset.ir |
| `qr-connect.png` | PNG | حداقل 200×200px، کیفیت بالا | QR Code → اتصال WiFi به دستگاه RODI |

### تصاویری که کد می‌سازد (SVG inline):

| نام | روش | محتوا |
|-----|-----|-------|
| آیکون هشدار | SVG inline در CSS/HTML | مثلث زرد با علامت ! |
| آیکون آب | SVG inline | قطره آب خط خورده |
| آیکون تهویه | SVG inline | فلش هوا خط خورده |

> این آیکون‌ها به صورت SVG مستقیم در HTML نوشته می‌شوند و نیازی به فایل جداگانه ندارند.

## Migration Plan

این یک قابلیت کاملاً جدید است و migration نیاز ندارد.

## Open Questions

1. **اطلاعات تماس پشتیبانی:** شماره تلفن یا تلگرام برای روی لیبل؟ (اختیاری — می‌توان فقط وب‌سایت نوشت)
2. **QR اتصال به RODI:** این QR دقیقاً به چه چیزی لینک می‌شود؟ صفحه راهنمای اتصال WiFi؟ یا WiFi provisioning مستقیم؟
