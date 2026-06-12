# Entropy Box

Entropy Box is a handheld entropy-driven device built around the idea of harvesting unstable input from the physical world and turning it into usable randomness.

At its core, it mixes analog noise, timer jitter, and human interaction timing into an evolving entropy pool, then uses that pool to generate output.

It has three modes:

- **Stream** — continuous random character generation  
- **Password** — generates 16-character passwords  
- **Visual** — renders randomized abstract patterns  

The device uses a floating analog pin as an entropy probe, sampling environmental electrical noise and combining it with button timing and internal clock variance.

Physically, it’s built into a black project enclosure with:

- a centered OLED display
- five breathing crown LEDs
- three stainless steel mode buttons
- a side-mounted toggle
- white and orange status LEDs
- flush-mounted USB-C power

This project is less about utility and more about physicalizing randomness.

Most random generators are invisible.

This one has weight, light, switches, and a body.

It makes randomness feel like something you interact with instead of something you call from software.

---

## Modes

### Mode 1 — Stream

Outputs a continuous stream of randomized characters.

Can be started and stopped at will.

Useful for observing live entropy output over time.

---

### Mode 2 — Password

Generates a new 16-character password on each activation.

Stores the last three generated passwords on screen.

---

### Mode 3 — Visual

Builds randomized visual compositions from:

- pixel noise
- blocks
- lines

Each press generates a new pattern.

---

## Entropy Sources

Current entropy inputs:

- floating analog input (A0)
- repeated analog sampling
- microsecond timer jitter
- millisecond timer state
- button press timing
- button hold duration

These are continuously mixed into an entropy pool and used to reseed the generator.

---

## Build

Built on an Arduino Uno with an SSD1309 OLED.

Hand-wired.

Hand-drilled.

Hand-filed.

The enclosure and layout were designed physically first, then solved around the constraints of the hardware.

Nothing is simulated.

Everything is embodied.

---

See:

- `HARDWARE.md`
- `WIRING.md`
- `SOFTWARE.md`
- `PHILOSOPHY.md`
- `BUILD_LOG.md`
