/*
	Library for Holtek HT16515 VFD Driver with FV865ND display
	
	Known Limitations: Left indicators are multiplexed and do not work
	
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

#ifndef HT16515_H
#define HT16515_H

#include "Arduino.h"

#define HT16515_DOLBY	16
#define HT16515_MS_SEP1	39
#define HT16515_DTS		40
#define HT16515_MS_SEP2	63
#define HT16515_VIDEO	64
#define HT16515_HM_SEP1	87
#define HT16515_AUDIO	88
#define HT16515_HM_SEP2	111
#define HT16515_LINK	112
#define HT16515_DH_SEP1	135
#define HT16515_HDD		136
#define HT16515_DH_SEP2	159
#define HT16515_DISC	160
#define HT16515_DVB		184

// TO DO: MULTIPLEXED
#define HT16515_EURO	-19
#define HT16515_PLAY	-18
#define HT16515_BACK	-17
#define HT16515_PAUSE	-16
#define HT16515_FWD		-15
#define HT16515_REC		-14
#define HT16515_ARROW	-13
#define HT16515_ROUND	-12 // round clock
#define HT16515_SQUARE	-11 // square clock
#define HT16515_CARD	-10
#define HT16515_NO_1	-9
#define HT16515_NO_2	-8
#define HT16515_KEY		-7
#define HT16515_16_9	-6
#define HT16515_USB		-5
#define HT16515_DVD		-4

class HT16515 {
	const unsigned short ht_font[65] = {
  0x0000, // Space
  0x0000, // !
  0x0000, // "
  0x0000, // #
  0x0000, // $
  0x0000, // %
  0x0000, // &
  0x0000, // '
  0x0000, // (
  0x0000, // )
  0x0000, // *
  0x0000, // +
  0x0000, // ,
  0x01c0, // -
  0x0000, // .
  0x0000, // /
  0x66b3, // zero
  0x0212, // 1
  0x45d1, // 2
  0x43d1, // 3
  0x03f0, // 4
  0x43e1, // 5
  0x47e1, // 6
  0x2083, //0x0231, // 7
  0x47f1, // 8
  0x43f1, // 9
  0x0000, // :
  0x0000, // ;
  0x0000, // <
  0x0000, // =
  0x0000, // >
  0x0000, // ?
  0x0000, // @
  0x07f1, // A
  0x52d5, // B
  0x4421, // C
  0x5295, // D
  0x45e1, // E
  0x05e1, // F
  0x4661, // G
  0x07f0, // H
  0x1084, // I
  0x4610, // J
  0x0da2, // K
  0x4420, // L
  0x06ba, // M
  0x0eb8, // N
  0x4631, // 0
  0x05f1, // P
  0x4e31, // Q
  0x0df1, // R
  0x42c9, // S
  0x1085, // T
  0x4630, // U
  0x24a2, // V
  0x5630, // W
  0x288a, // X
  0x108a, // Y
  0x6083, // Z
  0x0000, // [
  0x0000, // backslash
  0x0000, // ]
  0x0000, // ^
  0x0000, // _
  0x0000  // `
};
	
public:
	// constructor
	HT16515(int clockPin, int csPin, int dataWritePin, int dataReadPin);
	
	// call in setup()
	void init(int intensity = -1); // intensity = 0..7 or -1 to keep it turned off
	void setIntensity(int intensity = -1); // intensity = 0..7 or -1 to keep it turned off
	
	// Buffer manipulations
	void clearBuffer();
	void putCharAt(byte data, int pos);
	void putString(char str[8]);
	void setIndicator(int indicator, int on = 1);
	void putDigitAt(byte digit, int pos);
	void putNumberAt(long int num, int startpos = 1, int negative = 0, byte base = 10);
	
	// Write buffer data to driver IC and display it
	void writeBuffer();
	
	// Read pressed key mask
	byte readKeyboard();

private:
	int ht_clk, ht_cs, ht_di, ht_do;
	byte ht_buffer[36];
	
	void sendByte(byte data);
	byte receiveByte();
	void sendCommand(byte data);
	
};

#endif
