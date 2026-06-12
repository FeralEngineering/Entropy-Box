#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// ---------- DISPLAY OBJECT ----------
// Paged buffer constructor keeps SRAM usage low on the Uno.
U8G2_SSD1309_128X64_NONAME0_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// ---------- PIN DEFINITIONS ----------
const uint8_t TOGGLE_PIN  = 7;    // big metal toggle to GND, use INPUT_PULLUP
const uint8_t ORANGE_LED  = 8;    // "awake" LED

// Pink breathing LEDs on PWM pins
const uint8_t PINK_LEDS[5] = {3, 5, 6, 9, 10};

// Mode buttons (wired to GND, use INPUT_PULLUP)
const uint8_t BTN1_PIN = 2;   // Mode 1: Stream
const uint8_t BTN2_PIN = 4;   // Mode 2: Password
const uint8_t BTN3_PIN = 12;  // Mode 3: Visual

// ---------- MODE STATE ----------
enum Mode {
  MODE_STANDBY = 0,
  MODE_STREAM  = 1,
  MODE_PASSWORD = 2,
  MODE_VISUAL  = 3
};

Mode currentMode = MODE_STANDBY;

// ---------- TOGGLE STATE ----------
bool lastToggleState = HIGH;  // INPUT_PULLUP: HIGH = off, LOW = on

// ---------- BREATHING ANIMATION ----------
int breathValue = 0;              // 0..255
int breathDirection = 1;          // 1 = up, -1 = down
unsigned long lastBreathUpdate = 0;
const unsigned long BREATH_INTERVAL_MS = 15;

// ---------- BUTTON STATE / DEBOUNCE ----------
unsigned long lastButtonTime = 0;
const unsigned long DEBOUNCE_MS = 150;

struct ButtonState {
  uint8_t pin;
  bool lastState;
  unsigned long pressedAt;
};

ButtonState btn1 = {BTN1_PIN, HIGH, 0};
ButtonState btn2 = {BTN2_PIN, HIGH, 0};
ButtonState btn3 = {BTN3_PIN, HIGH, 0};

const unsigned long LONG_PRESS_MS = 800;

// ---------- ENTROPY POOL ----------
uint32_t entropyPool = 0;
uint8_t  entropyLevel = 0;           // internal activity level, not displayed
unsigned long lastEntropyDecay = 0;
const unsigned long ENTROPY_DECAY_MS = 80;

// ---------- STREAM MODE STATE ----------
const uint8_t STREAM_COLS = 18;  // keep within width
const uint8_t STREAM_ROWS = 3;
char streamBuf[STREAM_ROWS][STREAM_COLS];
uint16_t streamIndex = 0;
bool streamRunning = false;
unsigned long lastStreamStep = 0;
const unsigned long STREAM_INTERVAL_MS = 70;

// ---------- PASSWORD MODE STATE ----------
const uint8_t MAX_PASSWORDS = 3;
const uint8_t PASSWORD_LEN = 16;
char passwords[MAX_PASSWORDS][PASSWORD_LEN + 1];
uint8_t pwdCount = 0;

// ---------- STANDBY GLITCH WINDOW ----------
unsigned long lastStandbyRefresh = 0;
// Slow refresh to reduce display redraw load.
const unsigned long STANDBY_GLITCH_INTERVAL_MS = 1500; // ms between glitch redraws

// ---------- FORWARD DECLARATIONS ----------
void updateBreathing();
void setAllPink(uint8_t value);
void handleButtons();
void handleShortPress(uint8_t pin);
void handleLongPress(uint8_t pin);

void showMode(Mode m);

void drawStandbyPage();
void drawStreamPage();
void drawPasswordPage();
void drawVisualPage();

void drawStandbyScreen();
void drawStreamScreen();
void drawPasswordScreen();
void drawVisualScreen();

void initStreamBuffer();
void updateStreamContent();
char randomChar();
void generatePassword();

void mixEntropy(uint32_t v);
void stirFromAnalog(uint8_t samples = 4);
void reseedFromEntropy();
void updateEntropyDecay();

void drawGlitchWindow();

// ======================================================
// ENTROPY HELPERS
// ======================================================
void mixEntropy(uint32_t v) {
  // simple 32-bit mixer (hash-ish)
  entropyPool ^= v + 0x9e3779b9UL + (entropyPool << 6) + (entropyPool >> 2);
}

void stirFromAnalog(uint8_t samples) {
  for (uint8_t i = 0; i < samples; i++) {
    uint16_t v = analogRead(A0);                    // floating pin noise
    mixEntropy(((uint32_t)v << 16) ^ micros());     // mix reading + timer
    if (entropyLevel < 245) entropyLevel += 10;     // bump internal level
    delayMicroseconds(200);
  }
}

