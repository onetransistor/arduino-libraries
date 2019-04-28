/* Sercomm SHG1500 LCDv6 front panel test 
 *  
 *  https://www.onetransistor.eu/2018/10/router-lcd-and-keypad-to-arduino.html
 *  https://www.onetransistor.eu/2019/04/arduino-library-truly-lcd-hx8347i.html
 *  https://www.onetransistor.eu/2019/04/arduino-library-semtech-sx8646.html
 */

#include "Adafruit_GFX.h"
#include "Truly_HX8347I.h"
#include "SX8646.h"

// adjust pins (these are for STM32)
#define SPI_CS  PA4 // SPI CS pin
#define LCD_RST PA2 // LCD reset pin (mandatory here)
#define BKL_CTL PA3 // backlight control pin (optional)

#define KEY_INTB  PA8 // keypad interrupt pin

Truly_HX8347I tft(SPI_CS, LCD_RST, BKL_CTL);
SX8646 keypad(KEY_INTB);

Adafruit_GFX_Button btnOK, btnLEFT, btnRIGHT, btnUP, btnDOWN;
uint8_t lastKey = PANEL_NONE;

void keyTouched(const uint8_t key) {
  if (key != PANEL_NONE) {
    if (key == PANEL_OK) btnOK.drawButton(true);
    if (key == PANEL_UP) btnUP.drawButton(true);
    if (key == PANEL_DOWN) btnDOWN.drawButton(true);
    if (key == PANEL_LEFT) btnLEFT.drawButton(true);
    if (key == PANEL_RIGHT) btnRIGHT.drawButton(true);
  }

  switch (lastKey) {
    case PANEL_OK: btnOK.drawButton(); break;
    case PANEL_UP: btnUP.drawButton(); break;
    case PANEL_DOWN: btnDOWN.drawButton(); break;
    case PANEL_LEFT: btnLEFT.drawButton(); break;
    case PANEL_RIGHT: btnRIGHT.drawButton(); break;
    case PANEL_NONE:
    default: break;
  }

  lastKey = key; // use this to avoid redrawing all buttons when key no longer touched
}

void setup() {
  keypad.begin();
  
  tft.begin();
  tft.setRotation(1); // 90 degrees, landscape
  tft.fillScreen(TFT_BLACK);
  tft.backlightOn();

  delay(100);

  btnOK.initButtonUL(&tft, 130, 90, 60, 60, TFT_BLUE, TFT_CYAN, TFT_BLACK, "OK", 2);
  btnOK.drawButton();
  btnLEFT.initButtonUL(&tft, 40, 90, 80, 60, TFT_BLUE, TFT_CYAN, TFT_BLACK, "LEFT", 2);
  btnLEFT.drawButton();
  btnRIGHT.initButtonUL(&tft, 200, 90, 80, 60, TFT_BLUE, TFT_CYAN, TFT_BLACK, "RIGHT", 2);
  btnRIGHT.drawButton();
  btnUP.initButtonUL(&tft, 130, 20, 60, 60, TFT_BLUE, TFT_CYAN, TFT_BLACK, "UP", 2);
  btnUP.drawButton();
  btnDOWN.initButtonUL(&tft, 130, 160, 60, 60, TFT_BLUE, TFT_CYAN, TFT_BLACK, "DOWN", 2);
  btnDOWN.drawButton();

  delay(100);

  keypad.setKeyCallback(keyTouched);
}

void loop() {

}
