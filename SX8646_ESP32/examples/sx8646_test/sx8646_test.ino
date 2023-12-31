#include "SX8646_ESP32.h"

SX8646 keypad(5); // this is interrupt pin, adjust for your board
uint8_t lastKey = PANEL_NONE;

void keyTouched(uint8_t key) {
  if (key != PANEL_NONE) {
    if (key == PANEL_OK) Serial.println("Touched: OK.");
    if (key == PANEL_UP) Serial.println("Touched: UP.");
    if (key == PANEL_DOWN) Serial.println("Touched: DOWN.");
    if (key == PANEL_LEFT) Serial.println("Touched: LEFT.");
    if (key == PANEL_RIGHT) Serial.println("Touched: RIGHT.");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nSX8646 Keypad test.\n");

  keypad.setKeyCallback(keyTouched);
  keypad.begin();

}

void loop() {
  keypad.handleInterruptInClass();
  delay(30); // give it time to read data from SX8646 and clear interupt state
}
