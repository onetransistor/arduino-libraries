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

#include "Truly_HX8347I.h"

Truly_HX8347I::Truly_HX8347I(int CS, int RST, int BL):Adafruit_GFX(240, 320) {
	_CS = CS;
	_RST = RST;
	_BL = BL;
}

void Truly_HX8347I::reset(void) {
	setRSTPinStatus(HIGH);
	delay(20);
	setRSTPinStatus(LOW);
	delay(50);
	setRSTPinStatus(HIGH);
	delay(20);
}

void Truly_HX8347I::begin(void) {
	// configure pins
	initGPIO();
	setBLPinStatus(LOW); // disable backlight
	setCSPinStatus(HIGH);

	// configure SPI
	initSPIBus();

	// reset LCD
	reset();

	// start LCD
	initDisplay();
}

void Truly_HX8347I::setWindow(int16_t x, int16_t y, int16_t x1, int16_t y1) {
	writeRegister(0x02, x >> 8); // Column address start 2
	writeRegister(0x03, x & 0xFF); // Column address start 1
	writeRegister(0x04, x1 >> 8); // Column address end 2
	writeRegister(0x05, x1 & 0xFF); // Column address end 1
	writeRegister(0x06, y >> 8); // Row address start 2
	writeRegister(0x07, y & 0xFF); // Row address start 1
	writeRegister(0x08, y1 >> 8); // Row address end 2
	writeRegister(0x09, y1 & 0xFF); // Row address end 1

	sendSPIPacket(0x22, HX8347I_CMD);
}

void Truly_HX8347I::drawPixel(int16_t x, int16_t y, uint16_t color) {
	if (x < 0 || y < 0 || x >= width() || y >= height())
        	return;

	setWindow(x, y, x, y);
	setCurrentPixelColor(color);
}

/***** Basic draw API *****/

void Truly_HX8347I::fillScreen(uint16_t color) {
	setWindow(0, 0, width() - 1, height() - 1);
	for (int i = 0; i < width() * height(); i++) {
		setCurrentPixelColor(color);
	}
}

void Truly_HX8347I::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	setWindow(x, y, x + w - 1, y + h - 1);
	for (int i = 0; i < w * h; i++) {
		setCurrentPixelColor(color);
	}
}

void Truly_HX8347I::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
	fillRect(x, y, 1, h, color);
}

void Truly_HX8347I::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
	fillRect(x, y, w, 1, color);
}

/***** Control API *****/

void Truly_HX8347I::setRotation(uint8_t r) {
	rotation = (r & 3);
    	switch(rotation) {
        case 0:
		_width  = WIDTH;
		_height = HEIGHT;
		writeRegister(0x16, 0x10); // portrait, keypad down
		break;
        case 2:
		_width  = WIDTH;
		_height = HEIGHT;
		writeRegister(0x16, 0xC0); // portrait, keypad up
		break;
        case 1:
		_width  = HEIGHT;
		_height = WIDTH;
		writeRegister(0x16, 0x60); // landscape, keypad right
		break;
        case 3:
		_width  = HEIGHT;
		_height = WIDTH;
		writeRegister(0x16, 0xB0); // landscape, keypad left
		break;
    	}
}

void Truly_HX8347I::invertDisplay(boolean i) {
	i == true ? writeRegister(0x01, 0x02) : writeRegister(0x01, 0x00);
}

/***** LCD controller init and basic setting *****/

void Truly_HX8347I::writeRegister(uint8_t reg, uint8_t val) {
	sendSPIPacket(reg, HX8347I_CMD);
	sendSPIPacket(val, HX8347I_DATA);
}

