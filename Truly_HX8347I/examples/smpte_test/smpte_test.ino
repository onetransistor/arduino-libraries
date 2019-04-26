
#include "Adafruit_GFX.h"
#include "Truly_HX8347I.h"

#define SPI_CS  PA4 // SPI CS pin
#define LCD_RST PA2 // LCD reset pin (mandatory here)
#define BKL_CTL PA3 // backlight control pin (optional)

Truly_HX8347I tft(SPI_CS, LCD_RST, BKL_CTL); 

void setup() {
  tft.begin();
  tft.setRotation(1); // 90 degrees, landscape

  tft.fillScreen(tft.color565(0x13, 0x13, 0x13));
  tft.backlightOn();

  // SMPTE Color Bars (https://en.wikipedia.org/wiki/SMPTE_color_bars)
  tft.fillRect(  0, 0, 44, 160, tft.color565(0xc0, 0xc0, 0xc0));
  tft.fillRect( 44, 0, 46, 160, tft.color565(0xc0, 0xc0, 0x00));
  tft.fillRect( 90, 0, 46, 160, tft.color565(0x00, 0xc0, 0xc0));
  tft.fillRect(136, 0, 46, 160, tft.color565(0x00, 0xc0, 0x00));
  tft.fillRect(182, 0, 46, 160, tft.color565(0xc0, 0x00, 0xc0));
  tft.fillRect(228, 0, 46, 160, tft.color565(0xc0, 0x00, 0x00));
  tft.fillRect(274, 0, 46, 160, tft.color565(0x00, 0x00, 0xc0));

  tft.fillRect(  0, 160, 44, 20, tft.color565(0x00, 0x00, 0xc0));
  tft.fillRect( 90, 160, 46, 20, tft.color565(0xc0, 0x00, 0xc0));
  tft.fillRect(182, 160, 46, 20, tft.color565(0x00, 0xc0, 0xc0));
  tft.fillRect(274, 160, 46, 20, tft.color565(0xc0, 0xc0, 0xc0));
  
  tft.fillRect(  0, 180, 57, 60, tft.color565(0x00, 0x21, 0x4c));
  tft.fillRect( 57, 180, 57, 60, 0xffff);
  tft.fillRect(114, 180, 57, 60, tft.color565(0x32, 0x00, 0x6a));

  tft.fillRect(228, 180, 16, 60, tft.color565(0x09, 0x09, 0x09));
  tft.fillRect(258, 180, 16, 60, tft.color565(0x1d, 0x1d, 0x1d));

  // text box
  tft.fillRect(80, 80, 160, 40, TFT_BLACK);
  tft.setCursor(94, 90);
  tft.print("SMPTE Test on HX8347-I");
  tft.setCursor(100, 100);
  tft.print("www.onetransistor.eu");
}

void loop() {
  
}
