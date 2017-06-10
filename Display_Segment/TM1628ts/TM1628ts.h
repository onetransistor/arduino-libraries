/*
	TM1628 7-Segment Display Control Library
	designed for TeleSystem TS5.9RX DVD Recorder front panel
	which uses different LED mappings from other DVD players

	Some parts of code are heavily inspired from the TM1628 library
	developed by Vasyl Yudin [https://github.com/BlockThor/TM1628]

	Copyright (C) 2017 One Transistor [https://onetransistor.github.io]

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

#ifndef TM1628ts_H
#define TM1628ts_H

#include "Arduino.h"

#define   TM1628_MS_SEP   0 // 0
#define   TM1628_VIDEO    1 // 8
#define   TM1628_HM_SEP   2 // 16
#define   TM1628_AUDIO    3 // 24
#define   TM1628_PHOTO    4 // 32
#define   TM1628_PAUSE    5 // 40
#define   TM1628_TIMER    6 // 48
#define   TM1628_DVD      7 // 56
#define   TM1628_TYP_R    9 // 72
#define   TM1628_RECORD  10 // 80
#define   TM1628_PLAY    11 // 88
#define   TM1628_TYP_W   13 // 104



class TM1628ts
{

// Segments assignment
// RAM location is at Segment number * Position
// Position is 1 to 7 from the right
//
// Segment numbers:
//
//        __2__
//       /     \
//    8 /      / 0
//      \_____/
//      /  12 \
//   4 /      / 6
//     \_____/
//       10
//
//  0   2   4   6   8  10  12
// ___ ___ ___ ___ ___ ___ ___
//  1   1   1   1   1   1   0   = 0x7e (displays 0)
//  1   0   0   1   0   0   0   = 0x48 (displays 1) ... and so on

	const unsigned char tm_digit[18] = {0x7e, 0x48, 0x73, 0x6b, 0x4d, // 0, 1, 2, 3, 4
	                                    0x2f, 0x3f, 0x68, 0x7f, 0x6f, // 5, 6, 7, 8, 9
	                                    0x7d, 0x1f, 0x36, 0x5b, 0x37, 0x35, // A, b, C, d, E, F
	                                    0x01, // minus sign
	                                    0x65 // degree sign
	                                   };

	byte tm_buffer[14] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

public:
	// Run once:
	// 1. Constructor: set the corresponding pins
	TM1628ts(int clockPin, int dataPin, int strobePin);

	// 2. Initialize display with empty RAM and keep it off (-1) or power it on with intensity 0..7
	//    Call it before before any other function !
	void init(int intensity = -1);

	// Turn on/off and set intensity
	// 1. Change intensity 0..7; -1 = display off
	void setIntensity(int intensity = -1);

	// Buffer operations
	// 1. Set digit at position in RAM buffer (overwrites previous digit)
	void putDigitAt(byte digit, int pos);

	// 2. Set a number at position in RAM (overwrites positions occupied by the number)
	void putNumberAt(long int num, int startpos = 1, int negative = 0, byte base = 10);

	// 3. Empty entire RAM buffer (-1) or a single position (1..7)
	void clearBuffer(int pos = -1);

	// 4. Set a status LED on (1) or off (0) in RAM buffer
	void setStatus(byte status, int on = 1); // use only one status code at a time !!!

	// 5. Write buffer to display controller (needed after above functions to actually send display data to controller)
	void writeBuffer();

	// Read controller
	// 1. Get keyboard mask
	byte getKeyboard();


private:
	int tm_clk, tm_dio, tm_stb;

	void tm_sendByte(byte data);
	byte tm_receiveByte();

	void tm_sendCommand(byte data);

};

#endif