void reseedFromEntropy() {
  mixEntropy(micros());
  randomSeed(entropyPool ^ millis());
}

void updateEntropyDecay() {
  unsigned long now = millis();
  if (now - lastEntropyDecay >= ENTROPY_DECAY_MS) {
    lastEntropyDecay = now;
    if (entropyLevel > 0) {
      entropyLevel--;
    }
  }
}

// ======================================================
// SETUP
// ======================================================
void setup() {
  pinMode(TOGGLE_PIN, INPUT_PULLUP);
  pinMode(ORANGE_LED, OUTPUT);

  for (uint8_t i = 0; i < 5; i++) {
    pinMode(PINK_LEDS[i], OUTPUT);
  }

  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  u8g2.begin();
  // Start OLED off until toggle is flipped
  u8g2.setPowerSave(1);

  digitalWrite(ORANGE_LED, LOW);
  setAllPink(0);

  // Initial seeding: multiple noisy reads from A0 + timers
  stirFromAnalog(8);
  mixEntropy(millis());
  reseedFromEntropy();
}

// ======================================================
// MAIN LOOP
// ======================================================
void loop() {
  bool toggleState = digitalRead(TOGGLE_PIN);

  // Edge detect on toggle
  if (toggleState != lastToggleState) {
    lastToggleState = toggleState;

    if (toggleState == LOW) {
      // Toggle ON
      digitalWrite(ORANGE_LED, HIGH);
      u8g2.setPowerSave(0);
      showMode(MODE_STANDBY);
      lastStandbyRefresh = millis();
    } else {
      // Toggle OFF
      digitalWrite(ORANGE_LED, LOW);
      setAllPink(0);
      u8g2.setPowerSave(1);
      streamRunning = false;
    }
  }

  // If toggle is OFF, bail out
  if (toggleState == HIGH) {
    return;
  }

  updateBreathing();
  updateEntropyDecay();
  handleButtons();

  // Continuous update for stream mode
  if (currentMode == MODE_STREAM) {
    updateStreamContent();
  }

  // Periodically refresh standby screen to animate glitch window
  if (currentMode == MODE_STANDBY) {
    unsigned long now = millis();
    if (now - lastStandbyRefresh >= STANDBY_GLITCH_INTERVAL_MS) {
      lastStandbyRefresh = now;
      drawStandbyScreen();
    }
  }
}

// ======================================================
// BREATHING LED LOGIC
// ======================================================
void updateBreathing() {
  unsigned long now = millis();
  if (now - lastBreathUpdate >= BREATH_INTERVAL_MS) {
    lastBreathUpdate = now;

    breathValue += breathDirection * 2;
    if (breathValue >= 255) {
      breathValue = 255;
      breathDirection = -1;
    } else if (breathValue <= 0) {
      breathValue = 0;
      breathDirection = 1;
    }

    setAllPink((uint8_t)breathValue);
  }
}

void setAllPink(uint8_t value) {
  for (uint8_t i = 0; i < 5; i++) {
    analogWrite(PINK_LEDS[i], value);
  }
}

// ======================================================
// BUTTON HANDLING
// ======================================================
void handleButtons() {
  unsigned long now = millis();
  if (now - lastButtonTime < DEBOUNCE_MS) {
    return;
  }

  auto processButton = [&](ButtonState &b) {
    bool state = digitalRead(b.pin); // HIGH = not pressed, LOW = pressed

    if (state != b.lastState) {
      b.lastState = state;

      if (state == LOW) {
        // pressed
        b.pressedAt = now;
      } else {
        // released
        unsigned long held = now - b.pressedAt;
        lastButtonTime = now;

        // Every release: mix timing entropy + bump level
        mixEntropy(now);
        mixEntropy(held);
        if (entropyLevel < 250) entropyLevel += 5;

        if (held >= LONG_PRESS_MS) {
          handleLongPress(b.pin);
        } else {
          handleShortPress(b.pin);
        }
      }
    }
  };

  processButton(btn1);
  processButton(btn2);
  processButton(btn3);
}

