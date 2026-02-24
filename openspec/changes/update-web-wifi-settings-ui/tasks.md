## 1. Specification
- [ ] 1.1 Review existing `specs/web-panel/spec.md` WiFi-related requirements and align deltas.
- [ ] 1.2 Define MODIFIED requirements for WiFi provisioning interface in `specs/web-panel/spec.md` (dedicated page, network list with signal strength, click-to-fill SSID, status + IP display, forget button).
- [ ] 1.3 Define ADDED requirements for WiFi credentials deletion and related API behavior in `specs/wifi-station/spec.md`.
- [ ] 1.4 Run `openspec validate update-web-wifi-settings-ui --strict` and fix any validation issues.

## 2. Implementation - Firmware / Backend
- [ ] 2.1 Implement REST API endpoint or extended behavior for deleting WiFi credentials (e.g., `DELETE /api/wifi` or equivalent).
- [ ] 2.2 Ensure deletion clears SSID and password from NVS and safely handles active Station connection (disconnect and fallback behavior).
- [ ] 2.3 Ensure `GET /api/wifi` (or status API) returns connection state and Station IP for use in web panel header.
- [ ] 2.4 Add logging and error handling for WiFi delete and reconnect flows.

## 3. Implementation - Web Panel UI
- [ ] 3.1 Implement or update dedicated WiFi setup page (e.g., `wifi-setup.html`) to:
  - [ ] 3.1.1 Trigger WiFi network scan on load and via manual refresh.
  - [ ] 3.1.2 Display list of networks with SSID and signal strength indicator.
  - [ ] 3.1.3 On click, auto-fill SSID field from selected network.
- [ ] 3.2 Add connection status + Station IP display section at top of WiFi setup page using status API.
- [ ] 3.3 Add "Save WiFi settings" button wired to provisioning API with loading and success/error feedback.
- [ ] 3.4 Add "Forget WiFi" button wired to delete endpoint with confirmation dialog and UI state reset.
- [ ] 3.5 Ensure navigation from main page to WiFi setup and back works as specified.

## 4. Testing
- [ ] 4.1 Test first-time WiFi provisioning from SoftAP-only state (no stored credentials).
- [ ] 4.2 Test successful connection and verify Station IP is shown on WiFi page and main page.
- [ ] 4.3 Test invalid credentials (wrong password, invalid SSID) and verify error messages and retry UX.
- [ ] 4.4 Test "Forget WiFi" flow: credentials removed, Station disconnects, device remains reachable via SoftAP.
- [ ] 4.5 Test behavior when both SoftAP and Station are active (dual mode) and after WiFi deletion.
- [ ] 4.6 Cross-browser test (Chrome, Firefox, Edge, Safari) on mobile and desktop layouts.

