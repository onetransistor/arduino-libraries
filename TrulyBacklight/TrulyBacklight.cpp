/*
    Library to control backlight of Truly LCD from the front panel of some
    Sercomm SHG1500 routers. Only hardware versions LCDv6 have been tested
    for this type of controller. By the way, the backlight controller IC
    has "T43" printed on it.

    Copyright (C) 2018 One Transistor <one1transistor@gmail.com>
    Website: https://www.onetransistor.eu

    Code has been developed by sampling the backlight control signal with
    a logic analyzer. Code is also based on GPLv2 "Sercomm/Truly Led driver".
    File "rg/vendor/sercomm/truly_lcd/lcd_hw_hx8347i.c" contains the functions
    that control backlight. It was developed by Jungo LTD.

    Copyright - 2010 SerComm Corporation
    Copyright (C) Jungo LTD 2004

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
*/

#include "TrulyBacklight.h"

void TrulyBacklight::setControlPin(int pin) {
	if (pin) pin_t = pin;
	pinMode(pin_t, OUTPUT);
}

void TrulyBacklight::reset() {
	if (!pin_t) return;

	digitalWrite(pin_t, LOW);
	delay(1); // source code says 20 ms; logic sampling says 1 ms
	digitalWrite(pin_t, HIGH);
	delayMicroseconds(200);
	digitalWrite(pin_t, LOW);
	delayMicroseconds(400);
	digitalWrite(pin_t, HIGH);
}

void TrulyBacklight::setOnOff(unsigned char on) {
	if (!pin_t) return;

	if (on > 1) on = HIGH;
	digitalWrite(pin_t, on);
}

void TrulyBacklight::setIntensity(unsigned char intensity) {
	if (!pin_t) return;

	if (intensity < BKLIGHT_MIN_LEVEL || intensity > BKLIGHT_MAX_LEVEL)
		return;
	if (!intensity) return;

	char cmd[2];

	cmd[0] = 0x72;
	cmd[1] = 0x00 | (intensity & 0x1F);

	sendByte(cmd[0]);
	sendByte(cmd[1]);
}

/****** Low level functions ******/
void TrulyBacklight::sendBitZero() {
	digitalWrite(pin_t, LOW);
	delayMicroseconds(200);
	digitalWrite(pin_t, HIGH);
	delayMicroseconds(100);
}

void TrulyBacklight::sendBitOne() {
	digitalWrite(pin_t, LOW);
	delayMicroseconds(100);
	digitalWrite(pin_t, HIGH);
	delayMicroseconds(200);
}

void TrulyBacklight::sendByte(unsigned char data) {
	int i;

	digitalWrite(pin_t, HIGH);
	delayMicroseconds(50);

	for (i = 7; i >=0; i--)
		if ((data >> i) & 0x1) sendBitOne();
		else sendBitZero();

	digitalWrite(pin_t, LOW);
	delayMicroseconds(50);
	digitalWrite(pin_t, HIGH);
}