void handleShortPress(uint8_t pin) {
  if (currentMode == MODE_STANDBY) {
    // Mode selection from home screen
    if (pin == BTN1_PIN) {
      showMode(MODE_STREAM);
    } else if (pin == BTN2_PIN) {
      showMode(MODE_PASSWORD);
    } else if (pin == BTN3_PIN) {
      showMode(MODE_VISUAL);
    }
    return;
  }

  // Inside modes
  if (currentMode == MODE_STREAM) {
    if (pin == BTN1_PIN) {
      // Toggle stream on/off
      streamRunning = !streamRunning;
      if (streamRunning) {
        // add fresh analog noise before streaming
        stirFromAnalog(4);
        reseedFromEntropy();
      }
      drawStreamScreen();
    }
  } else if (currentMode == MODE_PASSWORD) {
    if (pin == BTN2_PIN) {
      // add fresh analog noise before new password
      stirFromAnalog(4);
      reseedFromEntropy();
      generatePassword();
      drawPasswordScreen();
    }
  } else if (currentMode == MODE_VISUAL) {
    if (pin == BTN3_PIN) {
      // fresh analog noise for each new pattern
      stirFromAnalog(4);
      reseedFromEntropy();
      drawVisualScreen(); // new pattern each press
    }
  }
}

void handleLongPress(uint8_t /*pin*/) {
  // Long press on any button → back to standby, stop stream
  streamRunning = false;
  showMode(MODE_STANDBY);
  lastStandbyRefresh = millis();
}

// ======================================================
// MODE SWITCH / DRAW HELPERS
// ======================================================
void showMode(Mode m) {
  currentMode = m;

  switch (currentMode) {
    case MODE_STANDBY:
      drawStandbyScreen();
      lastStandbyRefresh = millis();
      break;

    case MODE_STREAM:
      initStreamBuffer();
      streamRunning = false; // wait for BTN1 to start
      drawStreamScreen();
      break;

    case MODE_PASSWORD:
      drawPasswordScreen();
      break;

    case MODE_VISUAL:
      drawVisualScreen();
      break;
  }
}

// ======================================================
// STREAM MODE HELPERS
// ======================================================
void initStreamBuffer() {
  for (uint8_t r = 0; r < STREAM_ROWS; r++) {
    for (uint8_t c = 0; c < STREAM_COLS; c++) {
      streamBuf[r][c] = ' ';
    }
  }
  streamIndex = 0;
  lastStreamStep = millis();
}

