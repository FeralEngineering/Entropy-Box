# Wiring

This is the exact wiring layout used in the finished Entropy Box build.

The current build uses direct hand wiring with no PCB.

Ground and 5V are distributed through shared buses.

---

## Pin Map

### Buttons

| Function | Pin |
|---|---|
| Mode 1 (Stream) | D2 |
| Mode 2 (Password) | D4 |
| Mode 3 (Visual) | D12 |

Buttons are wired:

```text
button leg 1 → signal pin
button leg 2 → ground
```

Buttons have no polarity.

---

### Toggle

| Function | Pin |
|---|---|
| Wake / Standby | D7 |

Wired:

```text
one side → D7
one side → ground
```

Uses:

```cpp
INPUT_PULLUP
```

Meaning:

- HIGH = off
- LOW = on

The toggle is used for software wake/sleep state control and does not disconnect power.

---

### Status LEDs

| LED | Pin |
|---|---|
| Orange (awake) | D8 |
| White (soft power) | 5V |

Orange:

```text
D8 → resistor → anode
cathode → ground
```

White:

```text
5V → resistor → anode
cathode → ground
```

---

### Crown LEDs

| Position | Pin |
|---|---|
| Left 1 | D3 |
| Left 2 | D5 |
| Center | D6 |
| Right 1 | D9 |
| Right 2 | D10 |

Each wired:

```text
Arduino pin → resistor → anode
cathode → ground
```

All five are PWM-controlled for breathing.

---

### OLED

| OLED Pin | Arduino Pin |
|---|---|
| VDD | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

Display uses I2C communication.

---

### Entropy Probe

| Function | Pin |
|---|---|
| Floating analog input | A0 |

Current build:

```text
A0 → floating wire
```

The entropy probe is left electrically floating with no pull-up, pull-down, or shielding.

---

## Ground Bus

All grounds combine into one shared trunk.

Includes:

- 5 pink LED cathodes
- 3 button grounds
- white LED ground
- orange LED ground
- toggle ground
- OLED ground

Final:

```text
all grounds → single bus → Uno GND
```

---

## 5V Split

The 5V rail is split to:

- OLED VDD
- White LED resistor

Wired:

```text
5V → split →
    OLED VDD
    White LED resistor
```

---

## LED Polarity

Important:

```text
long leg = anode (+)
short leg = cathode (-)
flat side = cathode (-)
```

Resistors go on the anode side in this build.

---

## Build Notes

This build was assembled in phases:

1. signal wires first
2. LEDs soldered second
3. grounds grouped after physical mounting
4. final bus soldered after enclosure fitment

Assembly order was determined by enclosure access and front-mounted LED bezels.

The enclosure dictated the wiring order.
