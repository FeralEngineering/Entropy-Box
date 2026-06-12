# Wiring Diagram

This is the physical wiring layout of Entropy Box.

Simplified for clarity.

Use this alongside [WIRING.md](WIRING.md)

---

## Main Signal Layout

```text
                           +----------------------+
                           |     Arduino Uno      |
                           |                      |
A0  <----------------------| A0   Entropy Probe   |
                           |                      |
D2  <----------------------| BTN1 (Stream)        |
D4  <----------------------| BTN2 (Password)      |
D12 <----------------------| BTN3 (Visual)        |
                           |                      |
D7  <----------------------| Toggle Switch        |
                           |                      |
D8  ---------------------->| Orange LED           |
                           |                      |
D3  ---------------------->| Pink LED 1           |
D5  ---------------------->| Pink LED 2           |
D6  ---------------------->| Pink LED 3           |
D9  ---------------------->| Pink LED 4           |
D10 ---------------------->| Pink LED 5           |
                           |                      |
A4  <--------------------->| SDA (OLED)           |
A5  <--------------------->| SCL (OLED)           |
                           |                      |
5V  ---------------------->| OLED VDD             |
5V  ---------------------->| White LED            |
                           |                      |
GND <----------------------| Shared Ground Bus    |
                           +----------------------+
```

---

## Ground Bus

All grounds are joined into one shared bus:

```text
Pink LED 1 cathode ----\
Pink LED 2 cathode -----\
Pink LED 3 cathode ------\
Pink LED 4 cathode -------\
Pink LED 5 cathode --------\
BTN1 ground ---------------\
BTN2 ground ----------------\
BTN3 ground -----------------+-----> Arduino GND
Toggle ground --------------/
Orange LED cathode --------/
White LED cathode --------/
OLED GND -----------------/
```

---

## 5V Split

```text
Arduino 5V
   |
   +------> OLED VDD
   |
   +------> White LED resistor -> White LED anode
```

---

## LED Signal Pattern

Pink LEDs:

```text
Arduino Pin -> Resistor -> LED Anode
LED Cathode -> Ground Bus
```

Orange LED:

```text
D8 -> Resistor -> LED Anode
LED Cathode -> Ground Bus
```

White LED:

```text
5V -> Resistor -> LED Anode
LED Cathode -> Ground Bus
```

---

## Button Pattern

```text
Arduino Pin -> Button -> Ground
```

Uses internal pull-up.

Idle:

HIGH

Pressed:

LOW

---

## Toggle Pattern

```text
D7 -> Toggle -> Ground
```

Uses internal pull-up.

Toggle up/down changes wake state.

Not hard power.
