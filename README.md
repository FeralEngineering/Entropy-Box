# Entropy Box

![Entropy Box Final](images/entropy-box-final.JPG)

---

![Entropy Box Prototype](images/entropy-box-prototype.JPG)

Prototype phase before enclosure, during breadboard testing and UI development.

---

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

---

## Why it exists

Entropy Box started with a simple idea:

what if we made a handheld entropy machine?

That was the whole idea.

Not because it was useful.

Not because there was a problem to solve.

Just because the idea of building a dedicated physical object around randomness felt interesting enough to justify itself.

Most randomness in software is invisible.

You call it and get an answer.

This makes it physical.

You wake it up.

You press a button.

Your timing becomes part of the output.

The room becomes part of the output.

The machine itself becomes part of the output.

That was the point.

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

This means output is influenced by:

- ambient electrical noise
- nearby electronics
- human interaction timing
- internal system timing

No two moments are exactly the same.

---

## Build

Built on an Arduino Uno with an SSD1309 OLED.

The Uno was originally just sitting around unused.

It was free, oversized, and never seemed like the right fit for anything.

Entropy Box ended up being the project that finally gave it a purpose.

Hand-wired.

Hand-drilled.

Hand-filed.

The enclosure and layout were designed physically first, then solved around the constraints of the hardware.

Finished mattered more than perfect.

---

## Documentation

- [Hardware](docs/HARDWARE.md)
- [Wiring](docs/wiring/WIRING.md)
- [Wiring Diagram](docs/wiring/WIRING_DIAGRAM.md)
- [Software](docs/SOFTWARE.md)
- [Build Log](docs/BUILD_LOG.md)

---

## Gallery

### Standby

![Entropy Box Standby](images/entropy-box-standby.jpg)

### Stream Mode

![Entropy Box Stream](images/entropy-box-stream.jpg)

### Password Mode

![Entropy Box Password](images/entropy-box-password.jpg)

### Visual Mode

![Entropy Box Visual](images/entropy-box-visual.jpg)

### Side Profile

![Entropy Box Side](images/entropy-box-side.jpg)

### USB-C Port

![Entropy Box USB-C](images/entropy-box-usbc.jpg)

---

## License

[MIT License](LICENSE)