char randomChar() {
  const char charset[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!@#$%^&*";
  const uint8_t n = sizeof(charset) - 1;
  return charset[random(n)];
}

void updateStreamContent() {
  if (!streamRunning) return;

  unsigned long now = millis();
  if (now - lastStreamStep < STREAM_INTERVAL_MS) return;
  lastStreamStep = now;

  uint16_t cell = streamIndex;
  uint8_t row = cell / STREAM_COLS;
  uint8_t col = cell % STREAM_COLS;

  streamBuf[row][col] = randomChar();

  streamIndex++;
  if (streamIndex >= STREAM_ROWS * STREAM_COLS) {
    streamIndex = 0;
  }

  drawStreamScreen();
}

// ======================================================
// PASSWORD MODE HELPER
// ======================================================
void generatePassword() {
  const char charset[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!@#$%^&*";

  const uint8_t n = sizeof(charset) - 1;

  if (pwdCount < MAX_PASSWORDS) {
    pwdCount++;
  }

  // Shift older passwords up
  for (int i = 0; i < (int)pwdCount - 1; i++) {
    strcpy(passwords[i], passwords[i + 1]);
  }

  // New password in the last slot
  char *slot = passwords[pwdCount - 1];
  for (uint8_t j = 0; j < PASSWORD_LEN; j++) {
    slot[j] = charset[random(n)];
  }
  slot[PASSWORD_LEN] = '\0';
}

// ======================================================
// GLITCH WINDOW (STANDBY ONLY) – GRID OF SQUARE FILLS
// ======================================================
void drawGlitchWindow() {
  // Smaller grid-of-blocks glitch window on the right side
  const int gridCols = 4;
  const int gridRows = 4;
  const int cellSize = 6;          // 6x6 pixels per cell

  const int gridW = gridCols * cellSize;  // 24
  const int gridH = gridRows * cellSize;  // 24

  // Position it nicely on the right
  const int x0 = 94;  // left of the grid
  const int y0 = 30;  // top of the grid

  // Clear just this region first
  u8g2.setDrawColor(0);
  u8g2.drawBox(x0, y0, gridW, gridH);
  u8g2.setDrawColor(1);

  // For each cell, choose a "fill level" and draw a square-ish pattern
  for (int r = 0; r < gridRows; r++) {
    for (int c = 0; c < gridCols; c++) {
      int x = x0 + c * cellSize;
      int y = y0 + r * cellSize;

      // Bias toward empties so it doesn't look like a solid blob
      uint8_t style = random(6);  // 0..5

      switch (style) {
        case 0:
        case 1:
          // Empty cell
          break;

        case 2:
          // Tiny center pixel (very light)
          u8g2.drawPixel(x + cellSize / 2, y + cellSize / 2);
          break;

        case 3:
          // Small inner block (medium)
          u8g2.drawBox(x + 2, y + 2, cellSize - 4, cellSize - 4);
          break;

        case 4:
        case 5:
        default:
          // Fully filled block (heavy)
          u8g2.drawBox(x, y, cellSize, cellSize);
          break;
      }
    }
  }
}

// ======================================================
// OLED SCREENS – PAGE RENDERING
// ======================================================

// ---------- Standby / home screen ----------
void drawStandbyPage() {
  u8g2.drawFrame(0, 0, 128, 64);

  u8g2.setFont(u8g2_font_seraphimb1_tr);
  u8g2.drawStr(4, 18, "3NTR0PY B0X");

  u8g2.drawHLine(4, 22, 120);

  u8g2.setFont(u8g2_font_profont12_tr);
  u8g2.drawStr(6, 38, "B1: Stream");
  u8g2.drawStr(6, 50, "B2: Password");
  u8g2.drawStr(6, 62, "B3: Visual");

  // glitch window on the right
  drawGlitchWindow();
}

void drawStandbyScreen() {
  u8g2.firstPage();
  do {
    drawStandbyPage();
  } while (u8g2.nextPage());
}

// ---------- Mode 1: Stream ----------
void drawStreamPage() {
  u8g2.drawFrame(0, 0, 128, 64);

  u8g2.setFont(u8g2_font_seraphimb1_tr);
  u8g2.drawStr(4, 18, "M0DE 1 STR3AM");
  u8g2.drawHLine(4, 22, 120);

  u8g2.setFont(u8g2_font_profont12_tr);

  if (streamRunning) {
    u8g2.drawStr(6, 34, "[RUNNING]");
  } else {
    u8g2.drawStr(6, 34, "[IDLE] B1=start/stop");
  }

  const uint8_t baseY = 44;  // 44, 52, 60
  for (uint8_t r = 0; r < STREAM_ROWS; r++) {
    char line[STREAM_COLS + 1];
    for (uint8_t c = 0; c < STREAM_COLS; c++) {
      line[c] = streamBuf[r][c];
    }
    line[STREAM_COLS] = '\0';
    uint8_t y = baseY + r * 8;
    u8g2.drawStr(6, y, line);
  }
}

void drawStreamScreen() {
  u8g2.firstPage();
  do {
    drawStreamPage();
  } while (u8g2.nextPage());
}

// ---------- Mode 2: Password ----------
void drawPasswordPage() {
  u8g2.drawFrame(0, 0, 128, 64);

  u8g2.setFont(u8g2_font_seraphimb1_tr);
  u8g2.drawStr(4, 18, "M0DE 2 PASSW0RD");
  u8g2.drawHLine(4, 22, 120);

  u8g2.setFont(u8g2_font_profont12_tr);

  if (pwdCount == 0) {
    u8g2.drawStr(6, 38, "Press B2 for pwd");
  } else {
    for (uint8_t i = 0; i < pwdCount; i++) {
      uint8_t y = 38 + i * 12; // 38, 50, 62
      u8g2.drawStr(6, y, passwords[i]);
    }
  }
}

void drawPasswordScreen() {
  u8g2.firstPage();
  do {
    drawPasswordPage();
  } while (u8g2.nextPage());
}

// ---------- Mode 3: Visual ----------
void drawVisualPage() {
  u8g2.drawFrame(0, 0, 128, 64);

  u8g2.setFont(u8g2_font_seraphimb1_tr);
  u8g2.drawStr(4, 18, "M0DE 3 V1SUAL");
  u8g2.drawHLine(4, 22, 120);

  const int x0 = 6;
  const int y0 = 26;
  const int x1 = 122;
  const int y1 = 60;

  // Layer 1: starfield / static
  for (int i = 0; i < 50; i++) {
    int x = random(x0, x1);
    int y = random(y0, y1);
    u8g2.drawPixel(x, y);
  }

  // Layer 2: random boxes
  for (int i = 0; i < 8; i++) {
    int w = random(4, 12);
    int h = random(4, 12);
    int x = random(x0, x1 - w);
    int y = random(y0, y1 - h);
    if (random(2)) {
      u8g2.drawFrame(x, y, w, h);
    } else {
      u8g2.drawBox(x, y, w, h);
    }
  }

  // Layer 3: diagonal glitch lines
  for (int i = 0; i < 4; i++) {
    int x = random(x0, x1);
    int y = random(y0, y1);
    int len = random(8, 20);
    if (random(2)) {
      u8g2.drawLine(x, y, x + len, y + len);
    } else {
      u8g2.drawLine(x, y, x - len, y + len);
    }
  }
}

void drawVisualScreen() {
  u8g2.firstPage();
  do {
    drawVisualPage();
  } while (u8g2.nextPage());
}
