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

#ifndef TRULYBACKLIGHT_H
#define TRULYBACKLIGHT_H

#include <Arduino.h>

#define BKLIGHT_MAX_LEVEL 0x1F
#define BKLIGHT_MIN_LEVEL 0x00

class TrulyBacklight {

public:
	void setControlPin(int pin);
	void reset();
	void setOnOff(unsigned char on = HIGH);
	void setIntensity(unsigned char intensity = 0x1F);

private:
	int pin_t = -1;

	void sendBitZero();
	void sendBitOne();
	void sendByte(unsigned char data);
} ;

#endif

