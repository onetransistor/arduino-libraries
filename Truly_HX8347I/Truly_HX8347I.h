/*******************************************************************************

  (Arduino) Library for HX8347-I SPI 320x240 LCD from
  the front panel of Sercomm SHG1500 router (LCDv6 only)

  Version 1.0

  Truly_HX8347I is a subclass of Adafruit_GFX and requires this library.

  Based on the following open source libraries:

    - Sercomm/Truly Led driver (LCD init registers)
      File: rg/vendor/sercomm/truly_lcd/lcd_hw_hx8347i.c
      Copyright (C) Jungo LTD 2004
      License: GPLv2
      Found at: https://openwrt.org/toh/sercomm/shg1500

    - MCUFRIEND_kbv (Color definitions)
      File: MCUFRIEND_kbv.h
      Copyright (C) David Prentice 2018
      License: FreeBSD
      URL: https://github.com/prenticedavid/MCUFRIEND_kbv

    - Adafruit_GFX (Truly_HX8347I is a subclass of it)
      Copyright (c) 2012 Adafruit Industries.  All rights reserved.
      License: BSD
      URL: https://github.com/adafruit/Adafruit-GFX-Library

  Copyright (C) 2019 One Transistor <https://www.onetransistor.eu>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

*******************************************************************************/

#ifndef Truly_HX8347I_H
#define Truly_HX8347I_H

#include "Adafruit_GFX.h"
#include "SPI.h"

#define HX8347I_CMD  0x70
#define HX8347I_DATA 0x72

class Truly_HX8347I : public Adafruit_GFX {

public:
	Truly_HX8347I(int CS, int RST, int BL = -1);
	void begin(void);

	// specific backlight control
	void backlightReset() { resetBacklight(); }
	void backlightOn(boolean on = true); // you may want to fill screen before turning it on
	void backlightIntensity(uint8_t intensity, boolean reset = false); // turns it on

	virtual void drawPixel(int16_t x, int16_t y, uint16_t color); // mandatory

	// CONTROL API
	virtual void setRotation(uint8_t r);
	virtual void invertDisplay(boolean i);

	// BASIC DRAW API
	virtual void fillScreen(uint16_t color);
	virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
	virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

	void setWindow(int16_t x, int16_t y, int16_t x1, int16_t y1);

	uint16_t color565(uint8_t r, uint8_t g, uint8_t b) { return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3); }

private:
	int _CS, _RST, _BL;

	void reset(void);

	void resetBacklight(void);

	void writeRegister(uint8_t reg, uint8_t val);
	void initDisplay(void);
	void setCurrentPixelColor(uint16_t color);

	void sendBLbit(boolean b);
	void sendBLbyte(uint8_t by);
	
	void setCSPinStatus(int bit);
	void setRSTPinStatus(int bit);
	void setBLPinStatus(int bit);
	void initGPIO(void);
	void initSPIBus(void);
	void sendSPIPacket(uint8_t data, uint8_t type);
};

// color definitions from MCUFRIEND_kbv.h (by David Prentice)
// New color definitions.  thanks to Bodmer
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define TFT_PINK        0xFC9F


#endif
