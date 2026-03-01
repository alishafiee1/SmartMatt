# RODI Box Labels — Print Guide

لیبل‌های چاپی جعبه محصول تشک هوشمند RODI.

---

## فایل‌ها

| فایل | توضیح |
|------|-------|
| `label-config.js` | **تنها فایلی که برای هر Batch/دستگاه باید تغییر کند** |
| `label-styles.css` | استایل‌های مشترک (لازم نیست تغییر کند) |
| `label-seal.html` | لیبل پلمب اصالت — 160mm × 25mm |
| `label-info.html` | لیبل اطلاعات فنی — 150mm × 100mm |
| `assets/` | تصاویر مورد نیاز (لوگو، QR Code ها) |

---

## تصاویر مورد نیاز در پوشه `assets/`

قبل از چاپ، این فایل‌ها را در پوشه `assets/` قرار دهید:

| نام فایل | وضعیت | مشخصات |
|----------|--------|---------|
| `logo-rodi.png` | ⏳ منتظر | لوگوی RODI — PNG شفاف، حداقل 400×160px |
| `qr-website.png` | ⏳ منتظر | QR Code سایت rodiset.ir — حداقل 200×200px |
| `qr-connect.png` | ⏳ منتظر | QR Code اتصال به دستگاه — حداقل 200×200px |

> اگر فایل‌ها وجود نداشته باشند، لیبل‌ها با placeholder نمایش داده می‌شوند.

---

## به‌روزرسانی اطلاعات برای هر Batch

فقط فایل `label-config.js` را باز کنید و بخش `unit` را تغییر دهید:

```javascript
unit: {
  serialNumber:   "SM-1404-002",   // ← شماره سریال جدید
  batchLot:       "B1404-02",      // ← شماره Batch جدید
  productionDate: "1404/10/01",    // ← تاریخ تولید جدید
  sealId:         "SEAL-1404-002", // ← شماره پلمب جدید
},
```

پس از ذخیره، هر دو لیبل به صورت خودکار اطلاعات جدید را نمایش می‌دهند.

---

## راهنمای چاپ در Chrome / Edge

### لیبل پلمب (`label-seal.html`) — 160mm × 25mm

1. فایل را در Chrome یا Edge باز کنید
2. روی دکمه **"Print Seal Label"** کلیک کنید (یا `Ctrl+P`)
3. تنظیمات چاپ:
   - **Destination:** پرینتر مورد نظر
   - **Paper size:** Custom — `160mm × 25mm`
   - **Margins:** None (بدون حاشیه)
   - **Scale:** 100%
   - **Background graphics:** ✅ روشن (برای چاپ رنگ‌ها)
4. Print

### لیبل اطلاعات فنی (`label-info.html`) — 150mm × 100mm

1. فایل را در Chrome یا Edge باز کنید
2. روی دکمه **"Print Info Label"** کلیک کنید (یا `Ctrl+P`)
3. تنظیمات چاپ:
   - **Destination:** پرینتر مورد نظر
   - **Paper size:** Custom — `150mm × 100mm`
   - **Margins:** None (بدون حاشیه)
   - **Scale:** 100%
   - **Background graphics:** ✅ روشن
4. Print

---

## تغییر اندازه لیبل

اگر ابعاد جعبه تغییر کرد، اندازه لیبل را در بخش `<style>` هر HTML فایل تنظیم کنید:

**در `label-seal.html`:**
```css
@page { size: 160mm 25mm; margin: 0; }  /* ← اینجا تغییر دهید */
.label { width: 160mm; height: 25mm; }  /* ← و اینجا */
```

**در `label-info.html`:**
```css
@page { size: 150mm 100mm; margin: 0; }  /* ← اینجا تغییر دهید */
.label { width: 150mm; height: 100mm; }  /* ← و اینجا */
```

---

## نکات مهم

- **مرورگر توصیه شده:** Chrome یا Edge (بهترین پشتیبانی از CSS print)
- **فونت فارسی:** Segoe UI (روی ویندوز پشتیبانی خوبی از فارسی دارد)، در غیر این صورت Tahoma
- **تست اولیه:** قبل از چاپ روی کاغذ لیبل، یک بار روی کاغذ معمولی چاپ کنید تا اندازه را تأیید کنید
- **اسکن QR:** بعد از چاپ، QR Code را با موبایل اسکن کنید تا صحت آن را تأیید کنید

---

*RODI Smart Mattress — Documents/box-labels*
