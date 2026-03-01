## ADDED Requirements

### Requirement: Central Configuration File
The system SHALL provide a single `label-config.js` file as the only place where variable label data (serial number, batch, electrical specs, contact info, QR URLs) is defined, so that updating any label data requires changing only one file.

#### Scenario: Batch update
- **WHEN** a new production batch is prepared
- **THEN** only `label-config.js` needs to be updated (SN, Batch, date)
- **THEN** both `label-seal.html` and `label-info.html` automatically reflect the new data on next browser load

#### Scenario: Contact info update
- **WHEN** support phone number or website changes
- **THEN** only `label-config.js` is edited
- **THEN** all label pages show the updated contact info

---

### Requirement: Tamper Evident Seal Label
The system SHALL provide a `label-seal.html` page that renders a printable Tamper Evident Seal label sized to fit the box opening seam, containing: brand name, seal authenticity title (bilingual FA/EN), tamper evidence statement, unique seal ID, and an optional QR code for verification.

#### Scenario: Print seal label
- **WHEN** user opens `label-seal.html` in Chrome/Edge and prints
- **THEN** the printed output matches the defined physical dimensions (default 100mm × 30mm)
- **THEN** margins are zero and no browser headers/footers appear
- **THEN** all text is legible at the printed size

#### Scenario: Seal ID display
- **WHEN** `label-config.js` has `unit.sealId` set
- **THEN** the seal label shows the SEAL-ID value
- **WHEN** `unit.sealId` is empty
- **THEN** the label shows a blank fill-in line `________`

#### Scenario: QR code on seal
- **WHEN** `label-config.js` has `qr.sealVerifyUrl` set
- **THEN** a scannable QR code is rendered inline (no internet required)
- **THEN** scanning the QR opens the verification URL

---

### Requirement: Product Information Label
The system SHALL provide a `label-info.html` page that renders a printable product information label for the flat surface of the box, containing: logo, product name/model, identification section (SN, Batch, production date), electrical & safety section, three safety warnings with icons, and a QR code with support/manual info.

#### Scenario: Print info label
- **WHEN** user opens `label-info.html` in Chrome/Edge and prints
- **THEN** the printed output matches the defined physical dimensions (default 100mm × 150mm)
- **THEN** margins are zero and no browser headers/footers appear
- **THEN** all text including Persian is legible

#### Scenario: Layout structure
- **WHEN** the label is rendered
- **THEN** the top section shows logo + product name
- **THEN** the middle-left section shows QR code + scan instruction
- **THEN** the middle-right section shows SN, Batch, production date
- **THEN** the bottom section shows 3 safety warnings + electrical specs

#### Scenario: Electrical specs display
- **WHEN** `label-config.js` has electrical values set
- **THEN** the label shows input voltage, frequency, power, and current
- **WHEN** any electrical value is empty
- **THEN** a blank fill-in placeholder `____` is shown instead

#### Scenario: Safety warnings
- **WHEN** the label is rendered
- **THEN** exactly 3 warnings are displayed: (1) avoid water/high humidity, (2) do not block ventilation, (3) disconnect if cable/body is damaged
- **THEN** each warning has a clear WARNING/CAUTION prefix
- **THEN** warning text is not smaller than 7pt to remain legible

---

### Requirement: Shared Print Styles
The system SHALL provide a `label-styles.css` file with shared CSS including `@page` rules for exact mm dimensions, `@media print` rules to hide non-print elements (buttons, borders), RTL/LTR text direction support, and CSS variables for colors and fonts.

#### Scenario: Print button hidden
- **WHEN** user prints any label page
- **THEN** the "Print" button is not visible in the printed output

#### Scenario: CSS variables for customization
- **WHEN** a developer needs to change the primary color or font
- **THEN** only the CSS variable definition in `label-styles.css` needs to change
- **THEN** the change applies to all label pages

---

### Requirement: Offline QR Code Generation
The system SHALL generate QR codes entirely in the browser without any internet dependency, using a locally stored `qrcode.min.js` library in the `assets/` folder.

#### Scenario: QR generation offline
- **WHEN** user opens a label page with no internet connection
- **THEN** QR codes are still generated and displayed correctly
- **THEN** the QR encodes the URL defined in `label-config.js`

---

### Requirement: Print Guide Documentation
The system SHALL include a `README.md` in the `box-labels/` folder documenting: how to print each label with correct Chrome/Edge settings (Margins=None, Scale=100%), how to update per-batch data in `label-config.js`, and how to adjust label dimensions if the box size changes.

#### Scenario: New team member prints labels
- **WHEN** a team member reads `README.md`
- **THEN** they can successfully print both labels with correct dimensions on first attempt
- **THEN** they know exactly which values to change for a new batch