void Truly_HX8347I::initDisplay(void) {
	/* TE signal ON */
	writeRegister(0x2D, 0x1D); // Cycle control 1 (GDON)
	writeRegister(0x2E, 0x83); // Cycle control 2 (GDOF)
	writeRegister(0xE4, 0x02); // Power saving 1 (EQVCI_M1)

	/* Power Voltage Setting */
	writeRegister(0xE5, 0x26); // Power saving 2 (EQGND_M1)
	writeRegister(0xE6, 0x26); // Power saving 3 (EQVCI_M0)
	writeRegister(0xE7, 0x02); // Power saving 4 (EQGND_M0)
	writeRegister(0xE8, 0x6E); // Source OP control_Normal (OPON_N)
	writeRegister(0xE9, 0x46); // Source OP control_IDLE (OPON_I)

	/* VCOM offset */
	writeRegister(0xEA, 0x00); // Power control internal use (1) (STBA)
	writeRegister(0xEB, 0x24); // Power control internal use (2) (STBA)
	writeRegister(0xEC, 0x00); // Source control internal use (1) (PTBA)

	delay(5);

	writeRegister(0xED, 0xc4); // Source control internal use (2) (PTBA)
	writeRegister(0xF3, 0x00); // Power saving 5 (SEQVCI_M1)

	delay(5);

	/* Display ON Setting */
	writeRegister(0xF4, 0x00); // Power saving 6 (SEQGND_M0)

	delay(40);

	writeRegister(0x1B, 0x1E); // Power control 2 (VRH[5:0])
	writeRegister(0x1A, 0x04); // Power control 1 (BT[2:0])
	writeRegister(0x24, 0x7F); // VCOM Control 2 (VMH)
	writeRegister(0x25, 0x17); // VCOM Control 3 (VML)
	writeRegister(0x23, 0x75); // VCOM Control 1 (VMF)
	//writeRegister(0xE2, 0x04); // no such register for HX8347-I
	writeRegister(0x19, 0x01); // OSC Control 1 (OSC_EN[0])
	writeRegister(0x1C, 0x03); // Power control 3 (AP[2:0])
	writeRegister(0x01, 0x00); // Display mode control

	/* Power Voltage Setting */
	writeRegister(0x1F, 0x88); // Power control 6

	delay(5);

	writeRegister(0x1F, 0x80);  // Power control 6

	delay(5);

	writeRegister(0x1F, 0x90); // Power control 6

	delay(5);

	writeRegister(0x1F, 0xD4);  // Power control 6

	delay(5);

	writeRegister(0x18, 0x66); // OSC Control 2
	writeRegister(0x1A, 0x02); // Power control 1
	writeRegister(0x17, 0x05); // COLMOD
	writeRegister(0x36, 0x09); // Panel characteristic
	writeRegister(0x28, 0x38); // Display control 3

	delay(40);

	writeRegister(0x28, 0x3C); // Display control 3

	delay(40);

	/* Gamma 2.2 Setting */
	writeRegister(0x40, 0x00);
	writeRegister(0x41, 0x02);
	writeRegister(0x42, 0x06);
	writeRegister(0x43, 0x32);
	writeRegister(0x44, 0x33);
	writeRegister(0x45, 0x3B);
	writeRegister(0x46, 0x26);
	writeRegister(0x47, 0x7D);
	writeRegister(0x48, 0x01);
	writeRegister(0x49, 0x15);
	writeRegister(0x4A, 0x1A);
	writeRegister(0x4B, 0x1A);
	writeRegister(0x4C, 0x14);
	writeRegister(0x50, 0x04);
	writeRegister(0x51, 0x0C);
	writeRegister(0x52, 0x0D);
	writeRegister(0x53, 0x39);
	writeRegister(0x54, 0x3D);
	writeRegister(0x55, 0x3F);
	writeRegister(0x56, 0x02);
	writeRegister(0x57, 0x59);
	writeRegister(0x58, 0x0B);
	writeRegister(0x59, 0x05);
	writeRegister(0x5A, 0x05);
	writeRegister(0x5B, 0x0A);
	writeRegister(0x5C, 0x1E);
	writeRegister(0x5D, 0x88);

	/*    PAGE 2  */
	writeRegister(0xFF, 0x02); // Page select
	writeRegister(0x0C, 0x40); // Partial area end row 2

	/*    PAGE 0  */
	writeRegister(0xFF, 0x00); // Page select

	delay(100);

	writeRegister(0x16, 0x10); // Memory access control, rotation here
}

void Truly_HX8347I::setCurrentPixelColor(uint16_t color) {
	setCSPinStatus(LOW);

	// Arduino
	SPI.transfer(HX8347I_DATA);
	SPI.transfer(color >> 8);
	SPI.transfer(color & 0xFF);

	setCSPinStatus(HIGH);
}

/***** backlight control API *****/

void Truly_HX8347I::resetBacklight(void) {
	if (_BL == -1) return;

	setBLPinStatus(LOW);
	delay(1);
	setBLPinStatus(HIGH);
	delayMicroseconds(200);
	setBLPinStatus(LOW);
	delayMicroseconds(400);
	setBLPinStatus(HIGH);
}

void Truly_HX8347I::sendBLbit(boolean b) {
	setBLPinStatus(LOW);
	delayMicroseconds(b == true ? 100 : 200);
	setBLPinStatus(HIGH);
	delayMicroseconds(b == true ? 200 : 100);
}

void Truly_HX8347I::sendBLbyte(uint8_t by) {
	setBLPinStatus(HIGH);
	delayMicroseconds(50);

	for (int i = 7; i >= 0; i--)
		sendBLbit((by >> i) & 0x1);

	setBLPinStatus(LOW);
	delayMicroseconds(50);
	setBLPinStatus(HIGH);
}

void Truly_HX8347I::backlightOn(boolean on) {
	setBLPinStatus(on == true ? HIGH : LOW);
}

void Truly_HX8347I::backlightIntensity(uint8_t intensity, boolean reset) {
	if (_BL == -1) return;

	if (reset == true) resetBacklight();

	uint8_t cmd[2];
	cmd[0] = 0x72;
	cmd[1] = 0x00 | (intensity & 0x1F);

	sendBLbyte(cmd[0]);
	sendBLbyte(cmd[1]);
}

/***** platform specific API (pin manipulation and SPI port config) *****/

void Truly_HX8347I::initGPIO(void) {
	// Arduino
	pinMode(_CS, OUTPUT);
	pinMode(_RST, OUTPUT);
	if (_BL != -1)
		pinMode(_BL, OUTPUT);
}

void Truly_HX8347I::setRSTPinStatus(int bit) {
	// Arduino
	digitalWrite(_RST, bit);
}

void Truly_HX8347I::setCSPinStatus(int bit) {
	// Arduino
	digitalWrite(_CS, bit);
}

void Truly_HX8347I::setBLPinStatus(int bit) {
	if (_BL == -1) return; // undefined backlight control

	// Arduino
	digitalWrite(_BL, bit);
}

void Truly_HX8347I::initSPIBus(void) {
	SPI.begin();

	// STM32
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE3);
	SPI.setClockDivider(SPI_CLOCK_DIV2);

	// Arduino
	//SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE3));
}

void Truly_HX8347I::sendSPIPacket(uint8_t data, uint8_t type) {
	setCSPinStatus(LOW);

	// Arduino
	SPI.transfer(type);
	SPI.transfer(data);

	setCSPinStatus(HIGH);
}


