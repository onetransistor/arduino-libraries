#include "SX8646.h"

SX8646 keypad(D6); // this is interrupt pin, adjust for your board
uint8_t lastKey = PANEL_NONE;

void keyTouched(const uint8_t key) {
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

  keypad.begin();
  keypad.setKeyCallback(keyTouched);

}

void loop() {
}
