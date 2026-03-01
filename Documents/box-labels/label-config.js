// Central configuration for all box labels --- all variable data lives here; change only this file per batch/unit

const LABEL_CONFIG = {

  // ─── Product Identity ───────────────────────────────────────────────────────
  product: {
    name: "تشک هوشمند",          // Persian product name
    nameEn: "Smart Mattress",     // English product name
    version: "V1.0",
    model: "Smart Mattress V1.0",
  },

  // ─── Per-unit data — CHANGE FOR EACH BATCH/UNIT ─────────────────────────────
  unit: {
    serialNumber:    "SM-1404-001",   // SN — unique per device
    batchLot:        "B1404-01",      // Batch/LOT — same for all units in a batch
    productionDate:  "1404/09/23",    // Production date (Jalali)
    sealId:          "SEAL-1404-001", // Tamper seal ID — unique per device
  },

  // ─── Electrical Specs — CONFIRMED ───────────────────────────────────────────
  electrical: {
    inputVoltage: "220",   // VAC
    frequency:    "50",    // Hz
    power:        "400",   // W
    current:      "1.8",   // A  (calculated: 400W / 220V ≈ 1.82A)
    wifiFreq:     "2.4GHz",
  },

  // ─── Brand & Support ─────────────────────────────────────────────────────────
  brand: {
    name:             "RODI",
    website:          "rodiset.ir",
    supportPhone:     "",   // TODO: fill in support phone number
    supportTelegram:  "",   // TODO: fill in Telegram handle
  },

  // ─── QR Code images — provided as PNG files ──────────────────────────────────
  // Place files in assets/ folder with these exact names
  qr: {
    websiteQrImage: "assets/qr-website.png",  // QR → rodiset.ir
    connectQrImage: "assets/qr-connect.png",  // QR → WiFi connection guide
  },

  // ─── Internal meta (not printed on consumer labels) ─────────────────────────
  meta: {
    designedBy:  "RODI TEAM",
    designDate:  "1404/09/23",
    docTitle:    "پروپوزال فنی برای تشک هوشمند",
  },
};
