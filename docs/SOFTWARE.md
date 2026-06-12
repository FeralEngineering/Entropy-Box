# Software

This file documents the internal logic of Entropy Box.

The system is built around state transitions, entropy mixing, and mode-specific output generation.

The code runs on an Arduino Uno using the U8G2 display library.

---

## Core Structure

Main code sections:

- pin definitions
- mode state
- breathing animation
- button handling
- entropy system
- mode logic
- display rendering

The system is event-driven.

It spends most of its life waiting for interaction.

---

## State System

Entropy Box has four states:

```cpp
MODE_STANDBY
MODE_STREAM
MODE_PASSWORD
MODE_VISUAL
```

---

### Standby

Default state after wake.

Displays:

- title
- mode list
- glitch window

No active generation.

Waiting for input.

---

### Stream

Continuous random character output.

Updates on interval.

Can be toggled on/off.

---

### Password

Generates 16-character passwords.

Stores the last three.

Generates only on demand.

---

### Visual

Builds randomized visual compositions.

Static image generation.

New pattern on each press.

---

## Wake Logic

Wake is controlled by the toggle switch.

Uses:

```cpp id="zrvshw"
INPUT_PULLUP
```

Meaning:

```text id="kntwln"
HIGH = off
LOW = on
```

Wake behavior:

Toggle ON:

- OLED powers up
- orange LED turns on
- enters standby

Toggle OFF:

- OLED sleeps
- orange LED off
- pink LEDs off

White LED remains powered.

---

## Button Logic

Buttons are debounced.

Debounce time:

```cpp id="p7k0lu"
150ms
```

Each button tracks:

- last state
- press time

Used for:

- short press detection
- long press detection

---

### Short Press

Used for:

- entering modes
- activating mode functions

---

### Long Press

Threshold:

```cpp id="fjlwm6"
800ms
```

Long press from any mode:

Returns to standby.

Stops stream mode.

---

## Breathing System

The five pink LEDs use PWM breathing.

Pins:

```text id="rddtto"
3, 5, 6, 9, 10
```

Behavior:

- rises from 0 → 255
- falls from 255 → 0
- loops continuously

Update interval:

```cpp id="fjlwm6"
15ms
```

Step size:

```cpp id="kprn80"
2
```

Pure atmosphere.

No information.

---

## Entropy System

This is the core of the machine.

Entropy is accumulated into:

```cpp id="yxnq1q"
uint32_t entropyPool
```

This pool is continuously mixed and reused.

---

## Entropy Sources

### Floating analog input

Uses:

```cpp id="r9w7o4"
analogRead(A0)
```

A0 is left floating.

This picks up:

- ambient electrical noise
- EMI
- body capacitance
- nearby electronics

---

### Repeated analog sampling

Instead of one read:

```cpp id="3n53v2"
stirFromAnalog(samples)
```

Samples multiple times:

- 8 on startup
- 4 before mode generation

This improves variability.

---

### Timer jitter

Uses:

```cpp id="p1a9yz"
micros()
```

Mixed into the pool alongside analog readings.

Microsecond variance adds instability.

---

### Millisecond timer state

Uses:

```cpp id="vfek9u"
millis()
```

Used during:

- setup
- reseeding
- button release

---

### Button timing

Every button release adds:

```cpp id="b5u9r2"
mixEntropy(now)
```

Press timing becomes part of the entropy pool.

---

### Hold duration

Also adds:

```cpp id="4ntkkp"
mixEntropy(held)
```

Duration of interaction affects generation.

---

## Entropy Mixing

Values are folded into the pool using:

```cpp
entropyPool ^= v + 0x9e3779b9UL + (entropyPool << 6) + (entropyPool >> 2);
```

Simple but effective.

Keeps entropy moving.

Prevents stale reseeds.

---

## Reseeding

Before major generation:

```cpp id="dk3c1r"
randomSeed(entropyPool ^ millis());
```

This happens before:

- stream start
- password generation
- visual generation

Randomness is constantly refreshed.

---

## Stream Mode

Uses:

```cpp id="s1nwr0"
STREAM_COLS = 18
STREAM_ROWS = 3
```

Maintains a rolling character buffer.

Characters come from:

- letters
- numbers
- symbols

Update interval:

```cpp id="mdmmtw"
70ms
```

---

## Password Mode

Creates:

```cpp id="r84l7t"
16-character strings
```

Character set:

- uppercase
- lowercase
- numbers
- symbols

Stores:

```cpp id="j1s0jx"
last 3 passwords
```

---

## Visual Mode

Generates layered randomized images:

### Layer 1

Static / starfield

---

### Layer 2

Random filled or framed boxes

---

### Layer 3

Diagonal glitch lines

Each activation builds a new composition.

No persistence.

---

## Display System

Uses:

U8G2 paged buffer mode.

Important.

Original full buffer caused memory issues.

Paged mode reduced memory use from:

```text id="x5v4be"
~94% → ~49%
```

This was a critical stability fix.

---

## Standby Glitch Window

Standby includes a small 4x4 block-grid glitch field.

Refresh interval:

```cpp id="vrk4z2"
1500ms
```

Purely aesthetic.

Used to give standby some movement.

---

## Design Notes

Entropy Box is intentionally simple.

No networking.

No storage.

No logging.

No external APIs.

It only exists in the moment it is being used.

That was intentional.
