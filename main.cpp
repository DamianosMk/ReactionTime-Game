#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

hd44780_I2Cexp lcd;

// Pin definitions
const int redLEDs[] = {2, 3, 4, 5}; // left to right
const int buttonPin = 13;
const int numLEDs = 4;

bool lightsOut = false;
unsigned long startTime;
unsigned long reactionTime;

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("F1 Reaction Game");

  for (int i = 0; i < numLEDs; i++) {
    pinMode(redLEDs[i], OUTPUT);
    digitalWrite(redLEDs[i], LOW);
  }

  pinMode(buttonPin, INPUT_PULLUP);
  delay(2000);
  lcd.clear();
}

void loop() {
  lightsOut = false;
  lcd.setCursor(0, 0);
  lcd.print("Get Ready...");
  delay(1000);

  // Ensure all LEDs start OFF
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(redLEDs[i], LOW);
  }

  // Turn on LEDs one by one (left to right)
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(redLEDs[i], HIGH);
    delay(1000);
  }

  // Random delay before lights go out
  int randomDelay = random(1000, 3000);
  unsigned long preStart = millis();

  // Check for false start
  while (millis() - preStart < randomDelay) {
    if (digitalRead(buttonPin) == LOW) {
      showFalseStart();
      waitForRestart();
      return;
    }
  }

  // All LEDs OFF = lights out!
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(redLEDs[i], LOW);
  }

  lightsOut = true;
  startTime = millis();
  lcd.clear();
  lcd.print("GO!");

  // Wait for valid button press
  while (true) {
    if (digitalRead(buttonPin) == LOW && lightsOut) {
      reactionTime = millis() - startTime;
      showReactionTime();
      waitForRestart();
      return;
    }
  }
}

void showReactionTime() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Reaction Time:");
  float seconds = reactionTime / 1000.0;
  lcd.setCursor(0, 1);
  lcd.print(seconds, 3); // shows 0.382
  lcd.print(" s");
}

void showFalseStart() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("False Start!");
  lcd.setCursor(0, 1);
  lcd.print("Try Again...");
}

void waitForRestart() {
  delay(2000); // give time to view result
  lcd.setCursor(0, 1);
  lcd.print("Press to restart");

  while (digitalRead(buttonPin) == LOW); // wait for release
  delay(100);
  while (digitalRead(buttonPin) == HIGH); // wait for new press
  delay(200);
  lcd.clear();
}
