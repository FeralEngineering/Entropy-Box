# Build Log

This is the build history of Entropy Box.

---

## Phase 1 — The Idea

Started with a simple question:

what if we made a handheld entropy machine? (And finally found a use for this Arduino Uno...)

That was the whole thing.

No larger plan.

Just the idea of building a dedicated object whose only purpose was to gather unstable inputs and turn them into output.

Started rough.

Got weirder as it went.

---

## Phase 2 — Breadboard Prototype

Initial prototype was built on breadboard using:

- Arduino Uno
- OLED
- cheap square tactile buttons
- loose LEDs
- floating A0 entropy wire

This was where the basic system got worked out:

- mode switching
- entropy collection
- random stream generation
- password generation
- visual mode

The core architecture was established here.

---

## Phase 3 — UI Expansion

A lot of UI experiments happened early.

Tried:

- entropy bars
- lightning glyphs
- denser glitch fields
- heavier visual layers
- more aggressive standby animations

Most of it got removed.

Simpler ended up stronger.

Final UI became:

- cleaner
- more stable
- easier to read
- more consistent with the physical build

---

## Phase 4 — Memory Problems

Big problem.

Original display setup used the full framebuffer.

Memory usage was around:

~94%

This caused:

- instability
- broken rendering
- freezing
- visual corruption

Fix:

switched to U8G2 paged buffer mode.

Memory dropped to:

~49%

This was the major software fix.

Without it the build probably would have stayed unstable.

---

## Phase 5 — Hardware Upgrade

Original tactile buttons got replaced.

Final buttons became:

12mm stainless momentary buttons.

Huge improvement.

This changed:

- feel
- durability
- visual quality

At this point the build started feeling like an object instead of a prototype.

---

## Phase 6 — Layout Planning

The enclosure layout was planned physically first.

Paper templates were made for:

- OLED visible area
- OLED PCB footprint
- button diameters
- LED bezel diameters

This prevented major spacing mistakes.

---

## Phase 7 — Enclosure Work

All holes were hand cut.

Methods:

Round holes:

- drill
- widen
- file

OLED window:

- four corner holes
- center removal
- hand filing to final shape

Final cuts included:

Top face:

- 5 pink LEDs
- OLED window
- 3 buttons

Side wall:

- toggle
- white LED
- orange LED
- USB-C port

This was the most irreversible phase.

---

## Phase 8 — Harness Build

Everything was hand-wired.

Built in groups:

- pink LED signal wires
- pink LED ground bundle
- button ground bundle
- status LED wiring
- OLED power/signal
- toggle wiring

Ground bus built after physical mounting.

Had to be done this way because LED bezels load from the front.

The enclosure dictated the assembly order.

---

## Phase 9 — USB-C Conversion

The Uno’s USB-B port sat recessed too far into the enclosure.

Would have required an ugly oversized access hole.

Fix:

USB-B to USB-C adapter.

Flush mounted through side wall.

Hot glued in place.

Cleaner, better.

---

## Phase 10 — Final Assembly

Final mounting:

- Uno hot glued
- OLED hot glued
- USB-C adapter hot glued
- buttons locked with nuts
- LEDs mounted in stainless bezels

Final ground bus completed.

Everything plugged in.

Everything worked.

First try...

Rare.

---

## Final Notes

Known imperfections:

- OLED mounted *slightly* crooked
- pink LEDs are brighter than intended
- some cuts could be cleaner...

No rebuild planned.

Those imperfections belong to the object now.

Final result came out almost exactly like the original paper layout.

That almost never happens.
