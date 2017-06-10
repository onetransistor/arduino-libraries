/*
	Library for Silan SC75823 LCD controller

	Copyright (C) 2017 One Transistor [http://onetransistor.github.io]

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SC75823_H
#define SC75823_H

#include "Arduino.h"

#define SC75823_STEREO		0
#define SC75823_MP3			12
#define SC75823_AF			27
#define SC75823_TA			42
#define SC75823_TP			57
#define SC75823_DIR			75
#define SC75823_POINT		87
#define SC75823_PTY			90
#define SC75823_LOC			154

class SC75823
{
	// ASCII code - 0x20
	const unsigned short sc_font[65] =
	{
		0x0000, // Space
		0x0000, // !
		0x0410, // "
		0x0000, // #
		0x0000, // $
		0x0000, // %
		0x0000, // &
		0x0400, // '
		0x0600, // (
		0x0018, // )
		0x0718, // *
		0x0920, // +
		0x0008, // ,
		0x0820, // -
		0x0200, // .
		0x0408, // /
		0x64ce, // 0
		0x6400, // 1
		0x28e4, // 2
		0x68e0, // 3
		0x6822, // 4
		0x48e2, // 5
		0x48e6, // 6
		0x0488, // 7
		0x68e6, // 8
		0x68e2, // 9
		0x0000, // :
		0x0000, // ;
		0x0000, // <
		0x0860, // =
		0x0000, // >
		0x0000, // ?
		0x0000, // @
		0x68a6, // A
		0x69c0, // B
		0x00c6, // C
		0x61c0, // D
		0x08e6, // E
		0x08a6, // F
		0x48c6, // G
		0x6826, // H
		0x0100, // I
		0x6044, // J
		0x0700, // K
		0x0046, // L
		0x6416, // M
		0x6216, // N
		0x60c6, // O
		0x28a6, // P
		0x62c6, // Q
		0x2aa6, // R
		0x48d0, // S
		0x0180, // T
		0x6046, // U
		0x040e, // V
		0x6146, // W
		0x0618, // X
		0x0510, // Y
		0x04c8, // Z
		0x0000, // [
		0x0210, // back slash
		0x0000, // ]
		0x0000, // ^
		0x0040, // _
		0x0010  // `
	};

	const byte sc_digit[16] =
	{
		0x7b, // 0
		0x0a, // 1
		0x37, // 2
		0x1f, // 3
		0x4e, // 4
		0x5d, // 5
		0x7d, // 6
		0x4b, // 7
		0x7f, // 8
		0x5f, // 9
		0x6f, // A
		0x7c, // b
		0x71, // C
		0x3e, // d
		0x75, // E
		0x65 // F
	};

public:
	// constructor
	SC75823(int clockPin, int dataPin, int cePin, int inhPin = -1);

	// call in setup()
	void init();

	// turn on/off LCD using either hardware mode (INH pin) or software mode
	void turnOn(int on = 1, int swMode = 0);

	// buffer manipulations
	void clearBuffer();
	void putCharAt(byte data, int pos); // USE ONLY NUMBERS & UPPERCASE LETTERS!
	void putSmallDigit(byte digit); // HEX is supported
	void putString(char str[8]); // USE ONLY NUMBERS & UPPERCASE LETTERS!
	void setSignal(int strength = -1); // -1 = hide, 0 = show antenna icon, 1-3 = show antenna and level
	void setIndicator(int indicator, int on = 1); // use only one at a time
	void setInnerDisc(int centre = 1, byte mask = 0);
	void setSmallDisc(int frame = 1, byte mask = 0);
	void setBigDisc(int frame = 1, byte mask = 0);

	// write to controller - call after the above functions to send data to LCD and display it
	void writeBuffer();

private:
	int sc_clk, sc_data, sc_ce, sc_inh;
	byte sc_buffer[20] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	                     };

	void sendByte(byte data);
	void sendData();
	void sendAddress(byte address);
};

#endif