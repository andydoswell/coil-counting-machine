/*
  Turn Counter
  SparkFun Pro Micro + 16x2 I2C LCD

  Compiled in Arduino IDE 2.3.6

  A.G.Doswell Sept 2025
  --------------------------------------
  Version: 1.1 (Baseline)
  Features:
   - Two sensors (pins 0 & 1)
   - Counts forward when sensor A triggers
   - Requires sensor B before next count (prevents bounce)
   - Reset button (active LOW, pin 4) clears counter
   - LCD: 16x2, I2C address 0x3F
   - Simple splash screen at startup
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// I2C LCD, address 0x3F, 16 chars x 2 lines
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// Pin assignments
const int sensorA = 0;     // First sensor
const int sensorB = 1;     // Second sensor
const int resetPin = 4;    // Active LOW reset button

// Counter
volatile long count = 0;
bool waitingForB = false;

void setup() {
  pinMode(sensorA, INPUT_PULLUP);
  pinMode(sensorB, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Turn Counter v1.1");
  delay(1000);
  displayCount();
}

void loop() {
  // Reset button check
  if (digitalRead(resetPin) == LOW) {
    count = 0;
    displayCount();
  }

  // Sensor A triggers the increment
  if (digitalRead(sensorA) == LOW && !waitingForB) {
    count++;
    displayCount();
    waitingForB = true;  // Wait for sensor B to allow next increment
  }

  // Sensor B clears the "waiting" state
  if (digitalRead(sensorB) == LOW && waitingForB) {
    waitingForB = false;
  }
}

void displayCount() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Count:");
  lcd.setCursor(0, 1);
  lcd.print(count);
}
