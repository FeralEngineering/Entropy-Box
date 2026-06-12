# Software

This file documents the internal logic of Entropy Box.

The code runs on an Arduino Uno using the U8G2 display library.

---

## Core Structure

Main code sections:

- pin definitions
- mode state
- LED animation
- button handling
- entropy collection
- mode logic
- display rendering

The system is event-driven and waits for user input between actions.

---

## State System

Entropy Box has four operating states:

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

Outputs a continuous stream of randomized characters.

Updates on a fixed interval.

Can be toggled on and off.

---

### Password

Generates 16-character passwords.

Stores the last three generated passwords.

Runs only when triggered.

---

### Visual

Generates randomized visual compositions.

Each activation builds a new image.

No persistence.

---

## Wake Logic

Wake state is controlled by the side toggle switch.

Uses:

```cpp
INPUT_PULLUP
```

State:

```text
HIGH = standby/off
LOW = active/on
```

When switched on:

- OLED wakes
- orange LED turns on
- system enters standby

When switched off:

- OLED sleeps
- orange LED turns off
- pink LEDs stop

The white side LED remains powered directly from 5V.

---

## Button Logic

Buttons use debounce logic.

Debounce interval:

```cpp
150ms
```

Each button tracks:

- previous state
- press start time

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

```cpp
800ms
```

Long press from any mode:

- returns to standby
- stops stream mode if active

---

## LED System

The five top LEDs use PWM breathing.

Pins:

```text
3, 5, 6, 9, 10
```

Behavior:

- brightness ramps up from 0 to 255
- brightness ramps down from 255 to 0
- repeats continuously

Update interval:

```cpp
15ms
```

Step size:

```cpp
2
```

The breathing LEDs are decorative and are not tied to system state.

---

## Entropy System

Entropy is stored in:

```cpp
uint32_t entropyPool
```

This pool is continuously mixed and used for reseeding.

---

## Entropy Sources

### Floating analog input

Uses:

```cpp
analogRead(A0)
```

A0 is intentionally left floating.

This allows it to pick up variable analog noise from the surrounding electrical environment.

---

### Repeated analog sampling

Uses:

```cpp
stirFromAnalog(samples)
```

Sampling count:

- 8 during startup
- 4 before mode generation

This increases variability.

---

### Timer jitter

Uses:

```cpp
micros()
```

Microsecond timing is mixed into the entropy pool.

---

### Millisecond timer state

Uses:

```cpp
millis()
```

Used during:

- startup
- reseeding
- button release

---

### Button timing

Each button release adds:

```cpp
mixEntropy(now)
```

This adds timing variation based on interaction.

---

### Hold duration

Button hold time also adds:

```cpp
mixEntropy(held)
```

This changes the entropy pool based on interaction length.

---

## Entropy Mixing

Values are mixed using:

```cpp
entropyPool ^= v + 0x9e3779b9UL + (entropyPool << 6) + (entropyPool >> 2);
```

This updates the pool with each new entropy input.

---

## Reseeding

Before generation:

```cpp
randomSeed(entropyPool ^ millis());
```

This occurs before:

- stream mode
- password generation
- visual generation

---

## Stream Mode

Uses:

```cpp
STREAM_COLS = 18
STREAM_ROWS = 3
```

Maintains a rolling character buffer.

Character source includes:

- letters
- numbers
- symbols

Update interval:

```cpp
70ms
```

---

## Password Mode

Generates:

```cpp
16-character strings
```

Character set includes:

- uppercase letters
- lowercase letters
- numbers
- symbols

Stores:

```cpp
last 3 passwords
```

---

## Visual Mode

Builds layered randomized images:

### Layer 1

Pixel noise / starfield

---

### Layer 2

Random filled or framed boxes

---

### Layer 3

Diagonal glitch lines

Each activation builds a new composition.

---

## Display System

Uses U8G2 paged buffer mode.

The original full framebuffer implementation used too much SRAM and caused instability.

Switching to paged mode reduced memory use from:

```text
~94% → ~49%
```

This resolved display corruption and freezes.

---

## Standby Glitch Window

Standby includes a small 4x4 animated block grid.

Refresh interval:

```cpp
1500ms
```

This is a visual-only element.
