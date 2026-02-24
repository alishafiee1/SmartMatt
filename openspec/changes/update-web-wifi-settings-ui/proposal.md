## Why
Current WiFi setup behavior is partially specified and implemented, but the user experience on the dedicated WiFi setup page is not complete:
- Users need a clear, modern page to configure WiFi in a separate screen.
- Users must be able to forget/remove stored WiFi credentials without reflashing the device.
- The WiFi setup page should clearly show connection status and assigned IP when Station mode connects.

## What Changes
- Add and refine requirements for the dedicated WiFi setup page in the web panel:
  - Ensure WiFi settings open in a dedicated page.
  - Display scanned networks with signal strength, clickable to auto-fill SSID.
  - Show connection status and Station IP on the WiFi setup page when connected.
  - Add a "Forget WiFi" action to remove saved credentials.
- Extend WiFi Station capability spec with explicit "delete WiFi credentials" behavior and API.

## Impact
- Affected specs:
  - `specs/web-panel/spec.md` (WiFi Provisioning Interface)
  - `specs/wifi-station/spec.md` (WiFi credentials management and status)
- Affected code (high level):
  - Web panel frontend (HTML/CSS/JS) for WiFi setup page.
  - REST API handler(s) for WiFi provisioning and credential deletion.
  - WiFi station management in firmware (NVS credential delete + reconnection logic).

