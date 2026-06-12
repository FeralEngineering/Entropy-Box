# Hardware

This is the full physical component list for Entropy Box.

Some parts can be swapped, but the overall structure matters.

---

## Core Compute

### Arduino Uno

Main controller.

Uses an Arduino Uno for system control, input handling, entropy mixing, and display rendering.

---

## Display

### SSD1309 OLED (128x64, I2C)

Primary interface.

Used for:

- standby screen
- mode display
- stream output
- password generation
- visual entropy rendering

Connections:

- VDD
- GND
- SDA
- SCL

---

## Controls

### 3x 12mm Stainless Momentary Pushbuttons

Used for:

- Mode 1
- Mode 2
- Mode 3

Requirements:

- momentary
- normally open

Pre-wired versions make assembly easier.

---

### 1x Metal Toggle Switch

Wake / standby control.

Not hard power.

Used as a logic-state switch.

One side to ground, one side to signal.

---

## LEDs

### 5x Pink/Purple LEDs

Top crown LEDs.

Used for breathing animation.

Each requires:

- 1 resistor
- 1 bezel

Current build note:

These are very bright.

Higher resistor values may be preferable.

---

### 1x Warm White LED

Soft power indicator.

Always on while power is present.

Requires:

- 1 resistor
- 1 bezel

---

### 1x Orange LED

Awake-state indicator.

Turns on when system is active.

Requires:

- 1 resistor
- 1 bezel

---

## Resistors

Used for all LEDs.

Total:

- 5 for pink LEDs
- 1 for white LED
- 1 for orange LED

Value depends on brightness preference.

Higher resistance = dimmer output.

---

## Entropy Probe

### Floating analog lead

Connected to A0.

Used as the primary environmental noise input.

Possible variants:

- loose wire
- exposed antenna
- ferrite coil
- inductive probe

Current build uses a simple floating wire.

---

## Power Interface

### USB-B to USB-C Adapter

Used to convert the recessed Arduino Uno USB-B port into a cleaner external USB-C inlet.

Mounted flush in the enclosure wall.

Strongly recommended for enclosure builds.

---

## Enclosure

### Black project box

Dimensions:

- Length: 156 mm
- Width: 90 mm
- Height: 60.5 mm

Requirements:

- removable lid
- enough depth for Uno
- enough face area for OLED + LEDs + buttons

Final layout:

Top row:

- 5 pink LEDs

Middle:

- OLED display

Bottom:

- 3 buttons

Side wall:

- toggle switch
- white LED
- orange LED
- USB-C power inlet

---

## Mounting Materials

Used in final build:

- hot glue
- heat shrink
- hookup wire
- Dupont connectors

No custom PCB.

Entire build is hand-wired.
