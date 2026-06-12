# Build Log

Build history for Entropy Box.

---

## Phase 1 — Initial Concept

The project began as a dedicated handheld random output device built around physical entropy sources.

Initial goals:

- collect unstable analog input
- mix timing variation into a shared entropy pool
- generate visible outputs from that pool

The Arduino Uno was selected as the base platform because it was already available and had enough I/O for the planned layout.

---

## Phase 2 — Breadboard Prototype

Initial prototype used:

- Arduino Uno
- SSD1309 OLED
- temporary tactile buttons
- loose LEDs
- floating A0 entropy lead

This phase established:

- mode switching
- entropy collection
- stream generation
- password generation
- visual mode rendering

Core logic was developed and tested here.

---

## Phase 3 — UI Iteration

Several display elements were tested during development, including:

- entropy bars
- symbol indicators
- larger glitch fields
- denser visual layers
- faster standby animation

These were reduced or removed to simplify rendering and improve readability.

Final UI prioritized:

- stability
- clarity
- lower memory use

---

## Phase 4 — Memory Optimization

Initial display code used full framebuffer mode.

Measured SRAM usage:

```text
~94%
```

This caused:

- instability
- rendering corruption
- freezes

The display was switched to U8G2 paged buffer mode.

Measured SRAM usage after change:

```text
~49%
```

This resolved the stability issues.

---

## Phase 5 — Button Replacement

The original temporary tactile buttons were replaced with 12mm stainless steel momentary pushbuttons.

This improved:

- durability
- input feel
- enclosure fit

---

## Phase 6 — Layout Planning

The enclosure layout was planned using paper templates.

Templates were made for:

- OLED visible area
- OLED PCB footprint
- button diameter
- LED bezel diameter

This was used to verify spacing before drilling.

---

## Phase 7 — Enclosure Work

All enclosure cuts were done by hand.

Methods:

Round holes:

- drill
- widen
- file

OLED window:

- corner drill points
- center removal
- hand filing to final dimensions

Final cuts:

Top panel:

- 5 LED holes
- OLED window
- 3 button holes

Side panel:

- toggle switch
- white LED
- orange LED
- USB adapter opening

---

## Phase 8 — Internal Wiring

The unit was fully hand-wired.

Wiring groups:

- pink LED signal lines
- pink LED ground bus
- button ground bus
- status LEDs
- OLED power and I2C
- toggle wiring

Assembly order was determined by enclosure access and front-loaded LED bezels.

---

## Phase 9 — USB-C Adapter Installation

The Arduino Uno’s USB-B port sat recessed too far inside the enclosure for direct access.

A USB-B to USB-C adapter was installed through the side wall to provide external power access.

The adapter extends slightly outward from the enclosure.

---

## Phase 10 — Final Assembly

Final assembly included:

- mounting the Uno
- mounting the OLED
- securing the USB adapter
- locking buttons in place
- installing LEDs and bezels
- completing the ground bus

The system powered on and functioned correctly after assembly.

---

## Final Notes

Current build notes:

- OLED is mounted slightly off-center
- pink LEDs are brighter than originally intended
- enclosure cuts show minor hand-fit variation

No major rebuilds are planned.

The current unit is considered the finished working build.
